///usr/bin/true; gcc -Wall -Wextra -Wpedantic main.c -o passthrough && ./passthrough; exit $?

#include <stdio.h>

typedef struct testing {
	int a;
} test;

void lol(test input[], test *output, int len) {
	if(!input || len < 0)
		return;

	*output = input[len / 2];

	return;
}

int main() {
	test structure[] = {{4},{5},{6},{7}};
	test output;

	lol(structure, &output, 4);
	printf("%d\n", output.a);

	return 0;
}
