#include <stdio.h>

#define NORMALIZE(x, y, z, a, b) ((((x) - (y)) / ((z) - (y)) * ((b) - (a))) + (a))
#define BNORMALIZE(x, y, z, a, b) (((((x) * (b)) - ((x) * (a)) - ((y) * (b)) + ((y) * (a))) / ((z) - (y))) + (a))

int normalize(int x, int maxx, int minx, int nmin, int nmax) {
	return (((x - minx) / (maxx - minx) * (nmax - nmin)) + nmin);
}

int bnormalize(int x /*Value being normalized*/, int y /*Min value of x*/, int z /*Max value of x*/, int a /*New min val*/, int b /*New max val*/) {
	return ((((x * b) - (x * a) - (y * b) + (y * a)) / (z - y)) + a);
}

int main() {
	printf("7:[1, 10]->[5, 300] (func): %d\n7:[1, 10]->[5, 300] (#def): %d\n\n", 	normalize(7, 10, 1, 5, 300),	(int)NORMALIZE(7, 1, 10, 5, 300));
	printf("7:[1, 10]->[5, 300] (bfunc): %d\n7:[1, 10]->[5, 300] (b#def): %d\n\n", 	bnormalize(7, 1, 10, 5, 300),	(int)BNORMALIZE(7, 1, 10, 5, 300));

	for(int i = 0; i <= 25; i++)
		printf("%d:[3, 28]->[0, 1000] (b#def): %d\n", i + 3, (int)BNORMALIZE(i+3, 3, 28, 0, 1000));

	return 0;
}
