import numpy as np
from scipy.stats import chi2_contingency 

print("\nQ4 ---")
contingency_dat =np.array([[150,40],[15,3300]])
distance = (15+40)/(np.sum(contingency_dat))
print("sym_dist: " + str(distance))
jaccard = 150/(150+15+40)
print("jaccard: " + str(jaccard))

stat,p,ddof,expected = chi2_contingency(contingency_dat)
print("chi_stat: " + str(stat))
print("ddof: " + str(ddof))
alpha = 0.05
print("p value is " + str(p)) 
if p <= alpha: 
    print('Dependent (reject H0)') 
else: 
    print('Independent (H0 holds true)') 