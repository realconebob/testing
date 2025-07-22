///usr/bin/env -S gcc main.c && ./a.out; exit $?

#include <stdio.h>

enum TESTING {
	TESTING_INVAL,

	TESTING_A,
	TESTING_B,
	TESTING_C,

	TESTING_2BIG
};

int test(enum TESTING val) {
	if(val <= TESTING_INVAL || val >= TESTING_2BIG)
		return -1;

	switch(val) {
		case TESTING_A:
		printf("a");
		break;

		case TESTING_B:
		printf("b");
		break;

		case TESTING_C:
		printf("c");
		break;

		default:
			return -1;
	}

	return val;
}

int main(void) {
	enum TESTING val;
	printf("%d\n", test(val));

	return 0;
}
