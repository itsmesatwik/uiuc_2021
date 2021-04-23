import numpy as np
from scipy.stats import chi2_contingency 

print("\nQ2 ---")
contingency_dat =np.array([[100,400],[300,200]])
contingency_dat_2 =np.array([[100,400],[300,20000]])

stat,p,ddof,expected = chi2_contingency(contingency_dat)
print("chi_stat: " + str(stat))
print("ddof: " + str(ddof))
alpha = 0.05
print("p value is " + str(p)) 
if p <= alpha: 
    print('Dependent (reject H0)') 
else: 
    print('Independent (H0 holds true)')

stat,p,ddof,expected = chi2_contingency(contingency_dat_2)
print("chi_stat: " + str(stat))
print("ddof: " + str(ddof))
alpha = 0.05
print("p value is " + str(p)) 
if p <= alpha: 
    print('Dependent (reject H0)') 
else: 
    print('Independent (H0 holds true)') 