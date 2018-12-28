#ifndef _MONOMIAL_HEADER_
#define _MONOMIAL_HEADER_
#include <exception>
#include <iostream>

#define eps 1e-6

template <typename T>
class Monomial{
public:
	Monomial();
	Monomial(int newDeg, const T& newCoeff);
	Monomial<T>(const Monomial<T>&);
	Monomial<T>& operator=(const Monomial<T>&);
	~Monomial();

	Monomial<T> operator*(const T&)const;
	Monomial<T> operator/(const T&)const;
	Monomial<T>& operator*=(const T&);
	Monomial<T>& operator/=(const T&);

	Monomial<T> operator*(const Monomial<T>&)const;
	Monomial<T>& operator*=(const Monomial<T>&);
	Monomial<T> operator/(const Monomial<T>&)const;
	Monomial<T>& operator/=(const Monomial<T>&);

	bool operator==(const Monomial<T>&)const;
	bool operator!=(const Monomial<T>&)const;

	bool operator>(const Monomial<T>&)const;
	bool operator<(const Monomial<T>&)const;
	bool operator>=(const Monomial<T>&)const;
	bool operator<=(const Monomial<T>&)const;

	Monomial<T>& operator++();
	Monomial<T> operator++(int);
	Monomial<T>& operator--();
	Monomial<T> operator--(int);

	operator bool()const;

	int getDegree()const;
	const T& getCoeff()const;

	void setDegree(int);
	void setCoeff(const T&);

private:
	void copyFrom(const Monomial<T>&);

	int degree;
	T coeff;
};

template <typename T>
static std::ostream& operator<<(std::ostream& os, const Monomial<T>& monom){
	if (monom.getCoeff() == 0){
		return os;
	}
	if (monom.getCoeff() != 1 || monom.getDegree() == 0){
		os << monom.getCoeff();
	}
	if (monom.getDegree() == 0){
		return os;
	}
	os << "x";
	if (monom.getDegree() != 1){
		os << "^" << monom.getDegree();
	}
	return os;
}

template <typename T>
static std::istream& operator>>(std::istream& is, Monomial<T>& monom){
	int degree;
	double coeff;
	is >> degree >> coeff;
	if (degree < 0){
		throw std::exception("Can't have a negative monomial degree.");
	}
	monom.setDegree(degree);
	monom.setCoeff(coeff);
	return is;
}

#include "Monomial.hpp"
#endif // _Monomial<T>_HEADER_