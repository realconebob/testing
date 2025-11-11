#define _GNU_SOURCE
#define _FORTIFY_SORUCE 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define even(a) !(a & 1)

void* xcalloc(size_t, size_t);
size_t sizecomp(size_t);
int ibinsearch(int array[], int x, int low, int high);
int realbinsearch(int arr[], int check, int low, int high);

int main() {
    int x = 10;
    int testarray[] = {0, 2, 3, 6, 7, 8, 10, 19, 54, 90};
    //printf("Index of %d is at %d", x, ibinsearch(testarray, x, 0, (sizeof(testarray) / sizeof(*testarray)) - 1));
    printf("Index of %d in array is at %d\n", x, realbinsearch(testarray, x, 0, (sizeof(testarray) / sizeof(testarray[0])) - 1));

    return 0;
}

void* xcalloc(size_t nmemb, size_t size) {
    void *mem = calloc(nmemb, size);
    if(mem == NULL)
        abort();

    return mem;
}

// Divide by 2 if even, or divide by 2 and add one back if odd
size_t sizecomp(size_t size) {
    return (even(size)) ? (size >> 1) : (((size - 1) >> 1 ) + 1 );
}

// Lol this stuff doesn't work
int ibinsearch(int array[], int x, int low, int high) {
    int mid;
    
    while (low < high) {
        mid = low + ((high - low) / 2);
    
        if(array[mid] == x) {
            return mid;
        }

        if(array[mid] < x) {
            low = mid + 1;
        }

        if(array[mid] > x) {
            low = mid - 1;
        }
    }

    return -1;
}

int realbinsearch(int arr[], int check, int low, int high) {
    // Bound checking
    if(high < low)
        return -2;

    // Divide and Conquer
    

    return -1;
}




int oldmain() {
    //const char *lol = "test1";
    const char *lol2 = "test2";

    // Init values
    size_t len = 0, prev = -1;
    bool isOdd = false;
    char **hold = xcalloc(2, sizeof(*hold));
    //char *rhold = NULL;


    len = sizecomp(strlen(lol2));
    isOdd = !even(strlen(lol2));
    // split
    printf("Original String: %s\n", lol2);

    for(size_t i = 0; len != prev; i++) {
        hold[0] = strndup(lol2, len);
        hold[1] = strndup(lol2+len-isOdd, len);

        printf("Iter: %ld\n1st half: %s\n2nd half: %s\nlen: %ld\n\n", i + 1, hold[0], hold[1], len);

        prev = len;
        len = sizecomp(len);
        isOdd = !even(strlen(hold[0]));

        free(hold[0]);
        free(hold[1]);
    }

    return 0;
}