///usr/bin/true; gcc -std=c2x -Wall -Wextra -Wpedantic -pedantic-errors -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -O0 main.c -o sf && ./sf; exit $?

#include "main.h"
#include <stdio.h>




int main(void) {
	int test = 0;
	printf("%d\n", FORCETYPE(test, int));
	___defsig(int, int);

	return 0;
}
