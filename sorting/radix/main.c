#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <math.h>

#ifndef NULL
	#define NULL ((void*)0)
#endif

#define STATIC_ARRSIZE(arr) (sizeof((arr)) / sizeof((*arr)))

int collapsestr(const char* str) {
	unsigned int final = 0;

	const char* p = str;
	for(;*p;p++)
		final += *p;

	return final;
}

typedef signed long long int slli;
slli powimp(int base, int exp) {
	slli result = 1;
	for(int i = 0; i < exp; i++) {
		result *= base;
	}

	return result;
}

int getdigit(int base, int number, int index) {
	return (number / powimp(base, index)) % base;
}

int largestinarray(const int list[], size_t len) {
	if(list == NULL) {
		fprintf(stderr, "getlargestinlist: array is null");
		return -1;
	}

	int maximum = list[0];
	for(size_t i = 0; i < len; i++)
		if(list[i] > maximum)
			maximum = list[i];

	return maximum;
}

double logX(int base, double x) {
	switch(base) {
		case 2:
			return log2(x);

		case 10:
			return log10(x);
	}

	return (log(x) / log(base));
}

int digitwidth(int base, int number) {
	//return floor(log10(abs(number))) + 1;
	return floor(logX(base, abs(number))) + 1;
}

int safechartoint(const char* str, size_t index) {
	// I hate this
	if(index >= strlen(str))
		return 0;

	return str[index];
}

int lsdradix(char *(*strarr)[], size_t len) {
	// strarr is a pointer to an array of strings
		// char * = string
		// char *[] = array of strings
		// char *(*)[] = pointer to array of strings
			// I'm only doing this as a way to modify the array of strings in-place

	// Set up holder arrays
	char *startZero[len];
	char *startOne[len];

	// Get the number of passes that will have to be done
	size_t charpasses = 0; size_t bytepasses = 0;
	for(size_t i = 0, tmp = 0, tmp2 = 0; i < len; i++) {
		if((tmp = strlen((*strarr)[i])) > charpasses)
			charpasses = tmp;

		for(size_t j = 0; j < tmp; j++) {
			if((tmp2 = digitwidth(2, (*strarr)[i][j])) > bytepasses)
				bytepasses = tmp2;
		}
	}

	// One pass for each character of the largest string
		// One pass for each digit of the largest representation of a char
			// One pass for each string in the array
			// O(k1 * k2 * w)
	

	return 0;
}

int main(/* int argc, char *argv[] */) {
	//printf("%d\n", getdigit(293746, 10));

//	int testarr[] = {4, 7, 2023, 1};
//	size_t testarrsize = (sizeof(testarr) / sizeof(*testarr));

	char *strings[] = {"lmao", "schlmao3", "AAAAAAAAA", "lmao2"};
	size_t strslen = STATIC_ARRSIZE(strings);

	printf("%d\n", lsdradix(&strings, strslen));

	for(size_t i = 0; i < strslen; i++)
		printf("%s\n", strings[i]);

	return 0;
}
