#include <stdlib.h>

#define STATIC_ARRSIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

void* xcalloc(size_t nmemb, size_t size) {
    void *mem = calloc(nmemb, size);
    if(!mem)
        abort();

    return mem;
}