#include "shared.c"
#include <string.h>

#define ARRLEN ((int)10)

int main() {
    int *lmao = xcalloc(ARRLEN, sizeof(*lmao));
    memset(lmao, 90, ARRLEN);
    free(lmao);

    return 0;
}