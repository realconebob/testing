///usr/bin/env -S make && ./main; exit $?

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    heap *heap = heap_init(10);
    if(!heap)
        abort();

    heap_push(heap, (void*)"This is a string", 3);
    heap_push(heap, (void*)"This is another string", 5);
    heap_push(heap, (void*)"This is also a string", 1);
    
    heap_changeprio(heap, 1, -9);

    while(!heap_isEmpty(heap))
        printf("%s\n", (char*)heap_pop(heap));

    heap_free(&heap);

    return 0;
}