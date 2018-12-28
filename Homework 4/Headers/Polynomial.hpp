#include "Polynomial.h"

template <typename T>
Polynomial<T>::Iterator::Iterator(Polynomial<T>& polynom) :poly(&polynom), curr(0){}

template <typename T>
Polynomial<T>::Iterator::Iterator(Polynomial<T>& polynom, int index) : poly(&polynom), curr(index){}

template <typename T>
Polynomial<T>::Iterator::Iterator(Iterator& other) : poly(other.poly), curr(other.curr){}

template <typename T>
typename Polynomial<T>::Iterator Polynomial<T>::Iterator::operator++(int){
	Iterator temp(*this);
	++(*this);
	return temp;
}

template <typename T>
typename Polynomial<T>::Iterator& Polynomial<T>::Iterator::operator++(){
	if (curr < poly->numOfNonzeroDegrees()){
		++curr;
	}
	return *this;
}

template <typename T>
typename Polynomial<T>::Iterator Polynomial<T>::Iterator::operator--(int){
	Iterator temp(*this);
	--(*this);
	return temp;
}

template <typename T>
typename Polynomial<T>::Iterator& Polynomial<T>::Iterator::operator--(){
	if (curr > -1){
		--curr;
	}
	return *this;
}

template <typename T>
Monomial<T>& Polynomial<T>::Iterator::operator*(){
	return poly->polynom[curr];
}

template <typename T>
const Monomial<T>& Polynomial<T>::Iterator::operator*()const{
	return poly->polynom[curr];
}

template <typename T>
Monomial<T>* Polynomial<T>::Iterator::operator->(){
	return &poly->polynom[curr];
}

template <typename T>
const Monomial<T>* Polynomial<T>::Iterator::operator->()const{
	return &poly->polynom[curr];
}

template <typename T>
bool Polynomial<T>::Iterator::operator==(const Iterator& other)const{
	return poly == other.poly && curr == other.curr;
}

template <typename T>
bool Polynomial<T>::Iterator::operator!=(const Iterator& other)const{
	return !(other == *this);
}

template <typename T>
bool Polynomial<T>::Iterator::operator<(const Iterator& other)const{
	return poly == other.poly && curr < other.curr;
}

template <typename T>
bool Polynomial<T>::Iterator::operator>(const Iterator& other)const{
	return poly == other.poly && curr > other.curr;
}

template <typename T>
bool Polynomial<T>::Iterator::operator>=(const Iterator& other)const{
	return !(*this < other);
}

template <typename T>
bool Polynomial<T>::Iterator::operator<=(const Iterator& other) const{
	return !(*this > other);
}

template <typename T>
Polynomial<T>::const_Iterator::const_Iterator(const Polynomial<T>& polynom) :poly(&polynom), curr(0){}

template <typename T>
Polynomial<T>::const_Iterator::const_Iterator(const Polynomial<T>& polynom, int index) : poly(&polynom), curr(index){}

template <typename T>
Polynomial<T>::const_Iterator::const_Iterator(const_Iterator& other) : poly(other.poly), curr(other.curr){}

template <typename T>
typename Polynomial<T>::const_Iterator Polynomial<T>::const_Iterator::operator++(int){
	const_Iterator temp(*this);
	++(*this);
	return temp;
}

template <typename T>
typename Polynomial<T>::const_Iterator& Polynomial<T>::const_Iterator::operator++(){
	if (curr < poly->numOfNonzeroDegrees()){
		++curr;
	}
	return *this;
}

template <typename T>
typename Polynomial<T>::const_Iterator Polynomial<T>::const_Iterator::operator--(int){
	const_Iterator temp(*this);
	--(*this);
	return temp;
}

template <typename T>
typename Polynomial<T>::const_Iterator& Polynomial<T>::const_Iterator::operator--(){
	if (curr > -1){
		--curr;
	}
	return *this;
}

template <typename T>
const Monomial<T>& Polynomial<T>::const_Iterator::operator*()const{
	return poly->polynom[curr];
}

template <typename T>
const Monomial<T>* Polynomial<T>::const_Iterator::operator->()const{
	return &poly->polynom[curr];
}

template <typename T>
bool Polynomial<T>::const_Iterator::operator==(const const_Iterator& other)const{
	return poly == other.poly && curr == other.curr;
}

template <typename T>
bool Polynomial<T>::const_Iterator::operator!=(const const_Iterator& other)const{
	return !(other == *this);
}

