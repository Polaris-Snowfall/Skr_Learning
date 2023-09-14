#ifndef __STL_DEQUE_H
#define __STL_DEQUE_H
#include "alloc.h"
#include "construct.h"

template<class T, class Alloc = __default_alloc_template, size_t BufSize = 0>
class deque;

inline size_t __deque_buf_size(size_t n, size_t sz)
{
	return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template <class T, class Ref, class Ptr, size_t BufSize = 0>
struct __deque_iterator {
	friend class deque<T>;

	typedef __deque_iterator<T, T&, T*, BufSize> iterator;
	typedef __deque_iterator<T, Ref, Ptr, BufSize> self;

	static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }

	typedef random_access_iterator_tag iterator_category;
	typedef T            value_type;
	typedef Ptr          pointer;
	typedef Ref          reference;
	typedef size_t       size_type;
	typedef ptrdiff_t    difference_type;
	typedef T* value_pointer;
	typedef T** map_pointer;

	T* cur;
	T* first;
	T* last;
	map_pointer node;


protected:
	void set_node(map_pointer new_node)
	{
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}

public:
	reference operator*() const { return *cur; }
	pointer operator->() const { return &(operator*()); }
	difference_type operator-(const self& x) const
	{
		return difference_type(buffer_size()) * (node - x.node - 1) +
			(cur - first) + (x.last - x.cur);
	}

	self& operator++()
	{
		++cur;
		if (cur == last)
		{
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	self operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self operator--()
	{
		if (cur == first)
		{
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}

	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	self& operator+=(difference_type n)
	{
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size()))
			cur += n;
		else
		{
			difference_type node_offset =
				offset > 0 ? offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}

		return *this;
	}

	self operator+(difference_type n) const
	{
		self tmp = *this;
		return tmp += n; //const函数能调非const函数?
	}

	self operator-=(difference_type n) { return *this += -n; }

	self operator-(difference_type n) const
	{
		self tmp = *this;
		return tmp -= n;
	}

	reference operator[](difference_type n)const { return *(*this + n); }
	bool operator==(const self& x) const { return cur == x.cur; }
	bool operator!=(const self& x) const { return cur != x.cur; }
	bool operator<(const self& x) const
	{
		return (node == x.node) ? (cur < x.cur) : (node < x.node);
	}
};

template<class T, class Alloc, size_t BufSize>
class deque {
public:
	typedef T			value_type;
	typedef __deque_iterator<T, T&, T*, BufSize> iterator;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

protected:
	typedef pointer* map_pointer;
	typedef simple_alloc<value_type, Alloc> data_allocator;
	typedef simple_alloc<pointer, Alloc> map_allocator;
	enum { initial_map_size = 8 };

	iterator start;
	iterator finish;
	map_pointer map;
	size_t map_size;

	static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }

	pointer allocate_buf()
	{
		return data_allocator::allocate(buffer_size());
	}

	void deallocate_buf(T* p)
	{
		data_allocator::deallocate(p, buffer_size());
	}

	void create_map_and_nodes(size_type num_elements)
	{
		size_type num_nodes = num_elements / buffer_size() + 1;
		//最少8个,最多所需node+2
		map_size = max(size_t(initial_map_size), num_nodes + 2);
		map = map_allocator::allocate(map_size);

		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes;

		map_pointer cur;
		for (cur = nstart; cur < nfinish; ++cur)
			*cur = allocate_buf();
		start.set_node(nstart);
		finish.set_node(nfinish - 1);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % buffer_size();
	}

	void fill_initialize(size_type n, const value_type& value)
	{
		create_map_and_nodes(n);
		map_pointer cur;
		for (cur = start.node; cur < finish.node; ++cur)
			uninitialized_fill(*cur, *cur + buffer_size(), value);
		uninitialized_fill(finish.first, finish.cur, value);
	}


	void reallocate_map(size_type nodes_to_add, bool add_at_front)
	{
		size_t old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes)
		{
			new_nstart = map + (map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node)
				copy(start.node, finish.node + 1, new_nstart);
			else
				copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
		}
		else
		{
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
			map_pointer new_map = map_allocator::allocate(new_map_size);;
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes + 1);
	}

	void reserve_map_at_back(size_type nodes_to_add = 1)
	{
		if (nodes_to_add > map_size - (finish.node - map + 1))
		{
			reallocate_map(nodes_to_add, false);
		}
	}

	void reverse_map_at_front(size_type nodes_to_add = 1)
	{
		if (nodes_to_add > start.node - map)
		{
			reallocate_map(nodes_to_add, true);
		}
	}

	void push_back_aux(const value_type& x)
	{
		reserve_map_at_back();
		*(finish.node + 1) = allocate_buf();
		construct(finish.cur, x);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}

	void push_front_aux(const value_type& x)
	{
		reverse_map_at_front();
		*(start.node - 1) = allocate_buf();
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, x);
	}

	void pop_back_aux()
	{
		deallocate_buf(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}

	void pop_front_aux()
	{
		destroy(start.cur);
		deallocate_buf(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

public:
	iterator begin() { return start; }
	iterator end() { return finish; }

	reference operator[](size_type n) { return start[difference_type(n)]; }
	reference front() { return *start; }
	reference back() //*finish是越界访问!
	{
		iterator tmp = finish;
		--tmp;
		return *tmp;
	}

	size_type size() const { return finish - start; }
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return finish == start; }

	deque(int n, const value_type& value)
		:start(), finish(), map(0), map_size(0)
	{
		fill_initialize(n, value);
	}

	deque()
		:start(), finish(), map(0), map_size(0)
	{
		fill_initialize(0, value_type());
	}

	void push_back(const value_type& x)
	{
		if (finish.cur != finish.last - 1)
		{
			construct(finish.cur, x);
			++finish.cur;
		}
		else
			push_back_aux(x);
	}

	void push_front(const value_type& x)
	{
		if (start.cur != start.first)
		{
			construct(start.cur - 1, x);
			--start.cur;
		}
		else
			push_front_aux(x);
	}

	void pop_back()
	{
		if (finish.cur != finish.first)
		{
			--finish.cur;
			destroy(finish.cur);
		}
		else
			pop_back_aux();
	}

	void pop_front()
	{
		if (start.cur != start.last - 1)
		{
			destroy(start.cur);
			++start.cur;
		}
		else
			pop_front_aux();
	}

	void clear()
	{
		for (map_pointer node = start.node + 1; node < finish.node; ++node)
		{
			destroy(*node, *node + buffer_size());
			deallocate_buf((*node));
		}
		if (start.node != finish.node)
		{
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			deallocate_buf(finish.first);
		}
		else
			destroy(start.cur, finish.cur);
		start.cur = start.first;//源码没有这一句,但我觉得应该有?
		finish = start;
	}

	iterator erase(iterator pos)
	{
		iterator next = pos;
		++next;
		difference_type index = pos - start;
		if (index < (size() / 2))
		{
			copy_backward(start, pos, next);
			pop_front();
		}
		else
		{
			copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}

	iterator erase(iterator first, iterator last)
	{
		if (first == start && last == finish)
		{
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first;
			difference_type elems_before = first - start;
			if (elems_before < (size() - n) / 2)
			{
				copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
					deallocate_buf(*cur);
				start = new_start;
			}
			else
			{
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);
				for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
					deallocate_buf(*cur);
				finish = new_finish;
			}
			return start + elems_before;
		}
	}

	iterator insert_aux(iterator pos, const value_type& x)
	{
		difference_type index = pos - start;
		if (index < size() / 2)
		{
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			copy_backward(pos, back2, back1);
		}
		*pos = x;
		return pos;
	}

	iterator insert(iterator position, const value_type& x)
	{
		if (position.cur == start.cur)
		{
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur)
		{
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else
			return insert_aux(position, x);
	}
};

#endif // !__STL_DEQUE_H
