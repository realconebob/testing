///usr/bin/true; gcc -std=c2x -Wall -Wextra -Wpedantic main.c -o main && ./main; res="$?"; rm -f main; exit "$res"

#include <stdio.h>

typedef struct thing {
	int a, b, c;
	char *str;
} thing;

const thing things[] = {
	{1, 2, 3, "Hello"},
	{4, 5, 6, "World"},
	{0}
};

int main(void) {
	char *test = "This is a string";
	for(char *cur = test; *cur; cur++)
		printf("%c ", *cur);
	printf("\n");



	return 0;
}
