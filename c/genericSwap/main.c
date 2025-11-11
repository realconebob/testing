///usr/bin/env -S gcc -std=c2x -Wall -Wextra -Wpedantic -pedantic-errors -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -Og main.c && ./a.out; exit $?

#include <stdio.h>
#include <stdlib.h>

typedef struct ts {
	char* str;
	int val;

} teststruct;

teststruct* teststruct_init(char * const str, int val) {
	teststruct *ts = malloc(1 * sizeof(*ts));
	if(!ts)
		abort();

	ts->str = str;
	ts->val = val;

	return ts;
}

#define DSWAP(x, y, T)\
	do {\
		T DSWAP = (x);\
		(x) = (y);\
		(y) = DSWAP;\
	} while (0)
// This is honestly so  smart I can't believe it
// https://stackoverflow.com/a/3982397

void dvswap(void* *a, void* *b) {
	void *tmp = *a;
	*a = *b;
	*b = tmp;
	return;
}

void vswap(void *a, void *b) {
	void *tmp = a;
	a = b;
	b = tmp;
	return;
}

void int_dvswap(void) {
	int a = 5;
	int b = 6;
	printf("a: %d\nb: %d\n", a, b);

	dvswap((void**)&a, (void**)&b);
	printf("a: %d\nb: %d\n", a, b);
		// This "works" but causes stack smashing

	return;
}

void int_dswap(void) {
	int a = 5;
	int b = 6;
	printf("a: %d\nb: %d\n", a, b);

	DSWAP(a, b, int);
	printf("a: %d\nb: %d\n", a, b);
		// Ok, it works with normal values. What about pointers to structs?

	return;
}

void struct_dswap(void) {
	teststruct *t1 = teststruct_init("string 1", 10);
	teststruct *t2 = teststruct_init("string 2", 11);
	printf("t1: %d, %s\nt2: %d, %s\n\n", t1->val, t1->str, t2->val, t2->str);

	DSWAP(t1, t2, teststruct*);
	printf("t1: %d, %s\nt2: %d, %s\n", t1->val, t1->str, t2->val, t2->str);

	free(t1);
	free(t2);

	return;
}

void int_vswap(void) {
	int a = 5;
	int b = 6;
	printf("a: %d, b: %d\n", a, b);

	vswap(&a, &b);
	printf("a: %d, b: %d\n", a, b);

	return;
}

int main(void) {
	//int_dvswap();
	//int_dswap();

	//struct_dswap();

	int_vswap();

	return 0;
}
