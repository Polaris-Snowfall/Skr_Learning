#ifndef  __STL_SET_H
#define __STL_SET_H
#include "tree.h"
#include "alloc.h"
#include "function.h"

template <class Key,class Compare = less<Key>,class Alloc=__default_alloc_template>
class set {
public:
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;
private:
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
	rep_type t;

public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	// iterator定义为const，不允许写入操作
	typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;


	set() : t(Compare()) {}
	explicit set(const Compare& comp) :t(comp) {}

	template <class Inputiterator>
	set(Inputiterator first, Inputiterator last)
		: t(Compare())
	{
		t.insert_unique(first, last);
	}

	template <class Inputiterator>
	set(Inputiterator first, Inputiterator last, const Compare& comp)
		: t(comp) {
		t.insert_unique(first, last);
	}

	set(const set<Key,Compare,Alloc>& x):t(x.t) {}
	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x)
	{
		t = x.t;
		return *this;
	}

	key_compare key_comp() const { return t.Key_comp(); }
	value_compare value_comp() const { return t.key_comp(); }
	iterator begin() const { return t.begin(); }
	iterator end() const { return t.end(); }
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }
	size_type max_size() const { return t.max_size(); }

	typedef pair<iterator, bool> pair_iterator_bool;
	pair<iterator, bool> insert(const value_type& x)
	{
		pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
		return pair<iterator, bool>(p.first, p.second);
	}
	template<class Inputiterator>
	void insert(Inputiterator first, Inputiterator last)
	{
		t.insert_unique(first, last);
	}

	size_type erase(const key_type& x)
	{
		return t.erase(x);
	}

	void erase(iterator first, iterator last)
	{
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)first, (rep_iterator&)last);
	}

	void clear() { t.clear(); }
	iterator find(const key_type& x) const { return t.find(x); }
	size_type count(const key_type& x) const { return t.count(x); }

	iterator lower_bound(const key_type& x) const 
	{
		return t.lower_bound(x);
	}

	iterator upper_bound(const key_type& x) const 
	{
		return t.upper_bound(x);
	}

	pair<iterator, iterator> equal_range(const key_type& x) const
	{
		return t.equal_range(x);
	}

};


#endif // ! __STL_SET_H
