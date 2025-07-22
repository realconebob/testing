#define _GNU_SOURCE

#include "shared.h"
#include "monitor.h"
#include "dll.h"

#include <threads.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>

static int __t_dll__dlnprintf(void *dlndata) {
    static int iter = 0;
    printf("foreach [%d]: %s\n", iter++, (dlndata) ? (char*)dlndata : "null");
    return 1;
}
static void __t_dll(void) {
    printf("Running dll test...\n\n");
    dll *doubly = dll_init();

    dll_append(doubly, "This");
    dll_append(doubly, "is");
    dll_append(doubly, "some");
    dll_append(doubly, "text");

    dll_prepend(doubly, "Some");
    dll_prepend(doubly, "More");
    dll_prepend(doubly, "Data");

    dll_insert(doubly, 0, "Insert 1");
    dll_insert(doubly, dll_size(doubly), "Insert 2");
    dll_insert(doubly, 2, "Insert 3");

    dll_prepend(doubly, "buffer 1");
    dll_append(doubly, "buffer 2");

    dll_foreach(doubly, __t_dll__dlnprintf);

    char *tmp; int i;
    i = 6;printf("dll_get @ i=%d: %s\n", i, ((tmp = dll_get(doubly, i))) ? tmp : "null");
    i = 0; printf("dll_get @ i=%d: %s\n", i, ((tmp = dll_get(doubly, i))) ? tmp : "null");
    i = dll_size(doubly) - 1; printf("dll_get @ i=%d: %s\n", i, ((tmp = dll_get(doubly, i))) ? tmp : "null");
    
     
    printf("dll_first: %s\n", ((tmp = dll_first(doubly))) ? tmp : "null");
    printf("dll_last: %s\n", ((tmp = dll_last(doubly))) ? tmp : "null");
    

    dll_free(doubly);

    printf("Finished dll test!\n\n");
    return;
}



static void __t_gd__printnfree(void *data) {
    if(!data) return;
    printf("Print 'n Free: %s\n", ((data) ? (char*)data : "null"));
    free(data);
    return;
}
static void __t_gd(void) {
    printf("Running gdata test...\n\n");
    gdata *gd1 = gd_init("This is a statically allocated value", NULL);
    gdata *gd2 = gd_init(strdup("This is a dynamically allocated value"), free);

    char *res;
    res = gd_get(gd1); printf("%s\n", ((res) ? res : "null"));
    res = gd_get(gd2); printf("%s\n", ((res) ? res : "null"));

    gd_set(gd1, "This is a new, statically allocated value");
    free(res); gd_set(gd2, strdup("This is a new, dynamically allocated value"));
    res = gd_get(gd1); printf("%s\n", ((res) ? res : "null"));
    res = gd_get(gd2); printf("%s\n", ((res) ? res : "null"));

    if(gd_getcb(gd1) == free)
        gd_setcb(gd1, __t_gd__printnfree);
    if(gd_getcb(gd2) == free)
        gd_setcb(gd2, __t_gd__printnfree);

    gd_free(gd1);
    gd_free(gd2);
    printf("Finished gdata test!\n\n");
}



static int __t_monitor_pt1__mtcounter(void *mon) {
    monitor *real = mon;
    if(!real) return -1;

    m_lock(real);
    int *counter = m_uget(real);
    *counter += 1;
    m_unlock(real);

    return 0;
}
static int __t_monitor_pt1__freethread(void *thread) {
    thrd_t *real = thread;
    if(!real) return -1;

    thrd_detach(*real);
    free(real);
    return 1;
}
static void __t_monitor_pt1(void) {
    printf("Running monitor test 1...\n\n");
    int count = 0; const int DESIRED_COUNT = 100;
    monitor *mon = m_init(&count, mtx_plain);

    dll *threads = dll_init();
    for(int i = 0; i < DESIRED_COUNT; i++) {
        thrd_t *cur = calloc(1, sizeof(*cur));
        thrd_create(cur, __t_monitor_pt1__mtcounter, mon);
        dll_append(threads, cur);
    }
    sleep(1);

    dll_foreach(threads, __t_monitor_pt1__freethread);
    dll_free(threads);

    printf("%d\n", count);
    assert(count == DESIRED_COUNT);
    
    m_bfree(mon);
    printf("Finished monitor test 1!\n\n");
    return;
}



static int __t_monitor_pt2__producer(void *mon) {
    if(!mon) return -1;

    monitor *real = mon;
    m_lock(real);

    // static int counter = 0;
    // if(counter > 100) {
    //     m_ufinish(real, 0);
    //     m_unlock(real);
    //     return 1;
    // }
    // counter++;

    char *data;
    if(asprintf(&data, "PRODUCED DATA")) {
        m_unlock(real);
        return -1;
    }

    dll *list = m_uget(real);
    dll_append(list, data);

    m_signal(real);
    m_unlock(real);

    return 0;
}
static int __t_monitor_pt2__consumer(void *mon) {
    if(!mon) return -1;
    
    monitor *real = mon;
    m_lock(real);

    dll *list = m_uget(real);
    while(dll_isempty(list) && !m_uisdone(real))
        m_wait(real);
    if(m_uisdone(real)) {
        m_unlock(real);
        return 1;
    }

    char *data = dll_last(list);
    printf("Consuming: %s\n", (data) ? data : "null");
    free(data);

    m_unlock(real);
    return 0;
} 
static void __t_monitor_pt2(void) {
    printf("Running monitor test 2...\n\n");

    monitor *mon = m_init(dll_init(), mtx_recursive);
    m_cregister(mon, __t_monitor_pt2__producer);
    m_cregister(mon, __t_monitor_pt2__consumer);

    sleep(1);
    m_bfree(mon);

    printf("Finished monitor test 2!\n\n");
    return;
}



int main(void) {
    // __t_dll();
    // __t_gd();
    // __t_monitor_pt1();
    __t_monitor_pt2();

    return 0;
}