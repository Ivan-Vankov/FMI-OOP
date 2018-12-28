#ifndef _POLYNOMIAL_HEADER_
#define _POLYNOMIAL_HEADER_
#include "Vector.h"
#include "Monomial.h"

template <typename T>
class Polynomial{
	friend class Iterator;
public:
	// ��������� ���� ��������, ����� ���������� �� �� ���� ���� ������ �� - ������ ��������� �� ��������, � ��������� ��� �� ��������� ��.�� ���� �������������� ������������ ������� �� :
	// �������������� � ��������������;
	// ���������(����� �����);
	// ��������� �� ������ �� ��������(*� ->).
	class Iterator{
		Polynomial<T>* poly;
		int curr;
	public:
		Iterator(Polynomial<T>&);
		Iterator(Polynomial<T>&, int index);
		Iterator(Iterator&);
		//Iterator(const Iterator&);
		//Iterator& operator=(const Iterator&);
		~Iterator(){}

		Iterator operator++(int);
		Iterator& operator++();
		Iterator operator--(int);
		Iterator& operator--();

		Monomial<T>& operator*();
		const Monomial<T>& operator*()const;
		Monomial<T>* operator->();
		const Monomial<T>* operator->()const;

		bool operator==(const Iterator& other)const;
		bool operator!=(const Iterator& other)const;

		bool operator<(const Iterator& other)const;
		bool operator>(const Iterator& other)const;
		bool operator>=(const Iterator& other)const;
		bool operator<=(const Iterator& other) const;
	};
	class const_Iterator{
		const Polynomial<T>* poly;
		int curr;
	public:
		const_Iterator(const Polynomial<T>&);
		const_Iterator(const Polynomial<T>&, int index);
		const_Iterator(const_Iterator&);
		//const_Iterator(const const_Iterator&);
		//const_Iterator& operator=(const const_Iterator&);
		~const_Iterator(){}

		const_Iterator operator++(int);
		const_Iterator& operator++();
		const_Iterator operator--(int);
		const_Iterator& operator--();

		const Monomial<T>& operator*()const;
		const Monomial<T>* operator->()const;

		bool operator==(const const_Iterator& other)const;
		bool operator!=(const const_Iterator& other)const;

		bool operator<(const const_Iterator& other)const;
		bool operator>(const const_Iterator& other)const;
		bool operator>=(const const_Iterator& other)const;
		bool operator<=(const const_Iterator& other) const;
	};

	Polynomial();
	Polynomial(const Polynomial<T>&);
	Polynomial<T>& operator=(const Polynomial<T>&);
	~Polynomial();

	Polynomial<T>& nullify();

	//��������� �� ��������������;
	bool operator==(const Polynomial<T>&)const;
	bool operator!=(const Polynomial<T>&)const;

	//��������� �� ���������(���� ������� � �� - ����� �� ����, ������ � �� �� - ������ ������);
	bool operator>(const Polynomial<T>&)const;
	bool operator<(const Polynomial<T>&)const;
	bool operator>=(const Polynomial<T>&)const;
	bool operator<=(const Polynomial<T>&)const;

	//������� ��������� + , -, * � ��������� ��� ��������;
	Polynomial<T> operator+(const Polynomial<T>&)const;
	Polynomial<T>& operator+=(const Polynomial<T>&);
	Polynomial<T> operator-(const Polynomial<T>&)const;
	Polynomial<T>& operator-=(const Polynomial<T>&);

	Polynomial<T> operator*(const Polynomial<T>&)const;
	Polynomial<T>& operator*=(const Polynomial<T>&);

	//������� ��������� / � % � ��������� ��� ��������. �� ��������� ����������� ��������, ���� �������� � �������� ���� ����������� �� ����������� ������� � ������� � ������ �� ����������. ������������ � ����������� ������ � �����������;

	Polynomial<T> operator/(const Polynomial<T>&)const;
	Polynomial<T>& operator/=(const Polynomial<T>&);
	Polynomial<T> operator%(const Polynomial<T>&)const;
	Polynomial<T>& operator%=(const Polynomial<T>&);

	//������� ��������� �� ��������� � ������� � ���� �� ������������� �� ��������;
	Polynomial<T> operator*(const T&)const;
	Polynomial<T> operator/(const T&)const;
	Polynomial<T>& operator*=(const T&);
	Polynomial<T>& operator/=(const T&);

	Polynomial<T> operator*(const Monomial<T>&)const;
	Polynomial<T> operator/(const Monomial<T>&)const;
	Polynomial<T>& operator*=(const Monomial<T>&);
	Polynomial<T>& operator/=(const Monomial<T>&);

	Polynomial<T> operator+(const Monomial<T>&)const;
	Polynomial<T>& operator+=(const Monomial<T>&);
	Polynomial<T> operator-(const Monomial<T>&)const;
	Polynomial<T>& operator-=(const Monomial<T>&);

	//�������� �� �����������, ����� ����� ����������� ���� ����������� ������.��������, �� ����� ������� ��� �������� ���� �����������, ����� ���� �� ����� �� �������� �� 0;
	const T& operator[](int)const;

	//��������() � ��������� �������� X - �� ���� �� ������������� - �������� ���������� �� �������� � ������� X;
	T operator()(const T&)const;

	//��������() � ��� ��������� a � b - �� ���� �� �������������. �������� ��������� �������� �� �������� � �������[a, b];
	T operator()(const T&, const T&)const;

	//��������� �� ��������������, ����� ��������� �������� �� ��������(�������� ����������� �� 0);
	Polynomial<T>& operator++();
	Polynomial<T> operator++(int);

	//��������� �� ��������������, ����� ��������� ���������� �� ��������;
	Polynomial<T>& operator--();
	Polynomial<T> operator--(int);

	//�������� �� ������������� ��� ���� ����� - ����� �������� �� ��������;
	operator int()const;

	//�������� �� ������������� ��� bool � �������� �������� !->��������� ���� ������� � �����.
	operator bool()const;
	bool operator !()const;

	Iterator begin();
	Iterator end();
	const_Iterator cbegin()const;
	const_Iterator cend()const;

	int numOfNonzeroDegrees()const;

private:
	void copyFrom(const Polynomial<T>&);

	// We use the closest index when we search for a Monomial<T> in polynom 
	// when polynom doesn't have a Monomial<T> with that degree
	bool getIndexOf(int degree, int& closestIndex)const;

	T fastPow(const T& n, int exp)const;

	Vector<Monomial<T>> polynom;
};

// ��������� �� ���� � ����� � �����;
template <typename T>
static std::ostream& operator<<(std::ostream& os, const Polynomial<T>& poly);

template <typename T>
static std::istream& operator>>(std::istream& is, Polynomial<T>& poly);

template <typename T>
static Polynomial<T> operator+(const Monomial<T>& monom, const Polynomial<T>& poly){
	return poly + monom;
}

template <typename T>
static Polynomial<T> operator-(const Monomial<T>& monom, const Polynomial<T>& poly){
	return poly - monom;
}

#include "Polynomial.hpp"
#endif //_POLYNOMIAL_HEADER_