def smooth_number_3(n):
    res = [0 for i in range(n)]
    res[0] = 1
    p2, p3 = 0, 0

    for i in range(1, n):
        if res[p2] * 2 == res[i - 1]:
            p2 += 1
        if res[p3] * 3 == res[i - 1]:
            p3 += 1
        u2 = res[p2] * 2
        u3 = res[p3] * 3
        if u2 < u3:
            p2 += 1
            res[i] = u2
        else:
            p3 += 1
            res[i] = u3
    return res

def seq2(n):
    size = 0
    while n >= 1:
        n /= 1.3
        n = str(n)
        n = n.split('.')
        n = int(n[0])
        if n is 9 or n is 10:
            n = 11
        size += 1
    return size

#print(smooth_number_3(20))
print(seq2(70))
print(seq2(80))
print(seq2(90))
print(seq2(100))
print(seq2(150))