template <typename T>
bool Polynomial<T>::const_Iterator::operator<(const const_Iterator& other)const{
	return poly == other.poly && curr < other.curr;
}

template <typename T>
bool Polynomial<T>::const_Iterator::operator>(const const_Iterator& other)const{
	return poly == other.poly && curr > other.curr;
}

template <typename T>
bool Polynomial<T>::const_Iterator::operator>=(const const_Iterator& other)const{
	return !(*this < other);
}

template <typename T>
bool Polynomial<T>::const_Iterator::operator<=(const const_Iterator& other) const{
	return !(*this > other);
}

template <typename T>
Polynomial<T>::Polynomial() :polynom(){}

template <typename T>
Polynomial<T>::Polynomial(const Polynomial<T>& other){
	copyFrom(other);
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T>& other){
	if (this != &other){
		polynom = other.polynom;
	}
	return *this;
}

template <typename T>
Polynomial<T>::~Polynomial(){
	// No dynamic data to delete
}

template <typename T>
Polynomial<T>& Polynomial<T>::nullify(){
	polynom.clean();
	return *this;
}

template <typename T>
bool Polynomial<T>::operator==(const Polynomial<T>& other)const{
	int polynomSize = polynom.getSize();
	if (polynomSize != other.polynom.getSize()){
		return false;
	}
	for (int i = 0; i < polynomSize - 1; ++i){
		if (polynom[i] != other.polynom[i]){
			return false;
		}
	}
	return true;
}

template <typename T>
bool Polynomial<T>::operator!=(const Polynomial<T>& other)const{
	return !((*this) == other);
}

template <typename T>
bool Polynomial<T>::operator>(const Polynomial<T>& other)const{
	if (!polynom.getSize()){
		if (!other.polynom.getSize()){
			return false;
		}
		return false;
	}
	if (!other.polynom.getSize()){
		return true;
	}
	// If the degree of the Polynomial<T> is larger than that of other
	if (polynom.last().getDegree() > other.polynom.last().getDegree()){
		return true;
	}
	// If it is smaller
	else if (polynom.last().getDegree() < other.polynom.last().getDegree()){
		return false;
	}
	// If the degrees are equal but the last coefficient is larger than the largest coefficient in other.polynom
	else if (polynom.last().getCoeff() > other.polynom.last().getCoeff()){
		return true;
	}
	// And if it is smaller or equal
	else{
		return false;
	}
}

template <typename T>
bool Polynomial<T>::operator<(const Polynomial<T>& other)const{
	if (!polynom.getSize()){
		if (!other.polynom.getSize()){
			return false;
		}
		return true;
	}
	if (!other.polynom.getSize()){
		return false;
	}
	// If the degree of the Polynomial<T> is smaller than that of other
	if (polynom.last().getDegree() < other.polynom.last().getDegree()){
		return true;
	}
	// If it is larger
	else if (polynom.last().getDegree() > other.polynom.last().getDegree()){
		return false;
	}
	// If the degrees are equal but the last coefficient is smaller than the largest coefficient in other.polynom
	else if (polynom.last().getCoeff() < other.polynom.last().getCoeff()){
		return true;
	}
	// And if it is larger or equal
	else{
		return false;
	}
}

template <typename T>
bool Polynomial<T>::operator>=(const Polynomial<T>& other)const{
	return !((*this) < other);
}

template <typename T>
bool Polynomial<T>::operator<=(const Polynomial<T>& other)const{
	return !((*this) > other);
}

