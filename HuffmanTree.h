#ifndef __HUFFMANTREE_H__
#define __HUFFMANTREE_H__

#include"Heap.h"

template<class T>
typedef HuffmanTreeNode<T> Node
struct HuffmanTreeNode
{
	T _weight;
	Node* _right;
	Node* _left;
	HuffmanTreeNode(T &weight)
		:_weight(weight)
		, _right(NULL)
		, _left(NULL)
	{}
};
template<class T>
class HuffmanTree
{
public:
	HuffmanTree()
		:_root(NULL)
	{}
	HuffmanTree(const T*array, int size, const T& invalid)
	{
		_root = _create(array, size, invalid);
	}

	HuffmanTree()
	{
		if (_root != NULL)
	               _destroy(_root);
	}
	Node* GetRoot()
	{
	       return _root;
	}
private:
	Node* _create(const T* array, int size, const T&invalid)
	{
		struct compare
		{
			bool operator()(HuffmanTreeNode<T>*left, HuffmanTreeNode<T>*right)
			{
				return left->_weight<right->_weight;
			}
		};

		Heap<Node*, compare>h;
		for (int i = 0; i<size; i++)
		{
			if (array[i] != invalid)
			{
				Node*tmp = new Node(array[i]);
				h.Push(tmp);
			}
		}
		while (h.size()>1)
		{
			Node*left = h.Top();
			h.Pop();
			Node*right = h.Top();
			h.Pop();

			Node*parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;

			h.Push(parent);
		}
		return h.Top();
	}
private:
	Node* _root;
};
#endif
