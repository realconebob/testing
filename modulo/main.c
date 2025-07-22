///usr/bin/true && gcc -std=c2x -Wall -Wextra -Wpedantic -pedantic-errors -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -O0 main.c -o Main && ./Main; exit $?

// Modulo returns the result of division, but it's weird in C. Here are some define macros that should make using it easier
	// Testing the combinations of 178 against 7, with both positive, either positive, and both negative gives consistent but unintuitive results.

#define ISNEGATIVE(sval, type) (((sval) & (1 << ((sizeof(type) * 8) - 1))) != 0)
#define ABS(sval, type) ((sval) + (-2 * (sval) * ISNEGATIVE((sval), type)))

// Simple absmod where both values are first put through the "bitwise" ABS. Returns values in the range of [0, b - 1]
#define AMOD(a, b, type) (ABS((a), type) % ABS((b), type))

// Mod oper that also preserves signage
#define SMOD(a, b, type) (AMOD((a), (b), type) * ((ISNEGATIVE((a), type) * -2) + 1) * ((ISNEGATIVE((b), type) * -2) + 1))


#include <stdio.h>

#ifndef NULL
	#define NULL ((void*)0)
#endif

typedef void (*tmcb)(int, int);

void normalmod(int val, int mod) {
	printf("Normal modulus\n\t> Normal: %d, Inverted Val: %d, Inverted Mod: %d, Inverted Both: %d\n", val % mod, (-1 * val) % mod, val % (-1 * mod), (-1 * val) % (-1 * mod));
	return;
}

void absmod(int val, int mod) {
	printf("ABS mod\n\t> Normal: %d, Inverted Val: %d, Inverted Mod: %d, Inverted Both: %d\n", AMOD(val, mod, int), AMOD((-1 * val), mod, int), AMOD(val, (-1 * mod), int), AMOD((-1 * val), (-1 * mod), int));
	return;
}

void signmod(int val, int mod) {
	printf("Sign mod\n\t> Normal: %d, Inverted Val: %d, Inverted Mod: %d, Inverted Both: %d\n", SMOD(val, mod, int), SMOD((-1 * val), mod, int), SMOD(val, (-1 * mod), int), SMOD((-1 * val), (-1 * mod), int));
	return;
}

void testmod(int val, int mod, tmcb cb) {
	if(cb != NULL)
		cb(val, mod);
	return;
}

int main(void) {
	const int val = 178; const int mod = 7;

	printf("Testing val %d with mod %d\n", val, mod);
	testmod(val, mod, normalmod);
	testmod(val, mod, absmod);
	testmod(val, mod, signmod);
	return 0;
}
