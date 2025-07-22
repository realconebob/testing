#include <stdio.h>

#define STATIC_ARRSIZE(arr) (sizeof((arr))/sizeof((arr)[0]))

int main(void) {
	for(int i = 0; i < 32; i++) {
		printf("1 << %d = %d\n", i, (1<<i));
	}

	return 0;
}
