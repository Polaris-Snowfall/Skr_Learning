#ifndef __STL_vector_H
#define __STL_vector_H


#include "alloc.h"
#include "construct.h"
#include "iterator_base.h"

template <class T,class Alloc=__default_alloc_template>
class vector {
public:
	typedef T			value_type;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;

	iterator start;
	iterator finish;
	iterator end_of_storage;

	void insert_aux(iterator position, const T& x);
	void deallocate()
	{
		if (start)
			data_allocator::deallocate(start, end_of_storage - start);
	}

	void fill_initialize(size_type n, const T& value)
	{
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}

public:
	iterator begin() const { return start; }
	iterator end() const { return finish; }
	size_type size() const { return size_type(end() - begin()); }
	size_type capacity() const { return size_type(end_of_storage - begin()); }
	bool empty() const { return begin() == end(); }
	reference operator[] (size_type n){return *(begin() + n);}

	vector():start(0),finish(0),end_of_storage(0){}
	vector(size_type n, const T& value) { fill_initialize(n, value); }
	template<class InputIterator>
	vector(InputIterator first, InputIterator last);
	explicit vector(size_type n) { fill_initialize(n, T()); }
	vector<T, Alloc>& operator=(const vector<T, Alloc>& x);

	vector(const vector<T, Alloc>& x)
		: start(0), finish(0), end_of_storage(0)
	{
		start = data_allocator::allocate(difference_type(x.size()));
		uninitialized_copy(x.begin(),x.end(), start);
		finish = start + difference_type(x.size());
		end_of_storage = finish;
	}

	~vector()
	{
		destroy(start,finish);
		deallocate();
	}

	reference front() { return *(begin()); }
	reference back() { return *(end()-1); }
	void push_back(const T& x)
	{
		if (finish != end_of_storage)
		{
			construct(finish, x);
			finish++;
		}
		else
			insert_aux(end(), x);
	}

	void pop_back()
	{
		--finish;
		destroy(finish);
	}

	iterator erase(iterator first, iterator last)
	{
		iterator i = copy(last, finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	}

	iterator erase(iterator position)
	{
		if (position + 1 != end())
			copy(position + 1, finish, position);
		--finish;
		destroy(finish);
		return position;
	}

	void resize(size_type new_size, const T& x)
	{
		if (new_size < size())
		{
			erase(begin() + new_size, end());
		}
		else
			insert(end(), new_size - size(), x);
	}

	void resize(size_type new_size) { resize(new_size, T()); }
	void clear() { erase(begin(), end()); }

	void insert(iterator position, size_type n, const T& x);


protected:
	iterator allocate_and_fill(size_type n, const T& x)
	{
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}

	template <class _ForwardIterator>
	iterator allocate_and_copy(size_type __n, _ForwardIterator __first,
		_ForwardIterator __last)
	{
		iterator __result = data_allocator::allocate(__n);
		  uninitialized_copy(__first, __last, __result);
		  return __result;
	}
};

template <class T, class Alloc>
template<class InputIterator>
vector<T,Alloc>::vector(InputIterator first, InputIterator last)
:start(0),finish(0),end_of_storage(0)
{
	start = data_allocator::allocate(difference_type(last - first));
	uninitialized_copy(first, last, start);
	finish = start+difference_type(last - first);
	end_of_storage = finish;
}

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x)
{
	if (finish != end_of_storage)
	{
		construct(finish, *(finish - 1));
		++finish;
		T x_copy = x;
		copy_backward(position, finish - 2, finish - 1);
		*position = x_copy;
	}
	else
	{
		const size_type old_size = size();
		const size_type len = old_size!=0?2*old_size:1;
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;

		new_finish = uninitialized_copy(start, position, new_start);
		construct(new_finish, x);
		++new_finish;
		new_finish = uninitialized_copy(position, finish, new_finish);

		destroy(begin(), end());
		deallocate();

		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}

}

template <class T,class Alloc>
void vector<T,Alloc>::insert(iterator position,size_type n, const T& x)
{
	if (n != 0)
	{
		if (size_type(end_of_storage - finish) >= n)
		{
			T x_copy = x;
			const size_type elem_after = finish - position;
			iterator old_finish = finish;
			if (elem_after > n)
			{
				uninitialized_copy(finish - n, finish, finish);
				finish += n;
				copy_backward(position, old_finish - n, old_finish);
				fill(position, position + n, x_copy);
			}
			else
			{
				uninitialized_fill_n(finish, n - elem_after, x_copy);
				finish += n - elem_after;
				uninitialized_copy(position, old_finish, finish);
				finish += elem_after;
				fill(position, old_finish, x_copy);
			}
		}
		else
		{
			const size_type old_size = size();
			const size_type len = old_size + max(old_size, n);
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			
			new_finish = uninitialized_copy(start, position, new_start);
			new_finish = uninitialized_fill_n(new_finish, n, x);
			new_finish = uninitialized_copy(position, finish, new_finish);

			destroy(start, finish);
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}
}



template <class T, class Alloc>
vector<T, Alloc>&
vector<T, Alloc>::operator=(const vector<T, Alloc>& x)
{
	if (&x != this) {
		const size_type xlen = x.size();
		if (xlen > capacity()) {
			iterator tmp = allocate_and_copy(xlen, x.begin(), x.end());
			destroy(start, finish);
			deallocate();
			start = tmp;
			end_of_storage = start + xlen;
		}
		else if (size() >= xlen) {
			iterator i = copy(x.begin(), x.end(), begin());
			destroy(i, finish);
		}
		else {
			copy(x.begin(), x.begin() + size(), start);
			uninitialized_copy(x.begin() + size(), x.end(), finish);
		}
		finish = start + xlen;
	}
	return *this;
}



#endif // !__STL_vector_H
