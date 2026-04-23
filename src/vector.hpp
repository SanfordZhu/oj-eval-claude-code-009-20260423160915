#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <new>

namespace sjtu
{
template<typename T>
class vector
{
public:
	class const_iterator;
	class iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		T* ptr;
		const vector* container;

	public:
		iterator(T* p = nullptr, const vector* c = nullptr) : ptr(p), container(c) {}

		iterator operator+(const int &n) const
		{
			return iterator(ptr + n, container);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n, container);
		}
		int operator-(const iterator &rhs) const
		{
			if (container != rhs.container) {
				throw invalid_iterator();
			}
			return static_cast<int>(ptr - rhs.ptr);
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++ptr;
			return tmp;
		}
		iterator& operator++()
		{
			++ptr;
			return *this;
		}
		iterator operator--(int)
		{
			iterator tmp = *this;
			--ptr;
			return tmp;
		}
		iterator& operator--()
		{
			--ptr;
			return *this;
		}
		T& operator*() const
		{
			return *ptr;
		}
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}

		friend class const_iterator;
		friend class vector;
	};

	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		const T* ptr;
		const vector* container;

	public:
		const_iterator(const T* p = nullptr, const vector* c = nullptr) : ptr(p), container(c) {}
		const_iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr + n, container);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr - n, container);
		}
		int operator-(const const_iterator &rhs) const
		{
			if (container != rhs.container) {
				throw invalid_iterator();
			}
			return static_cast<int>(ptr - rhs.ptr);
		}
		const_iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			++ptr;
			return tmp;
		}
		const_iterator& operator++()
		{
			++ptr;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator tmp = *this;
			--ptr;
			return tmp;
		}
		const_iterator& operator--()
		{
			--ptr;
			return *this;
		}
		const T& operator*() const
		{
			return *ptr;
		}
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}

		friend class vector;
	};

private:
	T* data;
	size_t sz;
	size_t cap;

	void expand()
	{
		size_t new_cap = (cap == 0) ? 1 : cap * 2;
		T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
		for (size_t i = 0; i < sz; ++i) {
			new (new_data + i) T(data[i]);
		}
		for (size_t i = 0; i < sz; ++i) {
			(data + i)->~T();
		}
		::operator delete(data);
		data = new_data;
		cap = new_cap;
	}

public:
	vector() : data(nullptr), sz(0), cap(0) {}

	vector(const vector &other) : data(nullptr), sz(0), cap(0)
	{
		reserve(other.sz);
		for (size_t i = 0; i < other.sz; ++i) {
			push_back(other.data[i]);
		}
	}

	~vector()
	{
		clear();
		::operator delete(data);
	}

	vector &operator=(const vector &other)
	{
		if (this == &other) return *this;
		clear();
		reserve(other.sz);
		for (size_t i = 0; i < other.sz; ++i) {
			push_back(other.data[i]);
		}
		return *this;
	}

	T & at(const size_t &pos)
	{
		if (pos >= sz) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	const T & at(const size_t &pos) const
	{
		if (pos >= sz) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	T & operator[](const size_t &pos)
	{
		if (pos >= sz) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	const T & operator[](const size_t &pos) const
	{
		if (pos >= sz) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	const T & front() const
	{
		if (sz == 0) {
			throw container_is_empty();
		}
		return data[0];
	}

	const T & back() const
	{
		if (sz == 0) {
			throw container_is_empty();
		}
		return data[sz - 1];
	}

	iterator begin()
	{
		return iterator(data, this);
	}

	const_iterator begin() const
	{
		return const_iterator(data, this);
	}

	const_iterator cbegin() const
	{
		return const_iterator(data, this);
	}

	iterator end()
	{
		return iterator(data + sz, this);
	}

	const_iterator end() const
	{
		return const_iterator(data + sz, this);
	}

	const_iterator cend() const
	{
		return const_iterator(data + sz, this);
	}

	bool empty() const
	{
		return sz == 0;
	}

	size_t size() const
	{
		return sz;
	}

	void clear()
	{
		for (size_t i = 0; i < sz; ++i) {
			(data + i)->~T();
		}
		sz = 0;
	}

	void reserve(size_t n)
	{
		if (n > cap) {
			T* new_data = static_cast<T*>(::operator new(n * sizeof(T)));
			for (size_t i = 0; i < sz; ++i) {
				new (new_data + i) T(data[i]);
			}
			for (size_t i = 0; i < sz; ++i) {
				(data + i)->~T();
			}
			::operator delete(data);
			data = new_data;
			cap = n;
		}
	}

	iterator insert(iterator pos, const T &value)
	{
		size_t index = pos.ptr - data;
		return insert(index, value);
	}

	iterator insert(const size_t &ind, const T &value)
	{
		if (ind > sz) {
			throw index_out_of_bound();
		}
		if (sz == cap) {
			expand();
		}
		for (size_t i = sz; i > ind; --i) {
			new (data + i) T(data[i - 1]);
			(data + i - 1)->~T();
		}
		new (data + ind) T(value);
		++sz;
		return iterator(data + ind, this);
	}

	iterator erase(iterator pos)
	{
		size_t index = pos.ptr - data;
		return erase(index);
	}

	iterator erase(const size_t &ind)
	{
		if (ind >= sz) {
			throw index_out_of_bound();
		}
		(data + ind)->~T();
		for (size_t i = ind; i < sz - 1; ++i) {
			new (data + i) T(data[i + 1]);
			(data + i + 1)->~T();
		}
		--sz;
		return iterator(data + ind, this);
	}

	void push_back(const T &value)
	{
		if (sz == cap) {
			expand();
		}
		new (data + sz) T(value);
		++sz;
	}

	void pop_back()
	{
		if (sz == 0) {
			throw container_is_empty();
		}
		--sz;
		(data + sz)->~T();
	}
};

}

#endif
