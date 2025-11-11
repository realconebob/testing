#!/usr/bin/env -S python3

def read_etc_passwd() -> str:
	res: string = ""
	with open('/etc/passwd', encoding="utf-8") as etc:
		res = etc.read()

	return res

def condensed():
	with open("/etc/passwd") as etc: print(etc.read())
	pass

if __name__ == "__main__":
	condensed()
	print(f'User defined input: {input()}')
	pass
