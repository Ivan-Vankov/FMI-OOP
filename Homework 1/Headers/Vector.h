#ifndef _VECTOR_HEADER_
#define _VECTOR_HEADER_
#include <iostream>
#include <exception>

template <class T>
class Vector{
public:
	Vector();
	~Vector();
	Vector(int);
	Vector(int, const T&);
	Vector(const Vector&);
	Vector& operator=(const Vector&);
public:
	void add(const T&);
	void add(int, const T&);
	void remove(const T&);
	void remove(int);
	void removeWithNoShuffle(int);
	void resize(int);
public:
	T& last()const;
public:
	int getAllocatedSize() const;
	int getSize() const;
	void print() const;
	void clean();
private:
	void reallocate(int);
	void copyFrom(const Vector&);
public:
	bool isIn(const T&) const;
public:
	int indexOf(const T&) const;
public:

	const T& operator[](int) const;
	T& operator[](int);
private:
	T* arr;
	int allocSize;
	int size;
};

#include "Vector.hpp"
#endif