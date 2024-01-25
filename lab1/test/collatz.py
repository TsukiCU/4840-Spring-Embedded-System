def collatz(n, memo):
    if n == 1:
        return 0

    if n not in memo:
        if n % 2 == 0:
            memo[n] = 1 + collatz(n // 2, memo)
        else:
            memo[n] = 1 + collatz(3 * n + 1, memo)

    return memo[n]

if __name__ == '__main__':
    memo = {}
    sequence = [7, 22, 11, 34, 17, 52, 26, 13, 40, 20, 10, 5, 16, 8, 4, 2, 1]
    res = []
    for num in sequence:
        res.append(collatz(num, memo))
        memo={}
    print(res)
