#include "monitor.h"
#include "shared.h"
#include "dll.h"

#include <threads.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

typedef struct monitor {
    void *data;
    mtx_t mtx;
    cnd_t cnd;
    flag canceled, done; // Done is always set when execution should stop, canceled is set when execution halts from an error
    dll *threads; // dll of gdata objects
} monitor;

// Love me a huge, gross macro
    // The reason I'm bothering with the huge macro is mainly for easier returns. Having this be a suite of static functions would
    // be fine, but I'd have to check for a return for each return statement in the macro. As ugly as this is, it's the easiest 
    // given C's lack of exception handling (And I'm not going to bother implementing some dark magic signal handler to try to 
    // emulate it. This is good enough for what I'm doing)
#define __monitor_inlock(m, retval, nb, sig, code) do { \
    int __monitor_inlock; \
    switch((sig)) { \
        case M_SIGTYPE_SIG: case M_SIGTYPE_BC: case M_SIGTYPE_NONE: break; \
        default: ERRRET(EINVAL, retval); \
    } \
    if((nb)) { \
        if((__monitor_inlock = mtx_trylock(&(m)->mtx)) != thrd_success) { \
            ERRRET((__monitor_inlock == thrd_busy) ? EWOULDBLOCK : ENOTRECOVERABLE, retval); \
        } \
    } else { \
        if(mtx_lock(&(m)->mtx) != thrd_success) { \
            ERRRET(ENOTRECOVERABLE, retval); \
        } \
    } \
    if((m)->done) { \
        if((m)->canceled) \
            errno = ECANCELED; \
        mtx_unlock(&(m)->mtx); \
        return retval; \
    } \
    \
    code \
    \
    switch((sig)) { \
        case M_SIGTYPE_NONE: break; \
        case M_SIGTYPE_SIG: \
        cnd_signal(&(m)->cnd); \
        break; \
        \
        case M_SIGTYPE_BC: \
        cnd_broadcast(&(m)->cnd); \
        break; \
    } \
} while (0);

monitor * m_init(void *data) {
    monitor *res = calloc(1, sizeof(*res));
    if(!res) return NULL;

    if(!(res->threads = dll_init())) {free(res); return NULL;}
    if((mtx_init(&res->mtx, mtx_recursive)) != thrd_success) {dll_free(res->threads); free(res); return NULL;}
    if((cnd_init(&res->cnd)) != thrd_success) {dll_free(res->threads); mtx_destroy(&res->mtx); free(res); return NULL;}
    res->canceled = 0;
    res->data = data;
    res->done = 0;

    return res;
}

int m_ufinish(monitor *m, flag cancel) {
    if(!m) ERRRET(EINVAL, -1);

    if(m->done) {
        if(m->canceled)
            errno = ECANCELED;
        return 1;
    }

    if(cancel)
        m->canceled = 1;
    m->done = 1;
    cnd_broadcast(&m->cnd);

    return 0;
}
int m_finish(monitor *m, flag cancel, flag nb) {
    if(!m) ERRRET(EINVAL, -1);

    int tmp;
    if(nb) { 
        if((tmp = mtx_trylock(&m->mtx)) != thrd_success) { 
            ERRRET((tmp == thrd_busy) ? EWOULDBLOCK : ENOTRECOVERABLE, -1);
        } 
    } else { 
        if(mtx_lock(&m->mtx) != thrd_success) {
            ERRRET(ENOTRECOVERABLE, -1);
        }
    }

    tmp = m_ufinish(m, cancel);
    mtx_unlock(&m->mtx);

    return tmp;
}
int m_bcancel(monitor *m) {
    return m_finish(m, 1, 0);
}
int m_tcancel(monitor *m) {
    return m_finish(m, 1, 1);
}
int m_bdone(monitor *m) {
    return m_finish(m, 0, 0);
}
int m_tdone(monitor *m) {
    return m_finish(m, 0, 1);
}


