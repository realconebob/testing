CC = gcc
SHELL = /usr/bin/bash

CFLAGS = -Wall -Wextra -Wpedantic -fanalyzer -Wanalyzer-too-complex

CFLAGS += $$(pkg-config --cflags libevent) $$(pkg-config --cflags libsodium)
LDFLAGS += $$(pkg-config --libs-only-L libevent) $$(pkg-config --libs-only-L libsodium)
LDLIBS +=  $$(pkg-config --libs-only-l libevent) $$(pkg-config --libs-only-l libsodium)

.PHONY: all clean

all: client
client: client.o
client.o: client.c

clean:
	rm -rvf client{,.o}