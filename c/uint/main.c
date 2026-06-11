#include <stdio.h>
#include <stdint.h>

int main(void) {
	printf("uint64_t size: %ld\n", sizeof(uint64_t));
	printf("(uint64_t)-1: %lud\n", (uint64_t)-1);
	printf("(1 << (sizeof(uint64_t) * 8)) - 1: %lud\n", (uint64_t)((1 << (sizeof(uint64_t) * 8)) - 1));
	return 0;
}
