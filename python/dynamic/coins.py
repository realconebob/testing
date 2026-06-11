#!/usr/bin/env -S python3

''' The Coin Problem
    Given a set of coins [c1, c2, c3, ..., cK] and a target sum of money M, what is the minimum number of
    coins necessary to form the sum M?
'''

def min_ignore_none(a: int, b: int) -> int:
    if a is None:
        return b
    if b is None:
        return a
    return min(a, b)

def min_coins(coins: list[int, ...], m: int) -> int:
    answer: int = 0
    if m != 0:
        answer = None
        for coin in coins:
            subproblem: int = m - coin
            if subproblem < 0:
                continue # skip solutions where too much money has been given

            answer = min_ignore_none(answer, min_coins(coins, subproblem) + 1)

    return answer


def mmin_coins(coins: list[int, ...], m: int, memo: dict[int, int] = {}) -> int:
    if m in memo:
        return memo[m]

    answer: int = 0
    if m != 0:
        answer = None
        for coin in coins:
            subproblem: int = m - coin
            if subproblem < 0:
                continue # skip solutions where too much money has been given

            answer = min_ignore_none(answer, mmin_coins(coins, subproblem, memo) + 1)

    memo[m] = answer
    return answer

def bmin_coins(coins: list[int, ...], m: int) -> int:
    memo: dict[int, int] = {0: 0}
    for i in range(1, m + 1):
        for coin in coins:
            subproblem: int = i - coin
            if subproblem < 0:
                continue

            memo[i] = min_ignore_none(memo.get(i), memo.get(subproblem) + 1)

    return memo[m]

if __name__ == "__main__":
    coins: list[int, ...] = [1, 2, 3, 5, 7, 11]
    smallsum: int = 13
    largesum: int = 15000
    
    print(f"Available Coins: {coins}")
    print(f"min_coins({smallsum}) = {bmin_coins(coins, smallsum)}")
    print(f"min_coins({largesum}) = {bmin_coins(coins, largesum)}")

    pass