#include "heap.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct hn {
    void *data;
    int64_t prio;
} heapnode;
typedef struct h {
    heapnode **arr; // array of heapnode pointers
    uint64_t size;
    uint64_t used;
} heap;

#define DSWAP(x, y, T)\
	do {\
		T DSWAP = (x);\
		(x) = (y);\
		(y) = DSWAP;\
	} while (0)

heapnode* heapnode_init(void *data, int64_t prio) {
    heapnode *hn = malloc(1 * sizeof(*hn));
    if(!hn)
        return NULL;

    hn->data = data;
    hn->prio = prio;

    return hn;
}

heap* heap_init(uint64_t size) {
    heap *h = malloc(1 * sizeof(*h));
    if(!h)
        return NULL;

    h->arr = malloc(size * sizeof(*(h->arr)));
    if(!h->arr) {
        free(h);
        return NULL;
    }

    h->size = size;
    h->used = 0;

    return h;
}

int heap_free(heap **h) {
    if(!h)
        return -1;
    if(!(*h))
        return -1;

    for(uint64_t i = 0; i < (*h)->used; i++)
        free((*h)->arr[i]);
    free((*h)->arr);
    free(*h);

    (*h) = NULL;
    return 0;
}

int heap_heapify(heap * const heap, uint64_t index) {
    if(!heap)
        return -1;

    uint64_t lefti = (index * 2) + 1;
    uint64_t righti = (index * 2) + 2;
    uint64_t largesti = index;

    if(lefti < heap->used && heap->arr[lefti]->prio > heap->arr[largesti]->prio)
        largesti = lefti;
    if(righti < heap->used && heap->arr[righti]->prio > heap->arr[largesti]->prio)
        largesti = righti;

    if(largesti != index) {
        DSWAP(heap->arr[largesti], heap->arr[index], heapnode*);
        heap_heapify(heap, largesti);
    }

    return 0;
}

int heap_heapifyup(heap * const heap, uint64_t index) {
    if(!heap)
        return -1;
    
    if(index == 0)
        return 0;

    uint64_t parenti = (index - 1) / 2;
    if(heap->arr[parenti]->prio < heap->arr[index]->prio) {
        DSWAP(heap->arr[parenti], heap->arr[index], heapnode*);
        heap_heapifyup(heap, parenti);
    }

    return 0;
}

int heap_isEmpty(const heap * const heap) {
    return (heap->used <= 0);
}

int heap_push(heap * const heap, void *data, int64_t prio) {
    if(!heap)
        return -1;
    if(heap->used == heap->size)
        return -2;

    heapnode *hn = heapnode_init(data, prio);
    if(!hn)
        return -1;

    heap->arr[heap->used] = hn;
    heap_heapifyup(heap, heap->used);
    heap->used += 1;

    return 0;
}

void* heap_pop(heap * const heap) {
    if(!heap)
        return NULL;
    if(heap->used == 0)
        return NULL;

    void *data = heap->arr[0]->data;
    free(heap->arr[0]); heap->arr[0] = NULL;
    heap->arr[0] = heap->arr[heap->used - 1];
    heap->arr[heap->used - 1] = NULL;
    heap->used--;

    heap_heapify(heap, 0);

    return data;
}

void* heap_peek(const heap * const heap) {
    if(!heap)
        return NULL;
    if(heap->used == 0)
        return NULL;

    return heap->arr[0]->data;
}

int heap_invert(heap * const heap) {
    if(!heap)
        return -1;
    
    for(uint64_t i = 0; i < heap->used; i++)
        heap->arr[i]->prio *= -1;
    
    for(uint64_t i = (heap->used / 2) - 1;; i--) {
        heap_heapify(heap, i);

        // uint -> unsigned, so the loop won't exit properly unless this is done
        if(i == 0)
            break;
    }

    return 0;
}

heap* heap_heapifyarr(void * const data[], int64_t priorities[], uint64_t len, uint64_t dlen) {
    if(!data)
        return NULL;
    if(!priorities)
        return NULL;

    heap *heap = malloc(1 * sizeof(*heap));
    if(!heap)
        return NULL;

    if(dlen == 0)
        dlen = len;

    heap->arr = malloc((heap->size = dlen) * sizeof(*(heap->arr)));
    if(!heap->arr) {
        free(heap);
        return NULL;
    }

    heap->used = len;

    for(uint64_t i = 0; i < heap->used; i++) {
        heap->arr[i] = heapnode_init(data[i], priorities[i]);
        
        // This is gross but should prevent any memory leaks
        if(!heap->arr[i]) {
            for(uint64_t j = 0; j < i; j++)
                free(heap->arr[j]);

            free(heap->arr);
            free(heap);
            return NULL;
        }
    }

    for(uint64_t i = (heap->used / 2) - 1;; i--) {
        heap_heapify(heap, i);

        // uint -> unsigned, so the loop won't exit properly unless this is done
        if(i == 0)
            break;
    }

    return heap;
}

int heap_changeprio(heap * const heap, uint64_t index, int64_t prio) {
    if(!heap)
        return -1;
    if(index >= heap->used)
        return -1;
    
    int64_t oldprio = heap->arr[index]->prio;
    heap->arr[index]->prio = prio;

    if(oldprio > prio)
        heap_heapifyup(heap, index);
    if(oldprio < prio)
        heap_heapify(heap, index);

    return 0;
}

int heapsort(void **arr[], int64_t priorities[], uint64_t len, char direction) {
    // Make sure the data array being passed in is real
    if(!arr)
        return -1;
    if(!*arr)
        return -1;

    // Make sure the priorities array is real 
    if(!priorities)
        return -1;

    // Make sure the user is using a valid direction
    if(direction != 'h' && direction != 'l')
        return -1;

    // Get the heap
    heap* heap = heap_heapifyarr(*arr, priorities, len, 0);
    if(!heap)
        return -1;

    // Invert the heap if asked
    if(direction == 'l')
        heap_invert(heap);
        
    // Pop values from the heap into the user's array
    for(uint64_t i = 0; i < len; i++)
        (*arr)[i] = heap_pop(heap);

    // Free the heap now that it's not necessary
    heap_free(&heap);

    return 0;
}