#include "Vector.h"

template <class T>
inline Vector<T>::Vector() : arr(NULL), allocSize(0), size(0){
}


template <class T>
inline Vector<T>::Vector(int amount) : arr(new T[amount]), allocSize(amount), size(amount) {
}

template <class T>
inline Vector<T>::Vector(int amount, const T& val) : arr(new T[amount]){
	for (int i = 0; i < amount; ++i){
		arr[i] = val;
	}
	allocSize = amount;
	size = amount;
}

template <class T>
inline Vector<T>::Vector(const T& val) :arr(new T){
	arr[0] = val;
	allocSize = size = 1;
}

template <class T>
inline Vector<T>::~Vector(){
	clean();
}

template <class T>
inline Vector<T>::Vector(const Vector& rhs){
	copyFrom(rhs);
}

template <class T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& rhs){
	if (this != &rhs){
		clean();
		copyFrom(rhs);
	}
	return *this;
}

template <class T>
inline void Vector<T>::pushBack(const T& rhs){
	if (size == allocSize){
		reallocate((allocSize ? 2 * allocSize : 2));
	}
	arr[size++] = rhs;
}

template <class T>
inline void Vector<T>::add(int index, const T& value){
	if (index > size || index < 0){
		throw std::out_of_range("Invalid index at Vector::add(int, const T&).");
	}
	T* beg = new T[index];
	for (int i = 0; i < index; ++i){
		beg[i] = arr[i];
	}
	T* end = new T[size - index];
	for (int j = index, i = 0; j < size; ++j, ++i){
		end[i] = arr[j];
	}
	if (size == allocSize){
		int reallocSize = (allocSize ? 2 * allocSize : 2);
		delete[]arr;
		arr = new T[reallocSize];
		allocSize = reallocSize;
	}
	arr[index] = value;
	++size;
	for (int i = 0; i < index; ++i){
		arr[i] = beg[i];
	}
	for (int j = index + 1, i = 0; j < size; ++j, ++i){
		arr[j] = end[i];
	}
	delete[] beg;
	delete[] end;
}

template <class T>
inline void Vector<T>::remove(const T& rhs){
	if (!isIn(rhs)){
		cerr << "No element " << rhs << " exists in the vector.";
		return;
	}
	int halfAllocSize = allocSize / 2;
	if (size <= halfAllocSize){
		reallocate(halfAllocSize);
	}
	int index = indexOf(rhs);
	arr[index] = arr[--size];
}

template <class T>
inline void Vector<T>::remove(int index){
	if (index >= size || index < 0){
		cerr << "Invalid index at Vector::remove(int).";;
		return;
	}
	if (size){
		arr[index] = arr[--size];
	}
	int halfAllocSize = allocSize / 2;
	if (size == halfAllocSize){
		reallocate(halfAllocSize);
	}
}

template <class T>
inline void Vector<T>::removeWithNoShuffle(int index){
	if (index >= size || index < 0){
		std::cerr << "Invalid index at Vector::removeWithNoShuffle(int).";
		return;
	}
	if (size--){
		for (int i = index; i < size; ++i){
			arr[i] = arr[i + 1];
		}
	}
	int halfAllocSize = allocSize / 2;
	if (size == halfAllocSize){
		reallocate(halfAllocSize);
	}
}

template <class T>
inline void Vector<T>::resize(int newSize){
	if (newSize < 0){
		std::cerr << "Can't have a negative size.";
		return;
	}
	int currSize = newSize < size ? newSize : size;
	T* buff = new T[newSize];
	for (int i = 0; i < currSize; ++i){
		buff[i] = arr[i];
	}
	delete[] arr;
	arr = buff;
	size = currSize;
	allocSize = newSize;
}

template <class T>
inline T& Vector<T>::last()const{
	if (!size){
		throw "No last element in empty array.\n";
	}
	return arr[size - 1];
}

template <class T>
inline int Vector<T>::getAllocatedSize() const{
	return allocSize;
}

template <class T>
inline int Vector<T>::getSize() const{
	return size;
}

template <class T>
inline void Vector<T>::print() const{
	for (int i = 0; i < size; ++i){
		std::cout << arr[i];
	}
}

template <class T>
inline void Vector<T>::reallocate(int newAllocSize){
	T* temp = new T[newAllocSize];
	for (int i = 0; i < size; ++i){
		temp[i] = arr[i];
	}
	delete[] arr;
	arr = temp;
	allocSize = newAllocSize;
}

template <class T>
inline void Vector<T>::clean(){
	delete[] arr;
	arr = 0;
	allocSize = 0;
	size = 0;
}

template <class T>
inline void Vector<T>::copyFrom(const Vector<T>& rhs){
	arr = new T[rhs.allocSize];
	for (int i = 0; i < rhs.size; ++i){
		arr[i] = rhs.arr[i];
	}
	size = rhs.size;
	allocSize = rhs.allocSize;
}

template <class T>
inline bool Vector<T>::isIn(const T& rhs) const{
	for (int i = 0; i < size; ++i){
		if (arr[i] == rhs){
			return true;
		}
	}
	return false;
}

template <class T>
inline int Vector<T>::indexOf(const T& rhs) const{
	if (!isIn(rhs)){
		return -1;
	}
	int index;
	for (int i = 0; i < size; ++i){
		if (arr[i] == rhs){
			index = i;
			break;
		}
	}
	return index;
}

template <class T>
inline T& Vector<T>::operator[](int index){
	if (index >= allocSize || index < 0){
		throw std::out_of_range("Invalid index at Vector::operator[].");
	}
	return arr[index];
}

template <class T>
inline const T& Vector<T>::operator[](int index) const{
	if (index >= allocSize || index < 0){
		throw std::out_of_range("Invalid index at Vector::operator[] const.");
	}
	return arr[index];
}