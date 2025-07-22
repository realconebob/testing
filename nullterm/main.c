#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define sample "THIS IS SOME SAMPLE TEXT"

int main() {
	long len = strlen(sample) + 1;
	char* lol = calloc(len, sizeof(*lol));
	strncpy(lol, sample, len - 1);
	lol[len - 1] = '\0';
	printf("%s\n", lol);
	return 0;
}
