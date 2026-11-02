CXX := /usr/bin/clang++
CXXFLAGS := -std=c++20 -ggdb -Og -Wall -Wextra -Wpedantic
.PHONY: remake c clean

remake:
	@$(MAKE) clean >/dev/null 2>&1
	@$(MAKE) main -j

main: main.cpp crtp.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

c clean:
	-@rm -f main ||: