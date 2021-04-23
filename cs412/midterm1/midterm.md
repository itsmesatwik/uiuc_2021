
# Midterm

## Satwik Singh satwiks2

### Question 1

1. It’s a set of descriptive statistics of a dataset, which includes minimum, first quartile(25%), median, third quartile(75%) and maximum.
2. Boxplots are box-like diagrams that measures the dispersion of the dataset. A box plot is constructed from five values: the minimum value, the first quartile, the median, the third quartile, and the maximum value. We use these values to compare how close other data values are to them. The ends of the box are at the first and third quartiles, with median marked with a line inside the box. The height of the box represents the inter-quartile range. Two extensions from first and third quartile separately to connect the minimum and maximum, with points outside of the outlier filter plotted individually.
3. If two distribution have the same five-number summaries and outliers, then the boxplots can be the same. Boxplot only measures the dispersion of a dataset not the actual distribution and two different distributions can have the same dispersion.
4. A quantile plot displays all the data and plots the quantile information. The quantile plot permits identification of any peculiarities of the shape of the sample distribution, which might be symmetrical or skewed to higher or lower values.
5. A q-q plot is a plot of the quantiles of the first data set against the quantiles of the second data set. It is a graphical technique for determining if two data sets come from populations with a common distribution.
6. A quantile plot only shows the distribution of one dataset but a quantile-quantile plot compares the distribution and plots two different dataset.

### Question 2

```python
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
```

1. $E_{buybeer,buydiaper} = \frac{count(BuyBeer)*count(BuyDiaper)}{total} = \frac{500*400}{1000} = 200$
2. $E_{buybeer,nobuydiaper} = \frac{count(BuyBeer)*count(NotBuyDiaper)}{total} = \frac{500*600}{1000} = 300$
3. Chi sq statistic = 165.004
   1. Create expected values table using formula:
   expected val = (row total * column total) / grand total
   1. Then use the expected value table to calculate chi squared statistic using:
   $\chi ^2 = \sum\frac{(observed - expected)^2}{expected}$
4. Using code the p value is close to 0 so we reject the null hypothesis at significance level 0.05.
   1. If we want to do it by hand we get df = 1 and alpha = 0.05
   2. using lookup table we get critical value = 3.841
   3. since critical value < calculated chi square value we reject the null hypothesis hence we conclude that they are dependent.
5. Chi sq statistic = 877.818. Using the same process as above.
6. Using the same proces we still reject the null hypothesis since our critical value is still less than the calculated chi square statistic hence we conclude that they are dependent.

### Question 3

a. Frequent patterns

1. Min sup = 0.6.
The most frequent k-itemset with largest k = the 3-itemset {B,C,D} with rel_sup = 3/5 = 0.6.
   1. {A}: 4/5, {B}: 3/5, {C}: 4/5, {D}: 4/5
   2. {A,C}: 3/5, {A,D}: 3/5
   3. {B,C,D}: 3/5.
   Therefore, the frequent k-itemset for max k=3 is {B,C,D}.
1. Strong Association rules:
   min sup = 0.6, the only set of 3-itemset that passes is {B,C,D}. Therefore, all associations between these 3 items:
    1. For $buys(x, B) ∧ buys(x, C) ⇒ buys(x, D)$
       - the support is 3/5
       - the confidence is $\frac{sup(B, C, D)}{sup(B, C)} = 0.6/0.6 = 1$
       - the confidence > minconf.
       $buys(x,B)∧buys(x,C)⇒buys(x,D)$. [60%,100%]  
<br>
    2. For $buys(x, C) ∧ buys(x, D) ⇒ buys(x, B)$
       - the support is 3/5
       - the confidence is $\frac{sup(B, C, D)}{sup(C, D)} = 0.6/0.6 = 1$
       - the confidence > minconf.
       $buys(x, C) ∧ buys(x, D) ⇒ buys(x, B)$. [60%,100%]  
<br>
    3. For $buys(x, D) ∧ buys(x, B) ⇒ buys(x, C))$
        - the support is 3/5
        - the confidence is $\frac{sup(B, C, D)}{sup(D, B)} = 0.6/0.6 = 1$
        - the confidence > minconf.
        $buys(x, D) ∧ buys(x, B) ⇒ buys(x, C)$. [60%,100%]  


b. Constraint types

1. The average price of all the items in each pattern is greater than \$50.
   - $c_1: avg(S.price) > \$50$
   - This is a convertible constraint. It can be mined using FP-growth.
   - All the frequent items are listed in price descending order
   - A pattern as well as its conditional pattern base can be pruned, if the average price of items in the pattern is less than \$50 and we've reached items priced less than \$50 so the consequent items can't bring the average back above.
