#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Takeaway: Using log10 over log(x)/log(10) is accurate to more decimal points, but has no difference compared
// to log2. The other log functions like log10 and log2 do the same error handling as log, so there's no reason
// to bother with the change of base formula whenever they can be used. If you have some weirdo base like 3 or 17
// then it could still be of use, but when do you ever deal with log 143 outside of pre-calc? */

int lastinstr(const char *str, const char *delim) {
	int index = -1;

	size_t len = strlen(delim);
	const char *p = str;
	for(int i = 0; *p; p++, i++) {
		if(strncmp(p, delim, len) == 0)
			index = i;
	}

	return index;
}

double logX(double base, double x) {
	return (log(x) / log(base));
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "USAGE: %s <number>\n", argv[0] + lastinstr(argv[0], "/") + 1);
		return 1;
	}

	int x = atoi(argv[1]);

	// Print the log10 of the number using log10() and change of base
	printf("log10:\t\t%.20f\nlog10 (cob):\t%.20f\n\n", log10(x), logX(10, x));

	// Print the log2 of the number
	printf("log2:\t\t%.20f\nlog2 (cob):\t%.20f\n", log2(x), logX(2, x));

	return 0;
}
