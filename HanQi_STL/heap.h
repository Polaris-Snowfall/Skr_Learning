#ifndef __STL_HEAP_H
#define __STL_HEAP_H

#include "iterator.h"
#include "function.h"

template <class RandomAccessIterator, class Distance, class T,class Compare>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, Compare& comp)
{
	Distance parent = (holeIndex - 1) / 2;
	while (holeIndex > topIndex && comp(*(first + parent),value))
	{
		*(first + holeIndex) = *(first + parent);
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;
}

template <class RandomAccessIterator, class Distance, class T,class Compare>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare& comp)
{
	__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)),comp);
}

//push_heap函数只负责调整,执行前应已在末端插入结点.
template <class RandomAccessIterator,class Compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last,Compare& comp)
{
	__push_heap_aux(first, last, distance_type(first), value_type(first),comp);
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare& comp)
{
	Distance topIndex = holeIndex;
	Distance secondchild = holeIndex * 2 + 2;

	while (secondchild < len)
	{
		if (comp(*(first + secondchild),*(first + secondchild - 1)))
			secondchild--;
		*(first + holeIndex) = *(first + secondchild);
		holeIndex = secondchild;
		secondchild = 2 * (secondchild)+2;
	}
	if (secondchild == len)
	{
		*(first + holeIndex) = *(first + (secondchild - 1));
		holeIndex = secondchild - 1;
	}
	__push_heap(first, holeIndex, topIndex, value, comp);
	//*(first + holeIndex) = value; 
// STL源码剖析中侯捷指出这里可以改成这句,其实是错误的.
// 在make_heap调用__adjust_heap时更是错误.
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare& comp)
{
	*result = *first;
	__adjust_heap(first, Distance(0), Distance(last - first), value, comp);
}

template <class RandomAccessIterator,class T, class Compare>
inline void pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Compare& comp)
{
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first), comp);
}



//pop_heap只负责将跟结点移至末端
template <class RandomAccessIterator, class Compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare& comp)
{
	pop_heap_aux(first, last, value_type(first), comp);
}

template <class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare& comp)
{
	while (last - first > 1)
	{
		pop_heap(first, last--, comp);
	}
}

template <class RandomAccessIterator, class T, class Distance, class Compare>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*, Compare& comp)
{
	if (last - first < 2)
		return;
	Distance len = last - first;
	Distance parent = (len - 2) / 2;
	while (true)
	{
		__adjust_heap(first, parent, len, T(*(first + parent)), comp);
		if (parent == 0)
			return;
		parent--;
	}
}

template <class RandomAccessIterator, class Compare>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last,Compare& comp)
{
	return __make_heap(first, last, value_type(first), distance_type(first), comp);
}



#endif // !__STL_HEAP_H
