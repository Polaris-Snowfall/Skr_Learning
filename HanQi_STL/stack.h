#include "deque.h"



template <class T,class Sequence = deque<T> >
class stack {
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;

	bool empty() const { return seq.empty(); }
	size_t size() const { return seq.size(); }
	reference top() { return seq.back(); }
	void pop() { return seq.pop_back(); }
	void push(const value_type& x) { return seq.push_back(x); }

	friend bool operator== (const stack<T, Sequence>& x, const stack<T, Sequence>& y);

	stack(int n,const value_type& x)
		:seq(n,x)
	{
	
	}

	stack()
		:seq()
	{

	}
protected:
	Sequence seq;
};

template <class T,class Sequence>
bool operator== (const stack<T, Sequence>& x,const stack<T, Sequence>& y)
{
	return x.seq == y.seq;
}