static int __monitor_free__waitonthreads(void *gd) {
    gdata *real = gd;
    if(!real) return 0;
    
    thrd_t *thread = gd_get(real);
    if(!real) return 0;
    
    thrd_join(*thread, NULL);

    return 1;
}

static int __monitor_free__gd_free_wrapper(void *gd) {
    gdata *real = gd;
    if(!real) return 0;
    gd_free(real);
    return 0;
}

void m_free(void *m, flag nb) {
    monitor *real = m;
    if(!real) return;

    if(m_finish(real, 1, nb))
        ERRRET(errno, );
    dll_foreach(real->threads, __monitor_free__waitonthreads);

    cnd_destroy(&real->cnd);
    mtx_destroy(&real->mtx);
    dll_foreach(real->threads, __monitor_free__gd_free_wrapper);
    dll_free(real->threads);
    free(real);

    return;
}
void m_bfree(void *m) {
    m_free(m, 0);
}
void m_tfree(void *m) {
    m_free(m, 1);
}

int m_lock(monitor *m) {
    if(!m) ERRRET(EINVAL, -1);
    return mtx_lock(&m->mtx);
}

int m_unlock(monitor *m) {
    if(!m) ERRRET(EINVAL, -1);
    return mtx_unlock(&m->mtx);
}

int m_wait(monitor *m) {
    if(!m) ERRRET(EINVAL, -1);
    return cnd_wait(&m->cnd, &m->mtx);
}

int m_signal(monitor *m) {
    if(!m) ERRRET(EINVAL, -1);
    return cnd_signal(&m->cnd);
}

int m_broadcast(monitor *m) {
    if(!m) ERRRET(EINVAL, -1);
    return cnd_broadcast(&m->cnd);
}

int m_dolocked(monitor *m, flag nb, enum M_SIGTYPE sig, gcb callback) {
    if(!m || !callback) ERRRET(EINVAL, -1);

    int res;
    __monitor_inlock(m, -1, nb, sig, 
        res = callback(m->data);
    )

    return res;
}

void * m_uget(monitor *m) {
    if(!m) ERRRET(EINVAL, NULL);
    return m->data;
}

int m_uset(monitor *m, void *data) {
    if(!m) ERRRET(EINVAL, -1);
    m->data = data;
    return 0;
}

int m_uisdone(monitor *m) {
    if(!m) ERRRET(EINVAL, -1);
    return m->done;
}
int m_uiscanceled(monitor *m) {
    if(!m) ERRRET(EINVAL, -1);
    return m->canceled;
}

int m_register(monitor *m, thrd_t * const thread) {
    if(!m) ERRRET(EINVAL, -1);

    gdata *toadd = gd_init(thread, NULL);
    if(!toadd) return -1;

    int res = 0;
    __monitor_inlock(m, -1, 0, M_SIGTYPE_SIG, 
        if(dll_append(m->threads, toadd)) {
            gd_free(toadd);
            res = -1;
        }
    )
    return res;
}


static void __monitor_cregister__freethread(void *thread) {
    thrd_t *real = thread;
    if(!real) return;

    thrd_detach(*real);
    free(real);
    return;
}

int m_cregister(monitor *m, gcb thread_callback) {
    if(!m || !thread_callback) ERRRET(EINVAL, -1);

    thrd_t *newthread = calloc(1, sizeof(*newthread));
    if(!newthread) return -1;

    gdata *toadd = gd_init(newthread, __monitor_cregister__freethread);
    if(!toadd) {free(newthread); return -1;}

    if(thrd_create(newthread, thread_callback, m) != thrd_success) {
        gd_free(toadd); // Handles toadd and newthread vars
        return -1;
    }

    int res = 0;
    __monitor_inlock(m, -1, 0, M_SIGTYPE_SIG, 
        if(dll_append(m->threads, toadd)) {
            gd_free(toadd);
            res = -1;
        }
    )

    return res;
}
