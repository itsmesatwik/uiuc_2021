/* Your code here! */

#include "dsets.h"

void DisjointSets::addelements(int num) {
	for (int i = 0; i < num; i++)
		elements.push_back(-1);
}

int DisjointSets::find(int elem) {
	if (elements[elem] < 0)
		return elem;
	else
		return elements[elem] = find(elements[elem]);
}

void DisjointSets::setunion(int a, int b) {
	int root_A = find(a);
	int root_B = find(b);
	if (root_A == root_B)
		return;

	int newsize = elements[root_A] + elements[root_B];
	if (elements[root_A] <= elements[root_B]) {
		elements[root_B] = root_A;
		elements[root_A] = newsize;
	}
	else {
		elements[root_A] = root_B;
		elements[root_B] = newsize;
	}

}

int DisjointSets::size(int elem) {
	 if (elements[elem] < 0)
         return -1*elements[elem];
	return size(find(elem));
}
