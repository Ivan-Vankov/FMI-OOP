#include "Polynomial.h"

int main(){
	Polynomial<double> test1;
	Monomial<double> a(3,5);
	Monomial<double> b(2,4);
	test1 += a;
	test1 += b;

	Polynomial<double> test2;
	Monomial<double> c(1, 3);
	Monomial<double> d(2, 2);
	test2 += c;
	test2 += d;

	std::cout << "Polynomial test1: " << test1 << std::endl;
	std::cout << "Polynomial test2: " << test2 << std::endl;

	std::cout << "\nIterator through test1" << std::endl;
	for (Polynomial<double>::Iterator a = test1.begin(); a != test1.end(); ++a){
		std::cout << *a << std::endl;
	}


	std::cout << "\ntest1 + test2" << std::endl;
	std::cout << test1 + test2 << std::endl;

	std::cout << "\ntest1 - test2" << std::endl;
	std::cout << test1 - test2 << std::endl;

	std::cout << "\ntest1 < test2" << std::endl;
	std::cout << (test1 < test2) << std::endl;

	std::cout << "\ntest1 > test2" << std::endl;
	std::cout << (test1 > test2) << std::endl;

	std::cout << "\ntest1 == test2" << std::endl;
	std::cout << (test1 == test2) << std::endl;

	std::cout << "\n(int)test1" << std::endl;
	std::cout << (int)test1 << std::endl;

	std::cout << "\n(bool)test1" << std::endl;
	std::cout << (bool)test1 << std::endl;

	std::cout << "\n!test1" << std::endl;
	std::cout << !test1 << std::endl;

	std::cout << "\ntest1 * test2" << std::endl;
	std::cout << test1 * test2 << std::endl;

	Polynomial<double> test3;
	Polynomial<double> test4;

	std::cout << "\ntest3 = test1 / test2" << std::endl;
	std::cout << (test3 = test1 / test2) << std::endl;

	std::cout << "\ntest4 = test1 % test2" << std::endl;
	std::cout << (test4 = test1 % test2) << std::endl;

	std::cout << "\ntest1 == test2*test3 + test4" << std::endl;
	std::cout << (test1 == test2*test3 + test4) << std::endl;

	std::cout << "\ntest1(3)" << std::endl;
	std::cout << test1(3) << std::endl;

	std::cout << "\ntest1(3,4)" << std::endl;
	std::cout << test1(3,4) << std::endl;

	std::cout << "\n++test1" << std::endl;
	std::cout << ++test1 << std::endl;

	std::cout << "\n--test1" << std::endl;
	std::cout << --test1 << std::endl;

	return 0;
}