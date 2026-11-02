CXX := /usr/bin/clang++
CXXFLAGS := -std=c++20 -fsanitize=address,undefined -Og -ggdb -Wall -Wextra -Wpedantic

TOCLEAN := build .cache

.PHONY: remake build c clean

remake:
	@$(MAKE) clean >/dev/null 2>&1
	@$(MAKE) build -j

build:
	@cmake -B $@ \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_FLAGS="${CXXFLAGS}"
	@cmake --build $@ -j

c clean:
	-@rm -rvf build .cache