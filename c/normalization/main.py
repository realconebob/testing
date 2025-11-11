#!/usr/bin/env -S python3

import random

if __name__ == "__main__":
	# To scale from one linear range to another, use this formula (supposedly)
	# x(normal) = (b - a) * ((x - x(min)) / (max x - min x)) + a, where [a, b] is the new range

	i: int = 0
	while i < 255:
		print(f"Original value: {i}, Normalized value: {(1000 - 0) * ((i - 0) / (255 - 0)) + 0}")
		i += random.randrange(0, 20, 1)

	if i > 255:
		i = 255
		print(f"Original value: {i}, Normalized value: {(1000 - 0) * ((i - 0) / (255 - 0)) + 0}")

	pass
