CC = gcc
SHELL := /usr/bin/env
.SHELLFLAGS := -S bash -c

CFLAGS := -std=c2x -Wall -Wextra -Wpedantic -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -O0
DEFS := -D_GNU_SOURCE=1

SOURCES := $(wildcard *.c)
TIMESTAMP_DIR := .timestamps
TIMESTAMPS := $(patsubst %.c,$(TIMESTAMP_DIR)/%.t,$(SOURCES))

.PHONY: c clean t test
.DELETE_ON_ERROR:
.ONESHELL:

main: main.c

$(TIMESTAMP_DIR):
	mkdir -p $(TIMESTAMP_DIR)

$(TIMESTAMPS): $(TIMESTAMP_DIR)/%.t: %.c | $(TIMESTAMP_DIR)
	touch $@

main tests: %: %.c $(TIMESTAMPS)
	$(CC) $(DEFS) $(CFLAGS) $^ -o $@

# Phony rules
c clean:
	-rm -rvf main tests $(TIMESTAMP_DIR) $(wildcard *.test*) $(wildcard *.enc)

t test: tests
	valgrind --leak-check=yes ./tests