template <typename T>
Polynomial<T> Polynomial<T>::operator*(const T& newCoeff)const{
	Polynomial<T> poly(*this);
	return poly *= newCoeff;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator/(const T& newCoeff)const{
	Polynomial<T> poly(*this);
	return poly /= newCoeff;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator*=(const T& newCoeff){
	if (newCoeff != 0){
		for (int i = 0; i < polynom.getSize(); ++i){
			polynom[i] *= newCoeff;
		}
	}
	else{
		nullify();
	}
	return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator/=(const T& newCoeff){
	// No divide by 0 check because the Monomial<T>s chech for it themselves
	for (int i = 0; i < polynom.getSize(); ++i){
		polynom[i] /= newCoeff;
	}
	return *this;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator*(const Monomial<T>& monom)const{
	Polynomial<T> poly(*this);
	return poly *= monom;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator/(const Monomial<T>& monom)const{
	Polynomial<T> poly(*this);
	return poly /= monom;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator*=(const Monomial<T>& monom){
	if (monom){
		for (int i = 0; i < polynom.getSize(); ++i){
			polynom[i] *= monom;
		}
	}
	else{
		nullify();
	}
	return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator/=(const Monomial<T>& monom){
	// No divide by the 0th Monomial<T> check because the Monomial<T>s chech for it themselves
	for (int i = 0; i < polynom.getSize(); ++i){
		polynom[i] /= monom;
	}
	return *this;
}

template <typename T>
const T& Polynomial<T>::operator[](int degree)const{
	if (degree < 0){
		throw std::exception("Invalix degree at Polynomial<T>::operator[].");
	}

	// If it is the == 0 Polynomial<T>
	if (!polynom.getSize()){
		return 0;
	}

	// If the degree is larger than the 
	if (degree > polynom.last().getDegree()){
		return 0;
	}

	int index;
	bool isIn = getIndexOf(degree, index);
	if (!isIn){
		throw std::exception("Invalix index at Polynomial<T>::operator[].");
	}

	return polynom[index].getCoeff();
}

template <typename T>
T Polynomial<T>::operator()(const T& x)const{
	T sum = 0;
	for (int i = 0; i < polynom.getSize(); ++i){
		sum += polynom[i].getCoeff() * fastPow(x, polynom[i].getDegree());
	}
	return sum;
}

template <typename T>
T Polynomial<T>::operator()(const T& x, const T& y)const{
	Polynomial<T> poly(*this);
	++poly;
	return poly(y) - poly(x);
}

template <typename T>
Polynomial<T> Polynomial<T>::operator+(const Monomial<T>& monom)const{
	Polynomial<T> poly(*this);
	return poly += monom;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator+=(const Monomial<T>& monom){
	int index;
	bool isIn = getIndexOf(monom.getDegree(), index);
	if (!isIn){
		polynom.add(index, monom);
	}
	else{
		// Increase the index-th Monomial<T>'s coefficient by monoms coefficient
		polynom[index].setCoeff(polynom[index].getCoeff() + monom.getCoeff());
		// If the coefficient becomes 0
		if (polynom[index].getCoeff() == 0){
			// remove it from polynom
			polynom.removeWithNoShuffle(index);
		}
	}
	return *this;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator-(const Monomial<T>& monom)const{
	Polynomial<T> poly(*this);
	return poly -= monom;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator-=(const Monomial<T>& monom){
	Monomial<T> temp(monom);
	temp *= -1;
	return (*this) += temp;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator+(const Polynomial<T>& poly)const{
	Polynomial<T> temp(*this);
	return temp += poly;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator+=(const Polynomial<T>& poly){
	for (int i = 0; i < poly.polynom.getSize(); ++i){
		(*this) += poly.polynom[i];
	}
	return *this;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator-(const Polynomial<T>& poly)const{
	Polynomial<T> temp(*this);
	return temp -= poly;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator-=(const Polynomial<T>& poly){
	for (int i = 0; i < poly.polynom.getSize(); ++i){
		(*this) -= poly.polynom[i];
	}
	return *this;
}

template <typename T>
// Doesn't use operator*= in order to save an unnecessary copy constructor
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T>& poly)const{
	Polynomial<T> res;
	for (int i = 0; i < polynom.getSize(); ++i){
		for (int j = 0; j < poly.polynom.getSize(); ++j){
			res += polynom[i] * poly.polynom[j];
		}
	}
	return res;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator*=(const Polynomial<T>& poly){
	Polynomial<T> res;
	for (int i = 0; i < polynom.getSize(); ++i){
		for (int j = 0; j < poly.polynom.getSize(); ++j){
			res += polynom[i] * poly.polynom[j];
		}
	}
	return (*this) = res;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator/(const Polynomial<T>& other)const{
	Polynomial<T> temp(*this);
	return temp /= other;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator/=(const Polynomial<T>& other){
	if (!other){
		throw std::exception("Can't divide by the empty Polynomial<T>.");
	}
	if ((*this) < other){
		return nullify();
	}

	Polynomial<T> result;
	Monomial<T> currMult;
	while ((int)(*this) >= (int)other){
		currMult = this->polynom.last() / other.polynom.last();
		result += currMult;
		(*this) -= other * currMult;
	}
	return (*this) = result;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator%(const Polynomial<T>& other)const{
	Polynomial<T> temp(*this);
	return temp %= other;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator%=(const Polynomial<T>& other){
	if (!other){
		throw std::exception("Can't divide by the empty Polynomial<T>.");
	}
	if ((*this) < other){
		return nullify();
	}

	Polynomial<T> result;
	Monomial<T> currMult;
	while ((int)(*this) >= (int)other){
		currMult = this->polynom.last() / other.polynom.last();
		result += currMult;
		(*this) -= other * currMult;
	}
	return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator++(){
	for (int i = 0; i < polynom.getSize(); ++i){
		++polynom[i];
	}
	return *this;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator++(int){
	Polynomial<T> temp(*this);
	++(*this);
	return temp;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator--(){
	if (!polynom.getSize()){
		return *this;
	}
	// If there is a constant in the polynomial
	int i = 0;
	if (!polynom[i].getDegree()){
		// we delete it
		polynom.removeWithNoShuffle(i);
	}
	for (; i < polynom.getSize(); ++i){
		--polynom[i];
	}
	return *this;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator--(int){
	Polynomial<T> temp(*this);
	--(*this);
	return temp;
}

template <typename T>
T Polynomial<T>::fastPow(const T& n, int exp)const{
	if (exp == 0 || exp == 1){
		return n;
	}
	if (exp % 2 == 0){
		// 3^4 == 9*2
		return fastPow(n*n, exp / 2);
	}
	else{
		// 3^5 == 3*9^2
		return n*fastPow(n*n, exp / 2);
	}
}

template <typename T>
Polynomial<T>::operator int()const{
	if (polynom.getSize()){
		return polynom.last().getDegree();
	}
	else{ return 0; }
}

template <typename T>
Polynomial<T>::operator bool()const{
	return polynom.getSize() != 0;
}

template <typename T>
bool Polynomial<T>::operator !()const{
	return !((bool)*this);
}

template <typename T>
bool Polynomial<T>::getIndexOf(int degree, int& closestIndex)const{
	//int lower = 0;
	//int upper = polynom.getSize() - 1;
	//int middle = 0;
	//while (upper >= lower){
	//	middle = (upper + lower) / 2;
	//	int currDegree = polynom[middle].getDegree();
	//	if (currDegree < degree){
	//		lower = middle + 1;
	//		continue;
	//	}
	//	if (currDegree > degree){
	//		upper = middle - 1;
	//		continue;
	//	}
	//	closestIndex = middle;
	//	return true;
	//}
	//closestIndex = middle;

	closestIndex = 0;
	for (int i = 0; i < polynom.getSize(); ++i, ++closestIndex){
		if (polynom[i].getDegree() > degree){
			return false;
		}
		if (polynom[i].getDegree() == degree){
			return true;
		}
	}

	return false;
}

template <typename T>
void Polynomial<T>::copyFrom(const Polynomial<T>& other){
	polynom = other.polynom;
}

template <typename T>
typename Polynomial<T>::Iterator Polynomial<T>::begin(){
	return Iterator(*this);
}

template <typename T>
typename Polynomial<T>::Iterator Polynomial<T>::end(){
	return Iterator(*this, polynom.getSize());
}

template <typename T>
typename Polynomial<T>::const_Iterator Polynomial<T>::cbegin()const{
	return const_Iterator(*this);
}

template <typename T>
typename Polynomial<T>::const_Iterator Polynomial<T>::cend()const{
	return const_Iterator(*this, polynom.getSize());
}

template <typename T>
int Polynomial<T>::numOfNonzeroDegrees()const{
	return polynom.getSize();
}

template <typename T>
static std::ostream& operator<<(std::ostream& os, const Polynomial<T>& poly){
	int nonzeroDegrees = poly.numOfNonzeroDegrees();
	if (!nonzeroDegrees){
		os << Monomial<T>();
	}
	else{
		Polynomial<T>::const_Iterator b = poly.cend();
		--b;
		Polynomial<T>::const_Iterator a = poly.cbegin();
		while (a != b){
			os << *a++;
			if ((*a).getCoeff() > 0){
				os << "+";
			}
		}
		os << *a;
	}
	return os;
}

template <typename T>
static std::istream& operator>>(std::istream& is, Polynomial<T>& poly){
	// 3
	// 0 1 0 3
	// x + 3x^3

	int degrees;
	is >> degrees;
	if (degrees < 0){
		throw std::exception("Can't have a negative polynomial degree.");
	}
	poly.nullify();
	int coeff;
	for (int i = 0; i < degrees; ++i){
		is >> coeff;
		if (coeff){
			poly += (Monomial<T>(i, coeff));
		}
	}

	return is;
}