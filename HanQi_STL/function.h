#ifndef __STL_FUNCTION_H
#define __STL_FUNCTION_H

template <class Arg,class Result>
struct unary_function {
	typedef Arg arguement_type;
	typedef Result result_type;
};

template <class Arg1,class Arg2,class Result>
struct binary_function {
	typedef Arg1 first_arguement_type;
	typedef Arg2 second_arguement_type;
	typedef Result result_type;
};

template <class T>
struct less : public binary_function<T,T,bool>{
	bool operator()(const T& x, const T& y) const
	{
		return x < y;
	}
};

template <class T>
struct greater : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const
	{
		return x > y;
	}
};

template <class T>
struct identity : public unary_function<T,T> {
	const T& operator()(const T& x) const
	{
		return x;
	}
};


template <class Pair>
struct select1st :public unary_function<Pair,typename Pair::first_type>{
	const typename Pair::first_type& operator()(const Pair& x) const
	{
		return x.first;
	}
};

template <class Pair>
struct select2nd :public unary_function<Pair, typename Pair::second_type > {
	const typename Pair::second_type& operator()(const Pair& x) const
	{
		return x.second;
	}
};
#endif //__STL_FUNCTION_H