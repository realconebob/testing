#include <stdio.h>

int main() {
    for(int i = 'a'; i < 'z' + 1; i++)
        printf("%c = %d\n", i, i);
    printf("\n");

    printf("Please enter a char: ");
    int c = getchar();
    printf("Entered: %c, next: %c\n", c, ((c - 'a' + 1) % ('z' - 'a' + 1)) + 'a');

    return 0;
}