///usr/bin/env -S gcc --std=c2x -Wall -Wextra -Wpedantic -pedantic-errors -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -Og test.c -o test && ./test && exit $?

#include <stdio.h>

int main() {
	printf("This is a c program\n");
	return 0;
}
