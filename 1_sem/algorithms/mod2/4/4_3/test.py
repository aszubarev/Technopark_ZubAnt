import random

size = 1000
k = random.randint(0, size)

test = [i for i in range(0, size)]

size_swap = 500

print(test)
for i in range(0, size_swap):
    s = random.randint(0, size - 2)
    test[s], test[s + 1] = test[s + 1], test[s]

print(size, k)
for val in test:
    print(val, end=" ")
