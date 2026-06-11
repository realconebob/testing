#!/usr/bin/env -S python3

def fib(n: int) -> int:
    memo = {}

    for i in range(1, n + 1):
        if i <= 2:
            result = 1
        else:
            result = memo[i - 1] + memo[i - 2]

        memo[i] = result
    
    return memo[n]

if __name__ == "__main__":
    test: int = 20468
    print(f"fib({test}) = {fib(test)}")
    
    pass