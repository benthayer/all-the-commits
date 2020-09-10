import time

t = time.time()
i = 0
while time.time() < t + 0.0001:
    i += 1
print(i)
