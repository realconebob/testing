CC = gcc
SHELL = /usr/bin/bash

CFLAGS = -std=c2x -Wall -Wextra -Wpedantic -pedantic-errors -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -O0

.PHONY: all c clean val

all: main

c clean:
	rm -rvf main $(wildcard *.o)

val:
	$(MAKE) all
	valgrind --leak-check=yes ./main

main: main.o heap.o
main.o: main.c heap.h
heap.o: heap.c heap.h