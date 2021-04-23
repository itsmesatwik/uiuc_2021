#CS 412 Homework 4 Submission Stub
#Satwik Singh


import numpy as np
from sklearn.datasets import load_digits
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC, LinearSVC
from sklearn.utils import shuffle
from xgboost import XGBClassifier


def get_splits(n, k):
    arr = list(range(n))
    np.random.shuffle(arr)
    return np.array_split(arr, k)


def my_cross_val(method, X, y, k):
    svc = SVC(gamma='scale',C=10)
    linsvc = LinearSVC(max_iter=2000)
    lr = LogisticRegression(penalty='l2',solver='lbfgs',multi_class='multinomial',max_iter=5000)
    rfc = RandomForestClassifier(max_depth=20,random_state=0,n_estimators=500)
    xgbc = XGBClassifier()
    classifier = None
    if method == 'LinearSVC':
        classifier = linsvc
    elif method == 'SVC':
        classifier = svc
    elif method == 'LogisticRegression':
        classifier = lr
    elif method == 'RandomForestClassifier':
        classifier = rfc
    elif method == 'XGBClassifier':
        classifier = xgbc
    indices = get_splits(len(y),k)
    error_rates = []
    for test in indices:
        xtest = [x for i, x in enumerate(X) if i in test]
        ytest = [x for i, x in enumerate(y) if i in test]
        xtrain = [x for i, x in enumerate(X) if i not in test]
        ytrain = [x for i, x in enumerate(y) if i not in test]
        classifier.fit(np.array(xtrain), np.array(ytrain))
        expected_y = classifier.predict(np.array(xtest))
        error = np.sum(expected_y != ytest) / len(ytest)
        error_rates.append(error)
    return error_rates

def my_train_test(method, X, y, pi, k):
    svc = SVC(gamma='scale',C=10)
    linsvc = LinearSVC(max_iter=2000)
    lr = LogisticRegression(penalty='l2',solver='lbfgs',multi_class='multinomial',max_iter=5000)
    rfc = RandomForestClassifier(max_depth=20,random_state=0,n_estimators=500)
    xgbc = XGBClassifier()
    classifier = None
    if method == 'LinearSVC':
        classifier = linsvc
    elif method == 'SVC':
        classifier = svc
    elif method == 'LogisticRegression':
        classifier = lr
    elif method == 'RandomForestClassifier':
        classifier = rfc
    elif method == 'XGBClassifier':
        classifier = xgbc
    error_rates = []
    for i in range(k):
        X, y = shuffle(X, y, random_state=0)
        end_index = int(len(y) * pi)
        xtest = X[end_index:,]
        ytest = y[end_index:,]
        xtrain = X[0:end_index,]
        ytrain = y[0:end_index,]
        classifier.fit(np.array(xtrain), np.array(ytrain))
        expected_y = classifier.predict(xtest)
        error = np.sum(expected_y != ytest) / len(ytest)
        error_rates.append(error)
    return error_rates


# dat = load_digits()
# X = dat.data
# y = dat.target

# a = my_train_test('XGBClassifier', X, y, 0.75, 10)
# # a = my_cross_val('XGBClassifier', X,y,200)
# print(a)

# b = get_splits(45,10)
# print(b)
