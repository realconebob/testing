///usr/bin/true; gcc -std=c2x -Wall -Wextra -Wpedantic main.c -o main && ./main && rm ./main; exit $?

#include <stdio.h>

#define GETBIT(val, pos) (((val) & (1 << (pos))) != 0)

#define SETBIT(val, pos) ((val) |= 1 << (pos))
#define CLRBIT(val, pos) ((val) &= ~(1 << (pos)))
#define FLPBIT(val, pos) ((val) ^= 1 << (pos))


#define GETBITS(val, mask) ((val) & (mask))

#define SETBITS(val, mask) ((val) |= (mask))
#define CLRBITS(val, mask) ((val) &= ~(mask))
#define FLPBITS(val, mask) ((val) ^= (mask))

#define ABSBITS(val, set, mask) ((val) = (((val) | ((set) & (mask))) & (((set) & (mask)) | ~(mask))))

int main(void) {
	const unsigned char TESTBYTE = 0b01100011;
	unsigned char working = TESTBYTE;

	printf("Original Val: %d\n", working);
	printf("Get 2nd bit: %d\n", GETBIT(working, 2));
	printf("Set 7th bit: %d\n", SETBIT(working, 7));
	printf("Clear 7th bit: %d\n", CLRBIT(working, 7));
	printf("Flip 3th bit: %d\n", FLPBIT(working, 3));
	printf("Flip 3th bit: %d\n\n", FLPBIT(working, 3));

	printf("Set bits 1 and 2: %d\n", SETBITS(working, 0b00000110));
	printf("Clear bits 1 and 2: %d\n", CLRBITS(working, 0b00000110));
	printf("Flip bits 6 and 7: %d\n\n", FLPBITS(working, 0b11000000));

	printf("ABSBITS clear: %d\n", ABSBITS(working, 0b00000000, 0b11111111));
	printf("ABSBITS fill: %d\n", ABSBITS(working, ~0b00000000, 0b11111111));
	printf("ABSBITS set: %d\n", ABSBITS(working, 0b10101010, 0b11110000));

	return 0;
}

// Needed a refresher on bitmasking
