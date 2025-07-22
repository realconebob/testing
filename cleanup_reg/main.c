///usr/bin/true; gcc -Wall -Wextra -Wpedantic main.c -o cleanup && ./cleanup; exit $?

#include <stdio.h>
#include <errno.h>
#include <error.h>

#define RETURNWERR(errval, retval) do {\
    errno = (errval);\
    return (retval);\
} while (0)

typedef void (*cleanup_callback)(void*);
typedef struct cl {
	cleanup_callback *funcs;
	void **args;

	int size;
	int used;
} cleanup;

int cleanup_init(cleanup * const loc, int size, cleanup_callback funcs[], void *args[]) {
	if(!loc)
		RETURNWERR(EINVAL, -1);
	if(size < 1)
		RETURNWERR(EINVAL, -1);
	if(!funcs)
		RETURNWERR(EINVAL, -1);
	if(!args)
		RETURNWERR(EINVAL, -1);

	loc->funcs = funcs;
	loc->args = args;
	loc->size = size;
	loc->used = 0;

	return 0;
}

int cleanup_register(cleanup * const loc, cleanup_callback cb, void *arg) {
	if(!loc)
		RETURNWERR(EINVAL, -1);
	if(loc->used >= loc->size)
		RETURNWERR(ENOMEM, -1);
	if(!cb)
		RETURNWERR(EINVAL, -1);

	loc->funcs[loc->used] = cb;
	loc->args[loc->used] = arg;
	loc->used++;

	return 0;
}

int cleanup_cndregister(cleanup * const loc, unsigned char flag, cleanup_callback cb, void *arg) {
	if(flag)
		return 0;

	if(!loc)
		RETURNWERR(EINVAL, -1);
	if(loc->used >= loc->size)
		RETURNWERR(ENOMEM, -1);
	if(!cb)
		RETURNWERR(EINVAL, -1);

	loc->funcs[loc->used] = cb;
	loc->args[loc->used] = arg;
	loc->used++;

	return 0;
}

int cleanup_clear(cleanup * const loc) {
	if(!loc)
		RETURNWERR(EINVAL, -1);

	loc->used = 0;
	return 0;
}

cleanup_callback cleanup_peekf(cleanup * const loc) {
	if(!loc)
		RETURNWERR(EINVAL, NULL);
	if(loc->used == 0)
		RETURNWERR(ENODATA, NULL);

	return loc->funcs[loc->used - 1];
}
cleanup_callback cleanup_popf(cleanup * const loc) {
	cleanup_callback cb = cleanup_peekf(loc);
	if(cb == NULL)
		RETURNWERR(EINVAL, NULL);

	loc->used--;

	return cb;
}

void * cleanup_peeka(cleanup * const loc) {
	if(!loc)
		RETURNWERR(EINVAL, NULL);
	if(loc->used == 0)
		RETURNWERR(ENODATA, NULL);

	return loc->args[loc->used - 1];
}
void * cleanup_popa(cleanup * const loc) {
	void *mem = cleanup_peeka(loc);
	if(!mem)
		RETURNWERR(EINVAL, NULL);
	
	loc->used--;

	return mem;
}

int cleanup_fire(cleanup * const loc) {
	if(!loc)
		RETURNWERR(EINVAL, -1);

	for(int i = (loc->used - 1); i >= 0; i--)
		loc->funcs[i](loc->args[i]);
	loc->used = 0;

	return 0;
}

#define cleanup_create(size) \
cleanup __CLEANUP; \
cleanup_callback __CLEANUP_FUNCS[(size)]; \
void *__CLEANUP_ARGS[(size)]; \
cleanup_init(&__CLEANUP, (size), __CLEANUP_FUNCS, __CLEANUP_ARGS)

void print(void *data) {
    if(!data)
        return;

    static int inc = 0;
    printf("%d: %s\n", ++inc, (char*)data);
    return;
}

int main() {
    cleanup_create(3);

    cleanup_register(&__CLEANUP, print, "first registered");
    cleanup_register(&__CLEANUP, print, "second registered");
    cleanup_register(&__CLEANUP, print, "third registered");
    cleanup_register(&__CLEANUP, print, "fourth registered");
    cleanup_fire(&__CLEANUP);

    cleanup_register(&__CLEANUP, print, "fifth registered");
    cleanup_register(&__CLEANUP, print, "sixth registered");
    cleanup_register(&__CLEANUP, print, "seventh registered");
    cleanup_register(&__CLEANUP, print, "eighth registered");
    cleanup_fire(&__CLEANUP);

    return 0;
}