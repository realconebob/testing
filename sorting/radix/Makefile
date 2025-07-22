SHELL := /usr/bin/bash

CFLAGS += -Wall -Wextra -Wpedantic -fanalyzer -Wanalyzer-too-complex -ggdb -Og
LDLIBS += -lm
#LDFLAGS +=

.PHONY: all clean

all: main
main: main.c

clean:
	rm -rvf main $(wildcard *.o)
