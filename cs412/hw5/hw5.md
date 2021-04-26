# Homework 5

## Satwik Singh satwiks2

### Question 1
1. 
   - Gain Ratio: Gain ratio is a metric that attempts to lessen the bias of information gained on highly branched predictors by introducing a normalization on the information gain.
   $GainRatio(A) = \frac{Gain(A)}{SplitInfo(A)}$.

   - Gini Index: Gini index is calculated by subtracting the sum of the squared relative frequency of each class from one.  It calculates the probability of a certain randomly selected feature being classified incorrectly.
   $gini(D)=1-\sum^{n}_{j=1}p_j^2$
   $p_j$ is the relative frequency of class j in D.
<br>

2. 

- Patrons
  1. gini(None) = 1-1 = 0
  2. gini(Some) = 1-1 = 0
  3. gini(Full) = 1-$((2/6)^2+(4/6)^2)$ = 0.444
overall gini = $0 + 0 + \frac{6}{12}*0.444$ = 0.222
<br> 

- Type
  1. gini(French) = 1 - (0.25+0.25) = 0.5
  2. gini(Italian) = 1 - (0.25+0.25) = 0.5
  3. gini(Thai) = 1 - (0.25+0.25) = 0.5
  4. gini(Burger) = 1 - (0.25+0.25) = 0.5
overall gini = $(\frac{2}{12}+\frac{2}{12}+\frac{4}{12}+\frac{4}{12})*0.5$ = 0.5

Since the overall gini impurity is lower for Patrons then the probability of a feature getting classified wrong is lower there hence we will split on Patrons.

3. $Info(D) = -\sum_{i=1}^mp_ilog_2(p_i)$ <br> $Info_A(D) = \sum_{j=1}^v\frac{|D_j|}{|D|}*Info(D_j)$ <br> $Gain(A) = Info(D) - Info_A(D)$

- Patrons
  1. $Info(Patrons)=-(0.5log_2(0.5) + 0.5log_2(0.5))=1$
  2. $Info(Patrons|None)=-(0log_2(0) + log_2(1))=0$
  3. $Info(Patrons|Some)=-(log_2(1) + 0log_2(0))=0$
  4. $Info(Patrons|Full)=-(0.33log_2(0.33) + 0.67log_2(0.67))=0.918$
  5. $Gain(Patrons)=1-0.5(0.9183)=0.541$
  6. $SplitInfo_A(Patrons)=-(2/12*log_2(2/12)+4/12*log_2(4/12)+6/12*log_2(6/12))=1.459$
  7. $GainRatio(Patrons)=0.371$
- Type
  1. $Info(Type)=-(0.5log_2(0.5) + 0.5log_2(0.5))=1$
  2. $Info(Type|French)=-(0.5log_2(0.5) + 0.5log_2(0.5))=1$
  3. $Info(Type|Italian)=-(0.5log_2(0.5) + 0.5log_2(0.5))=1$
  4. $Info(Type|Thai)=-(0.5log_2(0.5) + 0.5log_2(0.5))=1$
  5. $Info(Type|Burger)=-(0.5log_2(0.5) + 0.5log_2(0.5))=1$
  6. $Gain(Type)=1-1(2/12+2/12+4/12+4/12)=0$
  7. $SplitInfo_A(Type)=-(2/12*log_2(2/12)+2/12*log_2(2/12)+4/12*log_2(4/12)+4/12*log_2(4/12))=1.918$
  8. $GainRatio(Patrons)=0$
   
Since GainRatio of patrons is higher, it's better to split on patrons since we get better classification from it.


### Question 2

$P(A|B) = \frac{P(B|A)*P(A)}{P(B)}$
<br>

1. Candy 1
   1. $P(h_1|lime)=\frac{P(lime|h_1)*P(h_1)}{P(lime)}=0$
   2. $P(h_2|lime)=\frac{P(lime|h_2)*P(h_2)}{P(lime)}=\frac{0.5*0.5}{0.5}=0.5$
   3. $P(h_3|lime)=\frac{P(lime|h_3)*P(h_3)}{P(lime)}=\frac{1*0.25}{0.5}=0.5$
2. Candy 2
   1. $P(h_1|lime \& cherry))=\frac{P(lime \& cherry|h_1)*P(h_1)}{P(lime \& cherry)}=0$
   2. $P(h_2|lime \& cherry))=\frac{P(lime \& cherry|h_2)*P(h_2)}{P(lime \& cherry)}=\frac{(0.5*0.5)*(0.5)}{0.5*0.5*0.5}=1$
   3. $P(h_3|lime \& cherry))=\frac{P(lime \& cherry|h_3)*P(h_3)}{P(lime \& cherry)}=0$

### Question 3


### Question 4

1. Random forest is a technique used in modeling predictions built on decision trees. It is a combination of many decision trees that represent a distinct instance of the classification of data input into the random forest. Each tree in the classifications takes input from samples in the initial dataset. Features are then randomly selected, which are used in growing the tree at each node. Every tree in the forest should not be pruned until the end of the exercise, when the prediction is reached decisively. In such a way, the random forest enables any classifiers with weak correlations to create a strong classifier. The tree depth d is the number of features used to grow the tree until decisive predictions.