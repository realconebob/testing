#include "protos/repeated.pb-c.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    CMessage msg = CMESSAGE__INIT;
    void *buf;
    size_t len, i;

    msg.n_c = argc - 1;
    msg.c = calloc(msg.n_c, sizeof(int32_t));
    for(i = 0; i < msg.n_c; i++)
        msg.c[i] = atoi(argv[i+1]);

    len = cmessage__get_packed_size(&msg);
    buf = malloc(len);
    cmessage__pack(&msg, buf);

    fprintf(stderr, "Printing to stdout\n");
    fwrite(buf, len, 1, stdout);

    free(msg.c);
    free(buf);

    return 0;
}