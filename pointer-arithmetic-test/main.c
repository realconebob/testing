#include <stdio.h>

int main(void) {
	// Indexing arrays through pointer arithmetic, as I understand it, is
	// as simple as adding (i * sizeof(elem)) to the base pointer, and then
	// dereferencing it. IIRC this is what's happening when you use the []
	// notation, it's just doing it in the background for you

	int test[] = {1, 2, 3};

	/*
	printf("%d\n%d\n%d\n",\
		*(test + (0 * sizeof(int))),\
		*(test + (1 * sizeof(int))),\
		*(test + (2 * sizeof(int)))\
	);
	*/

	// So the first attempt did not work. Lets try by casting to a void*

	void *fake = test;

	printf("%d\n%d\n%d\n",\
		*((int*)((unsigned char*)fake + (0 * sizeof(int)))),\
		*((int*)((unsigned char*)fake + (1 * sizeof(int)))),\
		*((int*)((unsigned char*)fake + (2 * sizeof(int))))\
	);

	// Ayyyy my intuition was right, I just wrote it wrong the first time

	return 0;
}
