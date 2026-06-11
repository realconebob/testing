#ifndef ___CONPRO__TASK_C__24792278357532__
#define ___CONPRO__TASK_C__24792278357532__

#include "shared.c"

#include <stddef.h>
#include <stdlib.h>

typedef int (*callback)(void*);
typedef void (*freer)(void*);

typedef struct task {
    callback cb;
    freer fcb;
    void *data;
} task;

task * task_new(callback cb, freer fcb, void *data) {
    if(!cb) return NULL;

    task *t = calloc(1, sizeof(*t));
    if(!t) return NULL;

    t->cb = cb;
    t->fcb = fcb;
    t->data = data;

    return t;
}

int task_fire(task *t) {
    if(!t) return -1;
    return t->cb(t->data);
}

void task_free(void *t) {
    
}

#endif