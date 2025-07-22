///usr/bin/true; gcc -Wall -Wextra -Wpedantic main.c -o ifs && ./ifs; exit $?

#include <stdio.h>

#define abs(val) ((val) + (-2 * (val) * (((val) & (1 << ((sizeof((val)) * 8) - 1))) != 0)))

int rflagtest(int count) {
	if(abs(count) > 2)
		return -1;

	printf("count: %d\n", count);
	if(!count)
		printf("\t!count\n");
	if(count == 0)
		printf("\tcount == 0\n");

	if(count)
		printf("\tcount\n");
	if(count != 0)
		printf("\tcount != 0\n");
	printf("\n");

	return rflagtest((abs(count) + 1) * (1 + (-2 * (count < 0))));
}

int flagtest(void) {
	return rflagtest(0);
}

int main(void) {
	flagtest();
	return 0;
}
