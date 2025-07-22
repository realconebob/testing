#!/usr/bin/env -S python3

import random

def mergesort(array: list[int,...]) -> list[int,...]:
	width: int = 1
	arraylen: int = len(array)
	max: int = arraylen

	start: int = -1
	end: int = -1

	while width < arraylen:
		# Set up the correct values
		max = arraylen
		width *= 2
		if width > arraylen:
			width = arraylen

#		while max > 0:
#			max -= width

		pass

	return

if __name__ == "__main__":
	nums: list[int,...] = []
	for i in range(1, 11):
		nums.append(i)
	random.shuffle(nums)
	print(nums)

	mergesort(nums)

	pass
