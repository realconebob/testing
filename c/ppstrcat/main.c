///usr/bin/true; gcc -Wall -Wextra -Wpedantic main.c -o main && ./main && rm main; exit $?

#include <stdio.h>

#define WORLD "world"

int main(void) {
	printf("One quote set: hello world\n");
	printf("Several quote sets: hello" " world" "\n");
	printf("Several quote sets & define macro: " "hello " WORLD "\n");
	return 0;
}
