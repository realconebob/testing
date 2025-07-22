#!/usr/bin/env -S python3

def doSumOfEvens(data: list[int, ...], n: int, i: int, s: int) -> int:
	if(i >= (n-1)):
		return s

	if(data[i] % 2 == 0):
		s = s + data[i]

	return doSumOfEvens(data, n, i + 1, s)

def sumOfEvens(data: list[int, ...], n: int) -> int:
	return doSumOfEvens(data, n, 0, 0)

if __name__ == "__main__":
	data: list[int, ...] = [1, 2, 3, 4, 5, 6, 7] # expected result: 2 + 4 + 6 = 12
	print(f"{sumOfEvens(data, len(data))}")

	exit(0)
