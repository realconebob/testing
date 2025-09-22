#include <stdio.h>

void modbypointer(int *val) {
	if(!val) return;
	*val = *val + 1;
	return;
}

void modbyreference(int &val) {
	val += 1;
	return;
}


int main(void) {
	int x = 10;
	modbypointer(&x);
	printf("%d\n", x);

	modbypointer(x);
	printf("%d\n", x);

	return 0;
}

// Yeah this is C++ specific syntax. Not sure why syntaxdb says otherwise
