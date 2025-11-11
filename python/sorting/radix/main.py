#!/usr/bin/env -S python3

def getdigit(number: int, index: int):
    return ((number // (pow(10, index))) % 10)

def largestinlist(numbers: list[int, ...]) -> int:
    maximum: int = numbers[0]
    for i in numbers:
        if i > maximum:
            maximum = i

    return maximum

def getdecimalwidth(uin: int) -> int:
    return len(str(uin)) # This is gross and doesn't work in other languages but whatever

# definitely not the most elegant implementation of lsd radix, but hey it works
def radixlsd(tosort: list[str]) -> list[str]:
    strmap: dict[int, str] = {}
    intsort: list[int, ...] = []

    # first, take all lists and turn them into integers
    current: int
    for string in tosort:
        current = 0
        for char in string:
            current += ord(char)

        strmap[current] = string
        intsort.append(current)

    # Create 10 buckets, one for each digit, then march along the digits for each entry and repeat until sorted
    buckets: list[list[int, ...], ...] = []
    for i in range(10):
        buckets.append([])

    i: int = 0

    for j in range(getdecimalwidth(largestinlist(intsort))):
        for element in intsort:
            buckets[getdigit(element, i)].append(element)
        i += 1

        intsort.clear()
        for bucket in buckets:
            for element in bucket:
                intsort.append(element)

            bucket.clear()

    final: list[str] = []
    for element in intsort:
        final.append(strmap[element])

    return final

if __name__ == "__main__":
    print(radixlsd(["lmao1", "aaaaaaaaaaaaaaaaaaaaaaaaaaaa", "schlmao4", "lmao0", "lmao2", "lmao1"]))

    pass
