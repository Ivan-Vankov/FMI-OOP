#ifndef _WALLET_HEADER_
#define _WALLET_HEADER_
//#include "String.h"
#include <iostream>

class Wallet{
public:
	Wallet();
	Wallet(/*double newFiatMoney, */const /*String&*/ char*newOwner);
	Wallet(const Wallet&);
	Wallet& operator=(const Wallet&);
	~Wallet();
private:
	void copyFrom(const Wallet&);

	char owner[256];
	//unsigned id;
	//double fiatMoney;
	//String owner;
};

#endif