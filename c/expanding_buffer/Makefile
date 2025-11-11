CC = gcc
SHELL = /usr/bin/env -S bash

CFLAGS = -Wall -Wextra -Wpedantic -pedantic-errors -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -O0

.PHONY: all clear val

all: main
clear:
	rm -rvf main

val:
	$(MAKE) all
	valgrind --leak-check=yes ./main

main: main.c
