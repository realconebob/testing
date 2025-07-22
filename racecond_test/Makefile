CC = gcc
SHELL := /usr/bin/env
.SHELLFLAGS := -S bash -c

CFLAGS = -std=c2x -Wall -Wextra -Wpedantic -pedantic-errors -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -O0 

CFLAGS +=
LDLIBS +=
LDFLAGS +=

.PHONY: all c clean v val
.DELETE_ON_ERROR:
.ONESHELL:

all: main

main: main.o monitor.o dll.o shared.o
main.o: main.c shared.h monitor.h dll.h
monitor.o: monitor.c monitor.h shared.h
dll.o: dll.c dll.h shared.h
shared.o: shared.c shared.h

c clean:
	rm -rvf main $(wildcard *.o)

v val:
	$(MAKE) all
	valgrind --leak-check=yes ./main