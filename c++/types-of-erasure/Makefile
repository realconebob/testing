CXX := /usr/bin/g++
SHELL := /usr/bin/env
.SHELLFLAGS := -S bash -c

CXXFLAGS := -std=gnu++20 -D_GNU_SOURCE=1 -Wall -Wextra -Wpedantic -Og -ggdb -g3 -fsanitize=address,undefined

TARGETS := standard half-template full-crtp

.PHONY: debug release c clean
.DELETE_ON_ERROR:
.ONESHELL:

TOCLEAN := build .cache

.PHONY: remake build c clean

remake:
	$(MAKE) clean >/dev/null 2>&1
	$(MAKE) build -j

build:
	cmake -B $@ \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_COMPILER=${CXX} \
		-DCMAKE_CXX_FLAGS="${CXXFLAGS}"
	cmake --build $@ -j

c clean:
	rm -rvf build .cache