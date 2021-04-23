import numpy as np
from scipy import stats

data_path = '/Users/satwik/classes/cs412/hw1/data.online.scores.txt'
table = np.full((1000,3), np.inf)

with open(data_path, 'r') as dat:
    i = 0
    for row in dat:
        cols = row.split()
        table[i,] = cols
        i+=1

_, max_midterm, max_final = np.amax(table, axis=0)
_, min_mid, min_final = np.min(table, axis=0)

print("problem 1a.\n", 'max_mid:', max_midterm, 'min_mid:', min_mid)

quantiles = np.quantile(table[:, 1], [0.25, 0.5, 0.75])
means = np.mean(table[:, 1])
modes = stats.mode(table[:,1])
variance = np.var(table[:,1], ddof=0)

print("problem 1b.\n", "quantiles: ", quantiles)
print("problem 1c.\n", "means: ",means)
print("problem 1d.\n", "modes: ",modes)
print("problem 1e.\n", "variance: ",variance)