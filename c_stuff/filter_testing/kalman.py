import math

def guassian(x, mean, variance):
    bottom = (1 / math.sqrt(2 * math.pi * variance))
    top = pow(math.e, (-0.5 * pow(x - mean, 2) / variance))
    return top / bottom

for i in range(80):
    print(guassian(i, 20, 10))
