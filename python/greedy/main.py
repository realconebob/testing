#!/usr/bin/env -S python3

# Finds locally optimal solutions, may not find the globally optimal solution (may find a solution, but one with too many point increments)
def greedy_point_solver(pointvals: list[int, ...], target: int) -> tuple[dict[int, int], int, int] | None:
    """
    Attempts to find the minimum number of point increments to reach `target`

    Returns a tuple containing:
        - used (dict[int, int]):    point denomination -> increments used
        - total_coins (int):        total number of increments
        - remainder (int):          leftover amount if no perfect solution exists (0 if exact)

    Returns None on invalid input
    """
    if target < 1 or not pointvals:
        return None

    pointcpy: list[int, ...] = pointvals[:]
    count: dict[int, int] = {}
    total: int = 0

    pointcpy.sort(reverse=True)
    for i in range(0, len(pointcpy)):
        if target >= pointcpy[i]:
            tmp = target // pointcpy[i]
            total += tmp
            target -= tmp * pointcpy[i]
            count[pointcpy[i]] = tmp

        if target == 0:
            break

    return (count, total, target) # Point increment counts, total increments used, remainder if any


# Finds globally optimal solution
def dynamic_point_solver(pointvals: list[int, ...], target: int) -> tuple[dict[int, int], int, int] | None:
    """
    Find the minimum number of points increments to make `target`.

    Returns a tuple of:
        - used (dict[int, int]):  point denomination -> increments used
        - total_coins (int):      total number of increments
        - remainder (int):        leftover amount if no perfect solution exists (0 if exact)

    Returns None on invalid input
    """
    if target < 1 or not pointvals:
        return None

    pointcpy = sorted(set(v for v in pointvals if v > 0), reverse=True)

    dp = [float('inf')] * (target + 1)
    parent = [-1] * (target + 1)
    dp[0] = 0

    for i in range(1, target + 1):
        for pointval in pointcpy:
            if pointval <= i and dp[i - pointval] + 1 < dp[i]:
                dp[i] = dp[i - pointval] + 1
                parent[i] = pointval

    # Find the best reachable amount (exact or closest without going over)
    best = next((i for i in range(target, -1, -1) if dp[i] != float('inf')), 0)
    remainder = target - best

    # Trace back which pointvals were used
    used: dict[int, int] = {}
    current = best
    while current > 0:
        pointval = parent[current]
        used[pointval] = used.get(pointval, 0) + 1
        current -= pointval

    return (used, sum(used.values()), remainder)

def optimize_points(pointvals: list[int, ...], target: int) -> tuple[dict[int, int], int, int]:
    res = greedy_point_solver(pointvals, target)
    if res[2] != 0:
        res2 = dynamic_point_solver(pointvals, target)
        if res2[1] > res[1]: res = res2

    return res

if __name__ == "__main__":
    points: list[int, ...] = [1, 2, 5, 10]

    print(f"greedy - points: {points}, res: {greedy_point_solver(points, 39)}")
    print(f"greedy - points: {points[1:]}, res: {greedy_point_solver(points[1:], 41)}")
    print()

    print(f"dynamic - points: {points}, res: {dynamic_point_solver(points, 39)}")
    print(f"dynamic - points: {points[1:]}, res: {dynamic_point_solver(points[1:], 41)}")
    print()

    print(f"optimized - points: {points}, res: {optimize_points(points, 39)}")
    print(f"optimized - points: {points[1:]}, res: {optimize_points(points[1:], 41)}")

exit(0)
