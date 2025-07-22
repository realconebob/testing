#include <stdio.h>

int func1() {
    return 0;
}
int func2() {
    return 20;
}
int func3() {
    return -4;
}
int func4() {
    return -44;
}

#define lol (int (* [])()){func1, func2, func3, func4}

int main() {
    /*
    int x = 10;
    char eff = 'f';
    double floating = 0.9;

    int *p = &x;
    //printf("%d\n", *p);
    */

    for(size_t i = 0; i < (sizeof(lol) / sizeof(lol[0])); i++) {
        printf("func%ld: %d\n", i + 1, (*lol[i])());
    }

    char *lmao = (char[]){'l', 'm', 'a', 'o'};
    printf("%c%c%c%c\n", lmao[0], lmao[1], lmao[2], lmao[3]);

    return 0;
}