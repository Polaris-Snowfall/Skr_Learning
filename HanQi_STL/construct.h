#ifndef __STL_Construct_H
#define __STL_Construct_H

#include "uninitialized.h"
#include <new.h>
#include "algobase.h"




template <class T1, class T2>
inline void _Construct(T1* __p, T2 __value)
{
	new(__p) T1(__value);
}

template <class T1>
inline void _Construct(T1* __p)
{
	new(__p) T1();
}

template <class T1,class T2>
inline void construct(T1* __p, T2 __value)
{
	_Construct(__p, __value);
}

template <class T1>
inline void construct(T1* __p)
{
	_Construct(__p);
}



template <class ForwardIterator>
inline void __Destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
{
	//不做处理
}

template <class ForwardIterator>
inline void __Destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
	for (; first != last; ++first)
	{
		_Destroy(&*first);
	}
}

template <class T>
inline void _Destroy(T* __p)
{
	__p->~T();
}

template <class ForwardIterator, class T>
inline void _Destroy(ForwardIterator first, ForwardIterator last,T*)
{
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__Destroy_aux(first, last, trivial_destructor());
}

template <class T>
inline void destroy(T* __p)
{
	_Destroy(__p);
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	_Destroy(first, last,value_type(first));
}




#endif