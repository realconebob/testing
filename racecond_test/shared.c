#include "shared.h"
#include <stdlib.h>
#include <errno.h>

// My favorite thing about C is how much the code complexity blooms whenever I want to do things the correct way

typedef struct gdata {
    void *data;
    fcb freer;
} gdata;

gdata * gd_init(void *data, fcb freer) {
    gdata *res = calloc(1, sizeof(*res));
    if(!res) return NULL;

    res->data = data;
    res->freer = freer;
    return res;
}

void gd_free(void *gd) {
    gdata *real = gd;
    if(!real) ERRRET(EINVAL, );

    if(real->freer)
        real->freer(real->data);
    free(real);

    return;
}

void * gd_get(gdata *gd) {
    if(!gd) ERRRET(EINVAL, NULL);
    return gd->data;
}

int gd_set(gdata *gd, void *data) {
    if(!gd) ERRRET(EINVAL, -1);
    gd->data = data;
    return 0;
}

fcb gd_getcb(gdata *gd) {
    if(!gd) ERRRET(EINVAL, NULL);
    return gd->freer;
}

int gd_setcb(gdata *gd, fcb newcallback) {
    if(!gd) ERRRET(EINVAL, -1);
    gd->freer = newcallback;
    return 0;
}