SHELL := /usr/bin/env -S bash
CC := gcc

CFLAGS = -Wall -Wextra -Wpedantic -fanalyzer -Wanalyzer-too-complex -Og -ggdb

.PHONY: all clean

all: main
main: main.c

clean:
	rm -rvf main
