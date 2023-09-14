#ifndef __STL_alloc_H
#define __STL_alloc_H

#if 0
#	include<new>
#	define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#	include<iostream>
#	define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl;exit(1)
#endif


class __malloc_alloc_template {
	//out_of_memory处理
private:
	static void* oom_malloc(size_t nbytes);
	static void* oom_realloc(void*, size_t nbytes);
	static void (*__malloc_alloc_oom_handler)();
public:
	static void* allocate(size_t nbytes)
	{
		void* result = malloc(nbytes);
		if (result == NULL)
			result = oom_malloc(nbytes);
		return result;
	}

	static void* reallocate(void* old_ptr, size_t nbytes)
	{
		void* result = realloc(old_ptr, nbytes);
		if (result == NULL)
			result = oom_realloc(old_ptr, nbytes);
		return result;
	}

	static void deallocate(void* ptr, size_t nbytes)
	{
		free(ptr);
	}

	static void(*set_malloc_handler(void(*f)()))()
	{
		void (*old_handler)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return old_handler;
	}
};

void (*__malloc_alloc_template::__malloc_alloc_oom_handler)() = NULL;

void* __malloc_alloc_template::oom_malloc(size_t nbytes)
{
	for (;;)
	{
		void (*my_malloc_handler)() = __malloc_alloc_oom_handler;
		if (my_malloc_handler == NULL)
			__THROW_BAD_ALLOC;
		(*my_malloc_handler)();
		void* result = malloc(nbytes);
		if (result)
			return result;
	}
}

void* __malloc_alloc_template::oom_realloc(void* old_ptr, size_t nbytes)
{
	for (;;)
	{
		void (*my_malloc_handler)() = __malloc_alloc_oom_handler;
		if (my_malloc_handler == NULL)
			__THROW_BAD_ALLOC;
		(*my_malloc_handler);
		void* result = realloc(old_ptr, nbytes);
		if (result)
			return result;
	}
}

enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };


class __default_alloc_template {
private:
	//将nbytes对齐到8字节(___ALIGN的倍数)
	static size_t ROUND_UP(size_t nbytes)
	{
		return (nbytes + __ALIGN - 1) & ~(__ALIGN - 1);
	}
private:
	union obj
	{
		union obj* free_list_link;
		char client_data[1];
	};
private:
	static obj* volatile free_list[__NFREELISTS];//volatile加在这里有啥用?
	static size_t FREELIST_INDEX(size_t nbytes)
	{
		return (nbytes + __ALIGN - 1) / __ALIGN - 1;
	}

	static void* refill(size_t nbytes);
	static char* chunk_alloc(size_t size, int& nobjs);

	static char* start_free;
	static char* end_free;
	static size_t heap_size;//?

public:
	static void* allocate(size_t nbytes);
	static void* reallocate(void* old_ptr, size_t nbytes);
	static void deallocate(void* ptr, size_t nbytes);

};

//static data member的定义和初值设定在类外完成.

char* __default_alloc_template::start_free = 0;
char* __default_alloc_template::end_free = 0;
size_t __default_alloc_template::heap_size = 0;

__default_alloc_template::obj* volatile __default_alloc_template::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

void* __default_alloc_template::allocate(size_t nbytes)
{
	obj* volatile* my_free_list;
	obj* result;
	if (nbytes > (size_t)__MAX_BYTES)
		return __malloc_alloc_template::allocate(nbytes);
	my_free_list = free_list + FREELIST_INDEX(nbytes);
	result = *my_free_list;
	if (result == NULL)
	{
		void* r = refill((ROUND_UP(nbytes)));
		return r;
	}
	*my_free_list = result->free_list_link;
	return result;
}

void __default_alloc_template::deallocate(void* ptr, size_t nbytes)
{
	obj* obj_ptr = (obj*)(ptr);
	obj* volatile* my_free_list;
	if (nbytes > (size_t)__MAX_BYTES)
	{
		__malloc_alloc_template::deallocate(ptr, nbytes);
		return;
	}
	my_free_list = free_list + FREELIST_INDEX(nbytes);
	obj_ptr->free_list_link = *my_free_list;
	*my_free_list = obj_ptr;
}

void* __default_alloc_template::refill(size_t nbytes)
{
	int nobjs = 20;
	char* chunk = chunk_alloc(nbytes, nobjs);
	obj* volatile* my_free_list;
	obj* result;
	obj* current_obj, * next_obj;
	int i;
	if (nobjs == 1)
		return chunk;
	my_free_list = free_list + FREELIST_INDEX(nbytes);
	result = (obj*)chunk;
	current_obj = (obj*)chunk;
	*my_free_list = (obj*)(chunk + nbytes);
	for (i = 0;; i++)
	{
		if (i  == nobjs-1)
		{
			current_obj->free_list_link = NULL;
			break;
		}
		next_obj = (obj*)((char*)current_obj + nbytes);
		current_obj->free_list_link = next_obj;
		current_obj = next_obj;
	}
	return result;
}

char* __default_alloc_template::chunk_alloc(size_t nbytes, int& nobjs)
{
	size_t total_bytes = nbytes * nobjs;
	size_t remain_bytes = end_free - start_free;
	char* results = start_free;
	if (total_bytes <= remain_bytes)
	{
		start_free += total_bytes;
		return results;
	}
	if (remain_bytes >= nbytes)
	{
		nobjs = (remain_bytes) / nbytes;
		total_bytes = nbytes * nobjs;
		start_free += total_bytes;
		return results;
	}
	else
	{
		//把残余的内存链入free_list
		if (remain_bytes > 0)
		{
			start_free += remain_bytes;
			obj* volatile* my_free_list = free_list + FREELIST_INDEX(remain_bytes);
			((obj*)results)->free_list_link = *my_free_list;
			*my_free_list = (obj*)results;
		}

		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);//这个heap_size什么意思没搞懂
		start_free = (char*)malloc(bytes_to_get);
		if (start_free == NULL)
		{
			obj* volatile* my_free_list;
			obj* p;
			for (int i = nbytes; i <= __MAX_BYTES; i += __ALIGN)
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (p != NULL)
				{
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					return chunk_alloc(nbytes, nobjs);
				}
			}
			end_free = NULL;
			start_free = (char*)__malloc_alloc_template::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return chunk_alloc(nbytes, nobjs);
	}
}

template <class T, class Alloc = __default_alloc_template>
class simple_alloc {
public:
	static T* allocate(size_t n)
	{
		return n == 0 ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	static T* allocate()
	{
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* ptr, size_t n)
	{
		if (n != NULL)
			Alloc::deallocate(ptr, n * sizeof(T));
	}
	static void deallocate(T* ptr)
	{
		Alloc::deallocate(ptr, sizeof(T));
	}
};
#endif
