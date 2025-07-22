#include <threads.h>
#include <stdarg.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>

enum stdiomtx {
    STDMTX_STDIN = 0,
    STDMTX_STDOUT = 1,
    STDMTX_STDERR = 2
};

int init_stdiomtx(mtx_t stdiom[3]) {
    for(int i = 0; i < 3; i++) {
        if(mtx_init(&stdiom[i], mtx_recursive | mtx_timed) == thrd_error) {
            error(0, errno, "Could not create %d of 3 mutexes", i);
            return -1;
        }
    }

    return 0;
}

int tfprintf(mtx_t mutex, FILE *stream, const char *format, ...) {
    va_list ap;
    va_start(ap, format);

    mtx_lock(&mutex);
    vfprintf(stream, format, ap);
    mtx_unlock(&mutex);

    va_end(ap);
    return 0;
}

struct funargs {
    mtx_t *stdmtx;
    const char *format;
};

int fun(void *args) {
    mtx_t *stdmtx = ((struct funargs*)args)->stdmtx;
    const char *format = ((struct funargs*)args)->format;

    tfprintf(stdmtx[STDMTX_STDOUT], stdout, format);
    return 0;
}

int main() {
    mtx_t stdmtx[3];
    init_stdiomtx(stdmtx);
    
    tfprintf(stdmtx[STDMTX_STDOUT], stdout, "This should print before anything in a thread does\n");

    struct funargs lmao;
    lmao.format = "This should print after\n";
    lmao.stdmtx = stdmtx;
    

    thrd_t thread;
    thrd_create(&thread, fun, (void*)&lmao);

    thrd_join(thread, NULL);
    return 0;
}