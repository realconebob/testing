#!/usr/bin/env -S python3

def constant(f):
	def fget(self): return f(self)
	def fset(self, value) -> None: return
	return property(fget, fset)

class TestClass:
	@constant
	def PROP1(_self) -> str: return "this is a string"

	@constant
	def PROP2(self) -> str: return self.PROP1[::-1]

	@constant
	def PROP3(self) -> dict[str, int]: return {char: idx for idx, char in enumerate(self.PROP1, start=0)}

if __name__ == "__main__":
	test: TestClass = TestClass()
	print(test.PROP1)
	print(test.PROP2)
	print(test.PROP3)
	exit(0)
