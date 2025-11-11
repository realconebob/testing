#include <stdlib.h>

int main() {
	char *test = NULL;
	test = malloc(10);
	test = malloc(10);
	free(test);

	return 0;
}
