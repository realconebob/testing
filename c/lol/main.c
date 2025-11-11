#include <stdio.h>

void dummy(const char *str) {
	printf("%s\n", str);
	return;
}

int main() {
	dummy("Original string");

	return 0;
}
