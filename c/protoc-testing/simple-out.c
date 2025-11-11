#include "protos/simple.pb-c.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    AMessage msg = AMESSAGE__INIT;
    void *buf = NULL;
    size_t len = 0;

    const int num1 = 9;
    const int num2 = 30;

    msg.a = num1;
    msg.b = num2;

    len = amessage__get_packed_size(&msg);

    buf = malloc(len);
    amessage__pack(&msg, buf);

    fprintf(stderr, "Writing message to stdout");
    fwrite(buf, len, 1, stdout);
    
    return 0;
}