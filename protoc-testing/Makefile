CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -fanalyzer -Wanalyzer-too-complex -ggdb -Og -g3 
LDFLAGS = -lprotobuf-c

BINARY_FILES := simple-out simple-in repeated-out repeated-in

.PHONY: all clear

all: $(BINARY_FILES)

clean:
	rm -rvf $(BINARY_FILES)

simple-out: simple-out.c protos/simple.pb-c.c protos/simple.pb-c.h
	$(CC) $(CFLAGS) simple-out.c protos/simple.pb-c.c -o simple-out $(LDFLAGS) 

simple-in: simple-in.c protos/simple.pb-c.c protos/simple.pb-c.h
	$(CC) $(CFLAGS) simple-in.c protos/simple.pb-c.c -o simple-in $(LDFLAGS)

repeated-out: repeated-out.c protos/repeated.pb-c.c protos/repeated.pb-c.h
	$(CC) $(CFLAGS) repeated-out.c protos/repeated.pb-c.c -o repeated-out $(LDFLAGS)

repeated-in: repeated-in.c protos/repeated.pb-c.c protos/repeated.pb-c.h
	$(CC) $(CFLAGS) repeated-in.c protos/repeated.pb-c.c -o repeated-in $(LDFLAGS)