CC = gcc
SHELL = /usr/bin/env -S bash

CFLAGS = -Wall -Wextra -Wpedantic -pedantic-errors -ggdb -g3 -Og

.PHONY: all clean c py python

all: main
main: main.c

c clean:
	rm -rvf main

py python:
	@python3 main.py
