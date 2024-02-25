from math import log

def H(n):
    """Returns an approximate value of n-th harmonic number.

       http://en.wikipedia.org/wiki/Harmonic_number
    """
    # Euler-Mascheroni constant
    gamma = 0.57721566490153286060651209008240243104215933593992
    return gamma + log(n) + 0.5/n - 1./(12*n**2) + 1./(120*n**4)

def tries_so_far(num_commits, n=16**7):
    k = num_commits
    return n * (H(n) - H(n-k-1))

def total_tries(n=16**7):
    return n * H(n)

def progress(num_commits, n=16**7):
    return tries_so_far(num_commits, n) / total_tries(n)

def total_tries1():
    # 16**7 = 268,435,456
    tries = 0
    for i in range(16**7):
        tries += 16**7/(16**7-i)

    print(tries)
    print(f'Total of {tries} attempts is {tries/16**7}x larger than {16**7}')
    # Total of 5364772976.401805 attempts is 19.98533672244029x larger than 268435456

def up_to_x_unique(x, n=16**7):
    return factorial(n) / (n**x * factorial(n-x))

def x_is_first_unique(x, n=16**7):
    return up_to_x_unique(x, n) * (x-1) / n

def expected_first():
    # Assuming there are 100 labels...
    # Probability that there are none at that point is the multiplier
    # P(no repeats yet | x) = 1 * 99/100 * 98/100 * ... * (100-(x-1))/100
    # P(x is first repeat | x) = P(no repeats yet | x-1) * (x-1)/100

    # E[no repeats] = SUM[P(x is the first repeat | x) * x

    stop_percent = 0.9
    no_repeats_prob = 1
    expected_value = 0
    total_prob = 0
    for i in range(16**7):
        first_repeat_prob = no_repeats_prob * i/16**7
        expected_value += first_repeat_prob * (i + 1)
        no_repeats_prob *= (16**7 - i)/16**7
        if total_prob > stop_percent:
            break

    expected_value /= stop_percent

    print(expected_value)  # 20534.965498815607 before 

if __name__ == '__main__':
    print(total_tries(16**7)/ 16**7)
