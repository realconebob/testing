#define _GNU_SORUCE
#define _FORTIFY_SOURCE 2

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define STACKSIZE 8

void test();
void* xcalloc(size_t, size_t);

void A();
void B(void (*ptr)());

int main() {
    void (**teststack)(void) = xcalloc(STACKSIZE, sizeof(*teststack)); // Holy stuff lol
    (teststack[0]) = test;

    (*teststack[0])();

    void (*ptr)() = &A;
    B(ptr);

    return 0;
}

void test() {
    static int iter = 1;
    printf("lol %d\n", iter++);
    return;
}

void* xcalloc(size_t nmemb, size_t size) {
    void *mem = calloc(nmemb, size);
    if(mem == NULL)
        abort();

    return mem;
}

void A() {
    printf("I am function A\n");
    return;
}

void B(void (*ptr)()) {
    (*ptr)();
    return;
}