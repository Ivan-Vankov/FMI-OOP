#ifndef _POLYNOMIAL_HEADER_
#define _POLYNOMIAL_HEADER_
#include "Vector.h"
#include "Monomial.h"

template <typename T>
class Polynomial{
	friend class Iterator;
public:
	// Направете клас итератор, даващ възможност да се мине през всички не - нулеви едночлени на полинома, в нарастващ ред на степените им.За него предефинирайте операторните функции за :
	// Инкрементиране и декрементиране;
	// Сравнение(пълен набор);
	// Оператори за достъп до елемента(*и ->).
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

	//Оператори за еквивалентност;
	bool operator==(const Polynomial<T>&)const;
	bool operator!=(const Polynomial<T>&)const;

	//Оператори за сравнение(един полином е по - голям от друг, когато е от по - висока степен);
	bool operator>(const Polynomial<T>&)const;
	bool operator<(const Polynomial<T>&)const;
	bool operator>=(const Polynomial<T>&)const;
	bool operator<=(const Polynomial<T>&)const;

	//Бинарни оператори + , -, * с аргументи два полинома;
	Polynomial<T> operator+(const Polynomial<T>&)const;
	Polynomial<T>& operator+=(const Polynomial<T>&);
	Polynomial<T> operator-(const Polynomial<T>&)const;
	Polynomial<T>& operator-=(const Polynomial<T>&);

	Polynomial<T> operator*(const Polynomial<T>&)const;
	Polynomial<T>& operator*=(const Polynomial<T>&);

	//Бинарни оператори / и % с аргументи два полинома. Те извършват съответните действия, като деленето и остатъка имат семантиката на целочислено деление и остатък в полето на полиномите. Реализирайте и съответните версии с присвояване;

	Polynomial<T> operator/(const Polynomial<T>&)const;
	Polynomial<T>& operator/=(const Polynomial<T>&);
	Polynomial<T> operator%(const Polynomial<T>&)const;
	Polynomial<T>& operator%=(const Polynomial<T>&);

	//Бинарни оператори за умножение и деление с типа на коефициентите на полинома;
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

	//Оператор за индексиране, който връща коефициента пред съответната степен.Приемете, че всеки полином има безкраен брой коефициенти, краен брой от които са различни от 0;
	const T& operator[](int)const;

	//Оператор() с единствен аргумент X - от типа на коефициентите - пресмята стойността на полинома в точката X;
	T operator()(const T&)const;

	//Оператор() с два аргумента a и b - от типа на коефициентите. Пресмята определен интеграл от полинома в граници[a, b];
	T operator()(const T&, const T&)const;

	//Оператори за инкрементиране, които пресмятат интеграл от полинома(приемете константата за 0);
	Polynomial<T>& operator++();
	Polynomial<T> operator++(int);

	//Оператори за декрементиране, които пресмятат производна на полинома;
	Polynomial<T>& operator--();
	Polynomial<T> operator--(int);

	//Оператор за преобразуване към цяло число - връща степента на полинома;
	operator int()const;

	//Оператор за преобразуване към bool и прилежащ оператор !->проверява дали полином е нулев.
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

// Оператори за вход и изход в поток;
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