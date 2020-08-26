#pragma once

#include <vector>

class UnionFindTree
{
public:
	std::vector<int> vec;

public:
	UnionFindTree(){}

	UnionFindTree(int size)
	{
		Initialize(size);
	}

	void Initialize(int size)
	{
		if(size <= 0) return;
		vec.clear();		
		vec.reserve(size);
		for(int i = 0; i < size; i++)
		{
			vec.push_back(i);
		}
	}

	void Reset()
	{
		for(int i = 0; i < vec.size(); i++)
		{
			vec[i] = i;
		}
	}

	int Find(int index)
	{
		if(index < 0 || index >= vec.size()) return -1;
		while(index != vec[index])
		{
			index = vec[index];
		}
		return index;
	}

	bool Union(int x, int y)
	{
		int x_root = Find(x);
		int y_root = Find(y);
		if(x == -1 || y == -1) return false;
		bool result = x_root != y_root;
		if(result) vec[y_root] = x_root;
		return result;
	}
};