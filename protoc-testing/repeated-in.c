#include "protos/repeated.pb-c.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_MSG_SIZE 4096

static size_t read_buffer(size_t max_length, uint8_t *out) {
    size_t curlen = 0;
    size_t nread;

    while((nread = fread(out + curlen, 1, max_length - curlen, stdin)) != 0) {
        curlen += nread;
        if(curlen >= max_length) {
            fprintf(stderr, "stuff brokey\n");
            exit(-1);
        }
    }

    return curlen;
}


int main() {
    CMessage *msg;
    uint8_t buf[MAX_MSG_SIZE];
    size_t msg_len = read_buffer(MAX_MSG_SIZE, buf);

    msg = cmessage__unpack(NULL, msg_len, buf);
    if(msg == NULL) {
        fprintf(stderr, "reading msg broke\n");
        exit(-1);
    }

    for(size_t i = 0; i < msg->n_c; i++) {
        if(i > 0)
            printf(", ");
        printf("%d", msg->c[i]);
    }
    printf("\n");

    cmessage__free_unpacked(msg, NULL);

    return 0;
}