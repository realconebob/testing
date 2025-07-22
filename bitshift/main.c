///usr/bin/env -S gcc main.c && ./a.out; exit $?

#include <stdio.h>

#define ISNEGATIVE(sval, type) (((sval) & (1 << ((sizeof(type) * 8) - 1))) != 0)
#define ABS(sval, type) ((sval) + (-2 * (sval) * ISNEGATIVE((sval), type)))

int main(void) {
	printf("%d %d\n", ABS(10, int), ABS(-10, char));
	return 0;
}