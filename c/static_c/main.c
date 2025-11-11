// This is because I'm confused on exactly how initial assignment for static variables in a C function work

#include <stdio.h>
#include <stdlib.h>

int test() {
	static int lmao = 0;
	lmao++;
	return lmao;
}

int* pointerTest() {
	static int *p = NULL;

	if(p != NULL)
		return p;

	p = calloc(1, sizeof(*p));
	if(!p)
		fprintf(stderr, "Could not allocate space for P");

	return p;
}

int main() {
	test();
	test();

	printf("%d\n", test());

	int *p = pointerTest();
	*p = 10;

	int *d = pointerTest();
	(*d)++;

	printf("p: %p\nd: %p\n*p: %d, *d: %d\n", (void*)p, (void*)d, *p, *d);
}

// Ok, everything worked how I thought it would. Good to know
