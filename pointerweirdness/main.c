#include <stdio.h>

#ifndef NULL
	#define NULL ((void*)0)
#endif

/* Ok yeah it turns out I was just being dumb. I only needed to dereference
// p once to get the original value, not twice. By doing it twice, if p is null,
// you cause a null pointer dereference and explode everything */

void checkNums(const int **p, int check, const int *a, const int *b, const int *c) {
	const int * const start = *p;

	*p = a;
	if(**p == check)
		return;

	*p = b;
	if(**p == check)
		return;

	*p = c;
	if(**p == check)
		return;

	*p = start;

	return;
}

int main() {
	int a = 5;
	int b = 6;
	int c = 7;

	int e = 9;

	const int *d = &e;


	printf("A: %d, %p\nB: %d, %p\nC: %d, %p\nE: %d, %p\n\n",\
		a, (void*)&a,\
		b, (void*)&b,\
		c, (void*)&c,\
		e, (void*)&e);

	checkNums(&d, 8, &a, &b, &c);

	printf("*D: %d, %p\n", (d == NULL) ? 0 : *d, (void*)d);

	return 0;
}


/* First trial. Making sure I'm not crazy and that by passing a pointer to a
// pointer it is possible to change the value of the original pointer through
// dereferencing */

void changePointer(const int **p, const int *a, const int *b) {
	(*p) = ((*a) > (*b)) ? a : b;
	return;
}

int firsttest() {
	int a = 5;
	int b = 6;
	const int *c = NULL;

	printf("A: %d, %p\nB: %d, %p\n", a, (void*)&a, b, (void*)&b);

	changePointer(&c, &a, &b);

	if(c != NULL)
		printf("*C: %d, %p\n", *c, (void*)c);

	return 0;
}
