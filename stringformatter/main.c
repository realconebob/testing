#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    const char *cutoff = "cut off the lmao";
    char *test = NULL;
    asprintf(&test, "%%.%lds", strlen(cutoff) - strlen("lmao") - 1);
    printf("%s\n", test);
    printf(test, cutoff);
    free(test);

    return 0;
}