#ifndef __STL_uninitialized_H
#define __STL_uninitialized_H

#include "iterator.h"
#include "type_traits.h"
#include "algobase.h"



template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
	return copy(first, last, result);
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
{
	ForwardIterator cur = result;
	for (; first != last; ++first, ++cur)
	{
		construct(&*cur, *first);
	}
	return cur;
}

template <class InputIterator, class ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return __uninitialized_copy(first, last, result, value_type(result));
}



inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}

template <class ForwardIterator, class T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	__uninitialized_fill_aux(first, last, x, is_POD());
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type)
{
	fill(first, last, x);
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type)
{
	ForwardIterator cur = first;
	for (; cur != last; ++cur)
	{
		construct(&*cur, x);
	}
}

template <class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
	__uninitialized_fill(first, last, x, value_type(first));
}



template <class ForwardIterator, class _Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, _Size __n, const T& __x, __true_type)
{
	return fill_n(first, __n, __x);
}

template <class ForwardIterator, class _Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, _Size __n, const T& __x, __false_type)
{
	ForwardIterator cur = first;
	for (; __n > 0; ++cur)
		construct(&*cur, __x);
	return cur;
}

template <class ForwardIterator, class _Size, class T, class T1>
inline ForwardIterator
__uninitialized_fill_n(ForwardIterator first, _Size __n, const T& __x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, __n, __x, is_POD());;
}


template <class ForwardIterator, class _Size, class T>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, _Size __n, const T& __x)
{
	return __uninitialized_fill_n(first, __n, __x, value_type(first));
}




#endif // !__STL_uninitialized_H
