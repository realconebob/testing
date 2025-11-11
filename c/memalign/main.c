#include <stdio.h>

#define MEM_ALIGN_BYTES 16 /* Equivalent to (2 * sizeof(void*)) */
#define MEM_ALIGN(x) ((x) + (((x) & (MEM_ALIGN_BYTES - 1)) != 0) * (MEM_ALIGN_BYTES - ((x) & (MEM_ALIGN_BYTES - 1))))

int main(void) {
	printf("%d\n", MEM_ALIGN(17));
	return 0;
}
