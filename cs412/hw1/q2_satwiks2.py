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

variance = np.var(table[:,1], ddof=0)

norm_table = stats.zscore(table[:,1:], axis=0)
norm_variance = np.var(norm_table[:,1], ddof=0)
norm = (90 - np.mean(table[:, 1])) / np.sqrt(np.var(table[:,1], ddof=0))
pearsons = stats.pearsonr(table[:,1], table[:,2])[0]
pearsons_n = stats.pearsonr(norm_table[:,0], table[:,2])[0]
covar = np.cov(table[:,1], table[:,2], ddof=0)

print("problem 2a.\n", "norm var: ", norm_variance, "variance: ",variance)
print("problem 2b.\n", "norm: ",norm)
print("problem 2c.\n", "pearsons: ",pearsons)
print("problem 2d.\n", "pearsons_n: ",pearsons_n)
print("problem 2e.\n", "covar: ",covar)



