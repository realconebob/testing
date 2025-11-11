SHELL := /usr/bin/bash
CC := gcc
CFLAGS = -Wall -Wextra -Wpedantic -fanalyzer -Wanalyzer-too-complex -ggdb -Og

#CFLAGS +=
LDLIBS += -lm
#LDFLAGS +=

.PHONY: all clean

all: main
main: main.c

clean:
	rm -rvf main $(wildcard *.o)
