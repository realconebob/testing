.PHONY: deps c clean

hello_world: clay.h main.c
	gcc -o hello_world main.c -lSDL2 -lSDL2_ttf -lm

clay.h:
	wget "https://raw.githubusercontent.com/nicbarker/clay/refs/heads/main/clay.h"

deps:
	sudo apt-get install libsdl2-dev libsdl2-ttf-dev

clean:
	rm hello_world
