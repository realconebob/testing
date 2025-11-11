#if defined _GNU_SOURCE
#error wtf are you doing? The whole point of this exercise is to reimplement these functions yourself dummy
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int vasprintf(char **strp, const char * const fmt, va_list ap) {
	va_list ap2;
	int b = -1, ret = -1;

	va_copy(ap2, ap);
	b = vsnprintf(NULL, 0, fmt, ap2);
	va_end(ap2);

	if(b < 0) // vsnprintf returns a negative value on error
		return -1;
	b++; // vsnprintf does not account for the null byte

	char *str = NULL;
	str = malloc(b * sizeof(char));
	if(!str) {
		return -1;
	}

	if((ret = vsnprintf(str, b, fmt, ap)) < 0) {
		free(str);
		return -1;
	}

	*strp = str;
	return ret;
}

int asprintf(char **strp, const char * const fmt, ...) {
	int ret = -1;
	va_list ap;

	va_start(ap, fmt);
	ret = vasprintf(strp, fmt, ap);
	va_end(ap);

	return ret;
}

int main() {
	char *str = NULL;
	if(asprintf(&str, "data data data %d", 9000) < 0) {
		fprintf(stderr, "asprintf broke");
		return 1;
	}

	printf("%s\n", str);
	free(str);

	return 0;
}
