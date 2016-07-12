#ifndef __HEAP_H__
#define __HEAP_H__

#include<vector>
#include<assert.h>

template<class T>
class Less
{
	bool operator()(const T&left, const T&right)
	{
		return left<right;
	}
};
class Greater
{
	bool operator()(const T&left, const T&right)
	{
		return left>right;
	}
};
template<class T, class Compare = Less<T>>
class Heap
{
public:
	Heap()
	{}
	Heap(const T&a, int size)
	{
		for (int i = 0; i < size; i++)
		{
			_a.push_back(a[i]);
		}
		for (int i = (_a.size() - 2) / 2; i >= 0; i--)
		{
			_AdjustDown(i);
		}
	}
public:
	void Push(const T& value)
	{
		_a.push_back(value);//插入
		_AdjustDown(_a.size() - 1);
	}
	void Pop()
	{
		assert(!_a.empty());
		swap(_a[0], _a[_a.size() - 1]);//将根节点与最后一个节点交换
		_a.pop_back();
		_AdjustDown(0);//重新调整堆
	}
	void Print()
	{
		for (int i = 0; i < _a.size(); i++)
		{
			cout << _a[i] << " ";
			i++;
		}
		cout << endl;
	}
	int Size()
	{
		return _a.size();
	}
	bool Empty()
	{
		return _a.empty();
	}
	T& Top()
	{
		assert(_a.size() > 0);
		return _h[0];
	}
private:
	void _AdjustDown(int parent)
	{
		Compare com;
		int child = parent * 2 + 1;
		while (child<_a.size())
		{
			if (child + 1 < _a.size() && com(_a[child + 1], _a[chile]))
			{
				child++;
			}
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
				break;
		}
	}
	void _AdjustUp(int child)
	{
		Compare com;
		int parent = (child - 1) / 2;
		while (child > 0)
		{
			if (Compare(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
				break;
		}
	}
private:
	vector<T> _a;
};
#endif
