#include "Monomial.h"

template <typename T>
Monomial<T>::Monomial() :degree(0), coeff(0){}

template <typename T>
Monomial<T>::Monomial(int newDeg, const T& newCoeff) : degree(newDeg), coeff(newCoeff){}

template <typename T>
Monomial<T>::Monomial(const Monomial<T>& other){
	copyFrom(other);
}

template <typename T>
Monomial<T>& Monomial<T>::operator=(const Monomial<T>& other){
	if (this != &other){
		copyFrom(other);
	}
	return *this;
}

template <typename T>
Monomial<T>::~Monomial(){
	// No dynamic data to delete
}

template <typename T>
Monomial<T> Monomial<T>::operator*(const T& newCoeff)const{
	Monomial<T> temp(*this);
	return temp *= newCoeff;
}

template <typename T>
Monomial<T> Monomial<T>::operator/(const T& newCoeff)const{
	Monomial<T> temp(*this);
	return temp /= newCoeff;
}

template <typename T>
Monomial<T>& Monomial<T>::operator*=(const T& newCoeff){
	coeff *= newCoeff;
	return *this;
}

template <typename T>
Monomial<T>& Monomial<T>::operator/=(const T& newCoeff){
	if (newCoeff < eps && newCoeff > -eps){
		throw std::exception("Can't divide by 0.");
	}
	coeff /= newCoeff;
	return *this;
}

template <typename T>
Monomial<T> Monomial<T>::operator*(const Monomial<T>& monom)const{
	Monomial<T> temp(*this);
	return temp *= monom;
}

template <typename T>
Monomial<T>& Monomial<T>::operator*=(const Monomial<T>& monom){
	coeff *= monom.coeff;
	degree += monom.degree;
	return *this;
}

template <typename T>
Monomial<T> Monomial<T>::operator/(const Monomial<T>& monom)const{
	Monomial<T> temp(*this);
	return temp /= monom;
}

template <typename T>
Monomial<T>& Monomial<T>::operator/=(const Monomial<T>& monom){
	if (!monom){
		throw std::exception("Can't divide by the zero Monomial<T>.");
	}
	if (degree < monom.degree){
		throw std::exception("Can't have a Monomial<T> with a negative degree.");
	}
	coeff /= monom.coeff;
	degree -= monom.degree;
	return *this;
}

template <typename T>
bool Monomial<T>::operator==(const Monomial<T>& other)const{
	return degree == other.degree && coeff == other.coeff;
}

template <typename T>
bool Monomial<T>::operator!=(const Monomial<T>& other)const{
	return !((*this) == other);
}

template <typename T>
bool Monomial<T>::operator>(const Monomial<T>& other)const{
	if (degree > other.degree){
		return true;
	}
	if (degree == other.degree && coeff > other.coeff){
		return true;
	}
	return false;
}

template <typename T>
bool Monomial<T>::operator<(const Monomial<T>& other)const{
	if (degree < other.degree){
		return true;
	}
	if (degree == other.degree && coeff < other.coeff){
		return true;
	}
	return false;
}

template <typename T>
bool Monomial<T>::operator>=(const Monomial<T>& other)const{
	return !(*this < other);
}

template <typename T>
bool Monomial<T>::operator<=(const Monomial<T>& other)const{
	return !(*this > other);
}

template <typename T>
Monomial<T>& Monomial<T>::operator++(){
	// 1 -> x/2
	// 4x^3 -> x^4
	++degree;
	coeff /= degree;
	return *this;
}

template <typename T>
Monomial<T> Monomial<T>::operator++(int){
	Monomial<T> temp(*this);
	++(*this);
	return temp;
}

template <typename T>
Monomial<T>& Monomial<T>::operator--(){
	// 3x^5 -> 15x^4
	// 5x^0 -> 0x^0
	if (degree == 0){
		coeff = 0;
	}
	else{
		coeff *= degree;
		--degree;
	}
	return *this;
}

template <typename T>
Monomial<T> Monomial<T>::operator--(int){
	Monomial<T> temp(*this);
	--(*this);
	return temp;
}

template <typename T>
Monomial<T>::operator bool()const{
	return coeff != 0;
}

template <typename T>
int Monomial<T>::getDegree()const{
	return degree;
}

template <typename T>
const T& Monomial<T>::getCoeff()const{
	return coeff;
}

template <typename T>
void Monomial<T>::copyFrom(const Monomial<T>& other){
	coeff = other.coeff;
	degree = other.degree;
}

template <typename T>
void Monomial<T>::setDegree(int newDegree){
	if (newDegree < 0){
		throw std::exception("Can't have a Monomial<T> wit a negative degree.");
	}
	degree = newDegree;
}

template <typename T>
void Monomial<T>::setCoeff(const T& newCoeff){
	coeff = newCoeff;
}