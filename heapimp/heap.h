#ifndef __HEAPIMP___HEAP_H___299472989129768
#define __HEAPIMP___HEAP_H___299472989129768

typedef struct h heap;

#include <stdint.h>

// Initialize a heap. Returns a new heap object or null on error
heap* heap_init(uint64_t size);

// Free a heap
int heap_free(heap **h);

// Check if a heap is empty
int heap_isEmpty(const heap * const heap);

// Push some new data onto a heap
int heap_push(heap * const heap, void *data, int64_t prio);

// Pop the top data from a heap (removes the data from the heap)
void* heap_pop(heap * const heap);

// Peek the top data from a heap (does not remove the data)
void* heap_peek(const heap * const heap);

// Invert the priorities of each element in the heap
int heap_invert(heap * const heap);

// Get a new heap populated with the contents of another array
heap* heap_heapifyarr(void * const data[], int64_t priorities[], uint64_t len, uint64_t dlen);

// Change the priority of an object in the heap
int heap_changeprio(heap * const heap, uint64_t index, int64_t prio);

// Sort an existing array using an array of priorities
int heapsort(void **arr[], int64_t priorities[], uint64_t len, char direction);

#endif