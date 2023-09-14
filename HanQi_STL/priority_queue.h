#ifndef __STL_PRI_QUEUE_H
#define __STL_PRI_QUEUE_H
#include "vector.h"
#include "function.h"
#include "heap.h"

template <class T,class Sequence = vector<T>,class Compare = less<typename Sequence::value_type> >
class priority_queue {
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;

	priority_queue():seq(),comp(){}

	template <class Inputiterator>
	priority_queue(Inputiterator first, Inputiterator last) :
		seq(first, last),comp()
	{
		make_heap(seq.begin(), seq.end(),comp);
	}

	bool empty() const{ return seq.empty(); }
	size_type size() const { return seq.size(); }
	const_reference top()  { return seq.front(); }
	void push(const value_type& x)
	{
		seq.push_back(x);
		push_heap(seq.begin(),seq.end(), comp);
	}
	void pop()
	{
		pop_heap(seq.begin(), seq.end(), comp);
		seq.pop_back();
	}

protected :
	Sequence seq;
	Compare comp;
};
#endif // !__STL_PRI_QUEUE_H
