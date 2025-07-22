#include <stdio.h>

#define STATIC_ARRSIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))
#define isEven(num) (!((num) % 2))

int doSumOfEvens(const int data[], int n, int i, int s) {
	if(i >= (n - 1))
		return s;

	if(isEven(data[i]))
		s += data[i];

	return doSumOfEvens(data, n, i + 1, s);
}

int sumOfEvens(const int data[], int n) {
	return doSumOfEvens(data, n, 0, 0);
}

int main() {
	const int data[] = {1, 2, 3, 4, 5, 6, 7}; // Expected outcome: 2 + 4 + 6 = 12
	printf("%d\n", sumOfEvens(data, STATIC_ARRSIZE(data)));
	return 0;
}