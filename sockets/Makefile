CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -Og

.PHONY: all, clean

all:
	$(MAKE) clean
	$(MAKE) server client
	
clean:
	rm -fv client server

server: server.c
client: client.c