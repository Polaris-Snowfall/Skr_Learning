#ifndef __STL_algobase_H
#define __STL_algobase_H

#include "iterator.h"
#include <memory>
#include <cstring>



//template <class OutputIterator>
//inline void swap(OutputIterator a, OutputIterator b)
//{
//	OutputIterator tmp = a;
//	*a = *b;
//	*b = *tmp;
//}

template <class T>
inline void swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}


template <class T>
inline const T& min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template <class T>
inline const T& max(const T& a, const T& b)
{
	return a > b ? a : b;
}

template <class T, class ForwardIterator>
ForwardIterator find(ForwardIterator first,ForwardIterator last,const T& x)
{
	for (; first != last; ++first)
	{
		if (*first == x)
			return first;
	}
	return first;
}

template<class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
	for (; first != last; ++first)
	{
		*first = value;
	}
}

inline void fill(unsigned char* first, unsigned char* last,
	const unsigned char& ch) {
	unsigned char tmp = ch;
	memset(first, tmp, last - first);
}

inline void fill(signed char* first, signed char* last,
	const signed char& ch) {
	signed char tmp = ch;
	memset(first, static_cast<unsigned char>(tmp), last - first);
}

inline void fill(char* first, char* last, const char& ch) {
	char tmp = ch;
	memset(first, static_cast<unsigned char>(tmp), last - first);
}

template<class ForwardIterator, class Size, class T>
ForwardIterator fill_n(ForwardIterator first, Size n, const T& x)
{
	while (n--)
	{
		*first++ = x;
	}
	return first;
}

template <class RandomAccessIterator, class OutputItertor,class Distance>
inline OutputItertor __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputItertor result,Distance*)
{
	for (Distance n = last - first; n > 0; --n, ++result, ++first)
		*result = *first;
	return result;
}

template <class T>
inline T* __copy_t(T* first, T* last, T* result,__true_type)
{
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}

template <class T>
inline const T* __copy_t(const T* first, const T* last, const T* result, __true_type)
{
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}

template <class T>
inline T* __copy_t(T* first, T* last, T* result, __false_type)
{
	return __copy_d(first, last, result,distance_type(first));
}

template <class T>
inline const T* __copy_t(const T* first, const T* last, const T* result, __false_type)
{
	return __copy_d(first, last, result, distance_type(first));
}

template <class InputIterator, class OutputItertor>
inline OutputItertor __copy(InputIterator first, InputIterator last, OutputItertor result,input_iterator_tag)
{
	for (; first != last; ++first, ++result)
	{
		*result = *first;
	}
	return result;
}

template <class InputIterator, class OutputItertor>
inline OutputItertor __copy(InputIterator first, InputIterator last, OutputItertor result, random_access_iterator_tag)
{
	return __copy_d(first,last,result,distance_type(first));
}

template <class T>
inline T* __copy_dispatch(T* first, T* last,T* result)
{
	typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment;
	return __copy_t(first, last, result, trivial_assignment());
}

template <class T>
inline const T* __copy_dispatch(const T* first, const T* last, const T* result)
{
	typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment;
	return __copy_t(first, last, result, trivial_assignment());
}

template <class InputIterator, class OutputItertor>
inline OutputItertor __copy_dispatch(InputIterator first, InputIterator last, OutputItertor result)
{
	return __copy(first, last, result, iterator_category(first));
}

template <class InputIterator,class OutputItertor>
inline OutputItertor copy(InputIterator first, InputIterator last, OutputItertor result)
{
	return __copy_dispatch(first, last, result);
}

inline char* copy(const char* first, const char* last, char* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 
__copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result,bidirectional_iterator_tag)
{
	for (; last != first; --last,--result)
	{
		*result = *last;
	}
	return result;
}

template <class RandomAccessIterator,class BidirectionalIterator>
BidirectionalIterator
__copy_backward(RandomAccessIterator first, RandomAccessIterator last, BidirectionalIterator result,random_access_iterator_tag)
{
	typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
	Distance n = last - first;
	for (; n > 0; --n)
	{
		*--result = *--last;
	}
	return result;
}

template<class BidirectionalIterator1,class BidirectionalIterator2>
inline BidirectionalIterator2 
copy_backward(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result)
{
	return __copy_backward(first, last, result, iterator_category(first));
}
#endif // !algobase
