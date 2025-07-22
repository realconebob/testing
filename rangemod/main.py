#!/usr/bin/env -S python3

def rangemod(x: int, offset: int, min: int, max: int) -> int:
    return ((x - min + offset) % (max - min + 1)) + min

# This stuff sucks, don't use it. My version is better as long as the language can deal with negative values in modulus operations
def brangemod(startingValue: int, offset: int, minimumValue: int, modulus: int):
	return (startingValue - minimumValue + (offset % modulus) + modulus) % modulus + minimumValue

if __name__ == "__main__":
	#print(f"7 + 1 rangemod 3-5: {rangemod(7, 1, 3, 5)}")
	#print(f"7 + 2 rangemod 3-5: {rangemod(7, 2, 3, 5)}")

	for i in range(1, 4 + 1):
		print(f"7 + {i} rangemod 3-5: {rangemod(7, i, 3, 5)}")

	print()

	for i in range(6, 13 + 1):
		print(f"7 + {i} rangemod 3-5: {rangemod(7, i, 3, 5)}")

	print()

	for i in range(1, -7, -1):
		print(f"7 + {i} rangemod 3-5: {rangemod(7, i, 3, 5)}")


	# better version, supposedly
	print(f"{brangemod(10, -20, 5, 15)}")

	pass
