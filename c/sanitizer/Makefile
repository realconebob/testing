CC = gcc
SHELL = /usr/bin/bash

.PHONY: address hwaddress thread clean warning macro

warning:
	@echo -ne "Please specify whether to make \"address\", \"hwaddress\", or \"thread\"\n"

address:
	gcc main.c -fsanitize=address -o main

hwaddress:
	gcc main.c -fsanitize=address -o main

thread:
	gcc main.c -fsanitize=address -o main
