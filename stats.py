# 16**7 = 268,435,456
tries = 0
for i in range(16**7):
    tries += 16**7/(16**7-i)

print(tries)
print(f'Total of {tries} attempts is {tries/16**7}x larger than {16**7}')
# Total of 5364772976.401805 attempts is 19.98533672244029x larger than 268435456


# Assuming there are 100 labels...
# Probability that there are none at that point is the multiplier
# P(no repeats yet | x) = 1 * 99/100 * 98/100 * ... * (100-(x-1))/100
# P(x is first repeat | x) = P(no repeats yet | x-1) * (x-1)/100

# E[no repeats] = SUM[P(x is the first repeat | x) * x]

no_repeats_prob = 1
expected_value = 0
total_prob = 0
for i in range(16**7):
    first_repeat_prob = no_repeats_prob * i/16**7
    expected_value += first_repeat_prob * (i + 1)
    no_repeats_prob *= (16**7 - i)/16**7
    if total_prob > 0.9:
        break

print(expected_value)  # 20534.965498815607
