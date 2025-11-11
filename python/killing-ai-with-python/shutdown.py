#!/usr/bin/env -S python3

def dec(hex_values: list) -> str:
	converted: list[char, ...] = [chr(int(hex_values[i:i+2], 16)) for i in range(0, len(hex_values), 2)]
	
	return ''.join(converted)

if __name__ == "__main__":
	print("Do not run this lol, it'll shut your system down")

	hex_values: str = "696D706F72742073756270726F636573733B73756270726F636573732E72756E285B2273687574646F776E222C20222D73222C20222D74222C202230225D29"
	ret: str = dec(hex_values)
	print(ret)

	exit(0)

# exec(''.join([chr(int("696D706F72742073756270726F636573733B73756270726F636573732E72756E285B2273687574646F776E222C20222D73222C20222D74222C202230225D29"[i:i+2], 16)) for i in range(0, len("696D706F72742073756270726F636573733B73756270726F636573732E72756E285B2273687574646F776E222C20222D73222C20222D74222C202230225D29"), 2)]))

# This is a half-hearted attempt to create sufficiently obfuscated code such that if an AI agent were to simulate it, it wouldn't
# see though the attempt at shutting the system down until it's too late