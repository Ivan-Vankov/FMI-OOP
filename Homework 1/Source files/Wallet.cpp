#include "Wallet.h"

Wallet::Wallet(){
	strcpy_s(owner, 256, "Unknown");
}

Wallet::Wallet(/*double newFiatMoney, */const /*String&*/ char* newOwner){
	strcpy_s(owner, 256, newOwner);
}

Wallet::Wallet(const Wallet& other){
	copyFrom(other);
}

Wallet& Wallet::operator=(const Wallet& other){
	if (this != &other){
		// No dynamic data => no need to delete anything
		copyFrom(other);
	}
	return *this;
}

Wallet::~Wallet(){
	// No dynamic data => no need to delete anything
}

void Wallet::copyFrom(const Wallet& other){
	strcpy_s(owner, 256, other.owner);
}