import numpy as np
from scipy import stats
from scipy.spatial import minkowski_distance
from scipy.special import kl_div

path = '/Users/satwik/classes/cs412/hw1/data.libraries.inventories.txt'
table = np.full((2,100), np.inf)

with open(path, 'r') as dat:
    i = 0
    for row in dat:
        if i == 0:
            i+=1
            continue
        cols = row.split()
        table[i-1,] = cols[1:]
        i+=1

h1 = minkowski_distance(table[0,], table[1,], 1)
h2 = minkowski_distance(table[0,], table[1,], 2)
h_inf = minkowski_distance(table[0,], table[1,], np.inf)


cosine_sim = np.dot(table[0,], table[1,]) / (np.linalg.norm(table[0,]) * np.linalg.norm(table[1,]))
# print(table)
print("Q3 ---")
print("h1: " + str(h1))
print("h2: " + str(h2))
print("h_inf: " + str(h_inf))
print("cosine_sim: " + str(cosine_sim))

prob_distribution = table / table.sum(axis=1).reshape(2,1)
KL = np.sum(kl_div(prob_distribution[0,], prob_distribution[1,]))
print("KL: " + str(KL))