2. The sum of the price of all the items with profit over \$5 in each patternis at least \$200.
   - $c_1: profit(S.price) > \$5, c_2: sum(S.price) > \$200$
   - Constraint c1 is succinct and c2 is monotonic and data antimonotonic. It can be mined using FP-growth.
   - Put all items with profits more than $5 at the end of the list of frequent items so that they are pushed first in mining and we can easily prune.
   - Derive new FP-trees for frequent items with profit > $5 and mine recursively .
   - Once a pattern has sum at least $200, we can stop the recursion as it will only go up.
   - Patterns and conditional patterns can be pruned if sum of frequent items in the pattern is < $200 (data antimonotonocity).

### Question 4

![Screen Shot 2021-03-23 at 12.17.31 AM](/assets/Screen%20Shot%202021-03-23%20at%2012.17.31%20AM.png)
Minsup = 3
1. ⟨a⟩ prefix, projected database:
   1. $⟨a⟩ : 〈(abc)(ac)d(cf)〉,〈(\_d)c(bc)(ae)〉,〈(\_b)(df)cb〉,〈(\_f)cbc〉$
   Here the support comes out to be $\{⟨a⟩ : 2, ⟨b⟩ : 4, ⟨\_b⟩ : 2, ⟨c⟩ : 4,⟨d⟩ : 2,⟨f⟩ : 2\}$ so we only take b and c forward in the pattern.
   2. $⟨ab⟩ : ⟨(\_c)(ac)d(cf)⟩, ⟨(\_c)(ae)⟩, ⟨c⟩$
   Here the support comes out to be $\{⟨a⟩ : 2,⟨\_c⟩ : 2, ⟨c⟩ : 3 ,⟨d⟩ : 1,⟨f⟩ : 1\}$ so we only take c forward in the pattern.
   3. $⟨ac⟩ : ⟨(ac)d(cf)⟩, ⟨(bc)(ae)⟩, ⟨bc⟩$
   Here only c has support more than 3.
   So the patterns are: $⟨a⟩, ⟨ab⟩, ⟨ac⟩, ⟨abc⟩, ⟨acc⟩$
<br>

1. ⟨c⟩ prefix, projected database:
   1. $⟨c⟩ : ⟨(ac)d(cf)⟩, ⟨(bc)(ae)⟩, ⟨b⟩, ⟨bc⟩$
   Here the support comes out to be $\{⟨a⟩ : 2, ⟨b⟩ : 3, ⟨c⟩ : 3,⟨d⟩ : 1,⟨e⟩ : 1,⟨f⟩ : 1\}$ so we only take b and c forward.
   1. $⟨cb⟩ : ⟨(\_c)(ae)⟩, ⟨c⟩$
   Here the support comes out to be $\{⟨a⟩ : 1, ⟨c⟩ : 2,⟨\_c⟩ : 1,⟨e⟩ : 1\}$ so no more generation.
   1. $⟨cc⟩ : ⟨d(cf)⟩, ⟨(ae)⟩$
   Here the support comes out to be $\{⟨a⟩ : 1, ⟨c⟩ : 1,⟨d⟩ : 1,⟨e⟩ : 1,⟨f⟩ : 1\}$ so no more generation.
   So the patterns are: $⟨c⟩, ⟨cb⟩, ⟨cc⟩$
<br>

1. ⟨d⟩ prefix, projected database:
   1. $⟨d⟩ : ⟨(cf)⟩, ⟨c(bc)(ae)⟩, ⟨( f)cb⟩$, only c has support of 3
   2. $⟨dc⟩ : ⟨(bc)(ae)⟩, ⟨b⟩$ no more patterns.
   So the patterns are: $⟨d⟩, ⟨dc⟩$
<br>

2. ⟨b⟩ prefix, projected database:
   1. $⟨b⟩ : ⟨(\_c)(ac)d(cf)⟩, ⟨(\_c)(ae)⟩, ⟨(df)cb⟩, ⟨c⟩$ only c has support of 3
   2. $⟨bc⟩ : ⟨d(cf)⟩, ⟨b⟩$ no more patterns.
   So the patterns are: $⟨b⟩, ⟨bc⟩$
<br>

3. ⟨e⟩ prefix, projected database:
   1. $⟨e⟩ : ⟨(\_f)(ab)(df)cb⟩, ⟨g(af)cbc⟩$ no more patterns.
   So the patterns are: $⟨e⟩$
<br>

4. ⟨f⟩ prefix, projected database:
   1. $⟨f⟩ : ⟨(ab)(df)cb⟩, ⟨cbc⟩$ no more patterns.
   So the patterns are: $⟨f⟩$
<br>

5. No, since every prefix has been accounted for except g which didn't pass the minsup requirement in the first pass itself.