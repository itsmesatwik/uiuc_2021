class Node(object):
	def __init__(self, data = None, next_node = None):
		self.data = data
		self.next = next_node
class Stack(object):
	def __init__(self, top = None):
		self.top = top
	def push(self, data):
		if self.top == None:
			self.top = Node(data)
			return
		temp = Node(data)
		temp.next = self.top
		self.top = temp
	