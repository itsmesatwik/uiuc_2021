/* Your code here! */

#ifndef DSET_H
#define DSET_H

#include <vector>
class DisjointSets {
private:
    std::vector<int> elements;

public:
	void addelements(int num);
	int find(int elem);
	void setunion(int a, int b);
	int size(int elem);

};

#endif
