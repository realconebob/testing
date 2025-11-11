CC = gcc
SHELL = /usr/bin/bash

# Don't know why but my gcc doesn't like c23, so I'm using c2x
CFLAGS = -std=c2x -Wall -Wextra -Wpedantic -pedantic -pedantic-errors -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -O0

.PHONY: all c clean val

all: main

c clean:
	rm -rvf main

val:
	$(MAKE) all
	valgrind --leak-check=yes ./main

main: main.c