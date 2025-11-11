#include <stdio.h>
#include <stdlib.h>
#include "protos/simple.pb-c.h"
#define MAX_MSG_SIZE 1024

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


int main () {
  AMessage *msg;

  // Read packed message from standard-input.
  uint8_t buf[MAX_MSG_SIZE];
  size_t msg_len = read_buffer (MAX_MSG_SIZE, buf);

  // Unpack the message using protobuf-c.
  msg = amessage__unpack(NULL, msg_len, buf);	
  if (msg == NULL)
  {
    fprintf(stderr, "error unpacking incoming message\n");
    exit(1);
  }

  // display the message's fields.
  printf("Received: a=%d",msg->a);  // required field
  if (msg->has_b)                   // handle optional field
    printf("  b=%d",msg->b);
  printf("\n");

  // Free the unpacked message
  amessage__free_unpacked(msg, NULL);
  return 0;
}