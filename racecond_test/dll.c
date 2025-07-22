#include "dll.h"
#include "shared.h"

#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

typedef struct dlln {
    void *data;
    struct dlln *prev;
    struct dlln *next;
} dlln;
typedef struct dll {
    dlln *first;
    dlln *last;
    int size;
} dll;

dlln * dllnode_init(void *data, dlln *const prev, dlln *const next) {
    dlln *res = calloc(1, sizeof(*res));
    if(!res) return NULL;

    res->data = data;
    res->prev = prev;
    res->next = next;

    return res;
}

void dlln_free(void *dllnode) {
    dlln *real = dllnode;
    if(!real) return;
    free(real);
    return;
}

dll * dll_init(void) {
    dll *res = calloc(1, sizeof(*res));
    if(!res) return NULL;

    res->first = NULL;
    res->last = NULL;
    res->size = 0;

    return res;
}

void dll_free(void *list) {
    dll *real = list;
    if(!real) return;

    
    for(dlln *p = real->first, *n; p != NULL;) {
        n = p->next;
        free(p);
        p = n;
    }
    free(real);

    return;
}

static int dll_dofirst(dll *const list, void *data) {
    if(!list) ERRRET(EINVAL, -1);
    if(list->size != 0) return 0;

    dlln *firstnode = dllnode_init(data, NULL, NULL);
    if(!firstnode) return -1;

    list->first = firstnode;
    list->last = firstnode;
    list->size = 1;

    return 1;
}


int dll_prepend(dll *const list, void *data) {
    if(!list) ERRRET(EINVAL, -1);

    int fres = 0;
    if((fres = dll_dofirst(list, data))) {return fres;}

    dlln *newnode = dllnode_init(data, NULL, list->first);
    list->first->prev = newnode;
    list->first = newnode;
    list->size++;

    return 0;
}

int dll_append(dll *const list, void *data) {
    if(!list) ERRRET(EINVAL, -1);

    int fres = 0;
    if((fres = dll_dofirst(list, data))) {return fres;}

    dlln *newnode = dllnode_init(data, list->last, NULL);
    list->last->next = newnode;
    list->last = newnode;
    list->size++;

    return 0;
}

int dll_insert(dll *const list, int index, void *data) {
    if(!list || index < 0 || index > list->size) ERRRET(EINVAL, -1);
    
    int fres = 0;
    if((fres = dll_dofirst(list, data))) {return fres;}

    if(index == 0) return dll_prepend(list, data);
    if(index == list->size) return dll_append(list, data);

    // I could make this run on O(n/2) average but I don't care enough to bother, as this is a test
    dlln *target = list->first, *after;
    for(int i = 0; i < (index - 1) && target != NULL;) {i++; target = target->next;}
    if(!target) ERRRET(ENOTRECOVERABLE, -1); // Target shouldn't be null, as the only case it would be null is handled by dll_append
    after = target->next;

    dlln *newnode = dllnode_init(data, target, after);
    target->next = newnode;
    after->prev = newnode;

    list->size++;

    return 0;
}

void * dll_first(dll *const list) {
    if(!list) ERRRET(EINVAL, NULL);
    if(list->size <= 0) ERRRET(ENODATA, NULL);

    dlln *tofree = list->first;
    void *res = tofree->data;

    if(list->size == 1) {
        list->first = NULL;
        list->last = NULL;

    } else if(list->size == 2) {
        list->first = list->last;
        list->first->prev = NULL;
        
    } else {
        list->first = tofree->next;
        list->first->prev = NULL;

    }

    list->size--;
    free(tofree);
    return res;
}

void * dll_last(dll *const list) {
    if(!list) ERRRET(EINVAL, NULL);
    if(list->size <= 0) ERRRET(ENODATA, NULL);

    dlln *tofree = list->last;
    void *res = tofree->data;

    if(list->size == 1) {
        list->first = NULL;
        list->last = NULL;

    } else if(list->size == 2) {
        list->last = list->first;
        list->first->next = NULL;

    } else {
        list->last = list->last->prev;
        list->last->next = NULL;
    
    }

    list->size--;
    free(tofree);
    return res;
}

void * dll_get(dll *const list, int index) {
    if(!list || index < 0 || index >= list->size) ERRRET(EINVAL, NULL);
    if(list->size <= 0) ERRRET(ENODATA, NULL);

    if(index == 0) return dll_first(list);
    if(index == list->size - 1) return dll_last(list);

    dlln *target = list->first, *after, *before;
    for(int i = 0; i < index && target != NULL;) {i++; target = target->next;}
    if(!target) ERRRET(ENOTRECOVERABLE, NULL); // Null edgecase handled by dll_last
    after = target->next;
    before = target->prev;

    void *res = target->data;
    before->next = after;
    after->prev = before;
    free(target);

    return res;
}

int dll_size(dll *const list) {
    if(!list) ERRRET(EINVAL, -1);
    return list->size;
}

int dll_isempty(dll *const list) {
    if(!list) ERRRET(EINVAL, -1);
    if(list->size < 0) ERRRET(ENOTRECOVERABLE, -1);
    return (list->size == 0);
}

int dll_foreach(dll *const list, gcb callback) {
    if(!list || !callback) ERRRET(EINVAL, -1);

    for(dlln *p = list->first; p != NULL; p = p->next) {
        if(!(callback(p->data)))
            break;
    }

    return 0;
}