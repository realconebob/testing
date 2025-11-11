CC = gcc
SHELL = /usr/bin/bash

CFLAGS = -Wall -Wextra -Wpedantic -fanalyzer -Wanalyzer-too-complex

CFLAGS += $$(pkg-config --cflags libevent)
LDFLAGS += $$(pkg-config --libs-only-L libevent)
LDLIBS +=  $$(pkg-config --libs-only-l libevent)

.PHONY: all clean

all: server
server: server.o
server.o: server.c

clean:
	rm -rvf server{,.o}