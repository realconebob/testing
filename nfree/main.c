#include <stdlib.h>
#include <stdio.h>

void nfree(void *p) {
	free(p);
	p = NULL;
	return;
}

void nnfree(void **p) {
	free(*p);
	*p = NULL;
	return;
}

int main() {
	int *p = malloc(1 * sizeof(*p));
	if(!p)
		return -1;

	nnfree((void*)&p);
	printf("%s\n", (p == NULL) ? "p is null" : "p is not null");

	return 0;
}
