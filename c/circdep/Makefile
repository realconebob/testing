.PHONY: clear all

all: test1 test2
test1: test1.c a.c b.c
test2: test2.c c.c d.c
a: a.c b.c
b: b.c a.c
c: c.c d.c
d: d.c c.c

clear:
	rm a b c d test1 test2

# ok suspicions confirmed. can't get away with circular deps even with header guards