class Node:
    def __init__(self, letter):
        self.left = None
        self.right = None
        self.val = letter

class Tree:
    def __init__(self, root):
        self.root = root
        self.index = 0

    def build_tree(self, in_seq, pre_seq, start_in, end_in):
        if start_in > end_in:
            return None
        node = Node(pre_seq[self.index])
        self.index+=1
        if start_in == end_in:
            return node
        start_in = in_seq.index(node.val)
        node.left = self.build_tree(in_seq[0:start_in])
        

