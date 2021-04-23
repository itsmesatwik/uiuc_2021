from collections import Counter
def get_sequences(file, minsup):
    #Load file:
    data = []
    with open(file, 'r') as outfile:
        for line in outfile:
            one = []
            for x in line.split(',')[-1]:
                if x.isalpha():
                    one.append(x)
            data.append(one)
# used counter, got idea from stackoverflow https://stackoverflow.com/questions/1155617/count-the-number-occurrences-of-a-character-in-a-string?rq=1
    print(data)
    total_count = Counter()
    for s in data:
        s = ''.join(s)
        transaction = [s[i:j+1] for i in range(len(s)) for j in range(i,len(s))]
        tran_count = Counter(transaction)
        #convert counts to 1 per trasaction
        tran_count = Counter({x:1 for x in tran_count})
        total_count += tran_count
    ret = {}
    for key, value in total_count.items():
        if value >= minsup:
            ret[key] = value
    return ret  






# path = "./input.txt"
# a = get_sequences(path, 2)
# print(a)
