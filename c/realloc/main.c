#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// I don't know why but realloc{,array}() gets me confused when it really shouldn't. This is me putting my suspicions to rest

#define TEST_STRING ((const char*)"This is a test string of arbitrary length")

int test_realloc(void) {
	// realloc() first:
		// From the manpages:
			/* The realloc() function changes the size of the memory block pointed to by ptr to size bytes.  The contents will be unchanged in the
			// range from the start of  the  region  up to the minimum of the old and new sizes.  If the new size is larger than the old size, the
			// added memory will not be initialized.  If ptr is NULL, then the call is equivalent to malloc(size), for all values of size; if size is
			// equal to zero, and ptr is not NULL, then the call is equivalent  to free(ptr).  Unless ptr is NULL, it must have been returned by an
			// earlier call to malloc(), calloc(), or realloc().  If the area pointed to was moved, a free(ptr) is done.

			// The realloc() function returns a pointer to the newly allocated memory, which is suitably aligned for any built-in type, or NULL if the
			// request failed. The  returned pointer may be the same as ptr if the allocation was not moved (e.g., there was room to expand the
			// allocation in-place), or different from ptr if the allocation was moved to a new address.  If size was equal to 0, either NULL or a pointer
			// suitable to be passed to free() is returned.  If realloc() fails, the original block is left untouched; it is not freed or moved. */

		// So from my understanding:
			// void *mem = realloc(null, 10) -- Allocates 10 bytes of memory, akin to malloc(10)
			// char *str ... realloc(str, 0) -- Frees whatever's held at str, akin to free(str)
			// char tmp[20]; realloc(tmp, 30) -- NOT VALID, ENTIRE COMPUTER WILL EXPLODE LIKE TSAR BOMBA

			// realloc(ptr, n), can return NULL, ptr, or NEWPTR:
				// NULL - Couldn't allocate for some reason. Original block of memory untouched
					// Because of this, `test = realloc(test, n)` is a bad idea, because this can overwrite the pointer and leak it
				// ptr - Pointer didn't change because the array could be expanded in-place. No need to worry
				// NEWPTR - The interesting case; New pointer because the old one couldn't be expanded in-place. Whatever was in that previous array has been copied over to the new one. Supposedly, the old array is also free()'d in the background.

				// Overall takeaway: Use a temp variable just to check that the result isn't null. Then, assign the old pointer to the new one


	const int DEFAULT_SIZE = 20;
	const int DIFF_SIZE = 20 * 2;

	// malloc()-like behavior
	char *str = realloc(NULL, DEFAULT_SIZE);
	if(!str) {
		fprintf(stderr, "Could not allocate space for str");
		exit(EXIT_FAILURE);
	}

	strncpy(str, TEST_STRING, DEFAULT_SIZE);
	str[DEFAULT_SIZE - 1] = '\0';
	printf("%s\n", str);


	// realloc() to a larger size
	void *tmp = realloc(str, DIFF_SIZE);
	if(!tmp) {
		fprintf(stderr, "Could not reallocate space for str");
		exit(EXIT_FAILURE);
	}
	str = tmp;

	strncpy(str, TEST_STRING, DIFF_SIZE);
	str[DIFF_SIZE - 1] = '\0';
	printf("%s\n", str);


	// free()-like behavior
	str = realloc(str, 0);
	if(str)
		free(str);

	return 0;
}



int test_reallocarray(void) {
	// reallocarray() now
		// From the manpages:
			/* The reallocarray() function changes the size of the memory block pointed to by ptr to be large enough for an array of nmemb elements,
			// each of  which  is size bytes.  It is equivalent to the call:
				// realloc(ptr, nmemb * size);

       			// However, unlike that realloc() call, reallocarray() fails safely in the case where the multiplication would overflow.  If such an
			// overflow occurs, reallocarray() returns NULL, sets errno to ENOMEM, and leaves the original block of memory unchanged.*/


	// So reallocarray is basically a wrapper of realloc(), like how calloc is a wrapper for malloc()


	const int DEFAULT_SIZE = 20;
	const int LARGER_SIZE = DEFAULT_SIZE * 2;
	const char *DEFAULT_STR = "This is a string of arbitrary length";

	// malloc()-like behavior
	char *str = NULL;
	str = reallocarray(str, DEFAULT_SIZE, sizeof(char));
	if(!str) {
		fprintf(stderr, "Could not allocate room for string");
		exit(EXIT_FAILURE);
	}

	strncpy(str, DEFAULT_STR, DEFAULT_SIZE);
	str[DEFAULT_SIZE - 1] = '\0';
	printf("%s\n", str);

	// reallocate to larger size
	char *tmp = reallocarray(str, LARGER_SIZE, sizeof(char));
	if(!tmp) {
		free(str);
		fprintf(stderr, "Could not reallocate space for str\n");
		exit(EXIT_FAILURE);
	}
	str = tmp;

	strncpy(str, DEFAULT_STR, LARGER_SIZE);
	str[LARGER_SIZE - 1] = '\0';
	printf("%s\n", str);

	// free()-like behavoir
	str = reallocarray(str, 0, 0);
	if(str)
		free(str);

	return 0;
}

int main(void) {
	test_realloc();
	test_reallocarray();
}
