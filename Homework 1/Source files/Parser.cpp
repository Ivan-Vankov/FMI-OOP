#include "Parser.h"

bool Parser::parseAndExecute(Bookkeeper& bookkeeper, const String& input){
	try{
		int wordCount = input.countWords();
		// Check if the input is empty
		if (!wordCount){ return true; }
		// Get the command from <input>
		String command = input.wordNumber(1);
		if (!strcmp(command, "add-wallet")){
			addWallet(wordCount, bookkeeper, input);
			std::cout << "Wallet successfully added." << std::endl;
			return true;
		}
		if (!strcmp(command, "make-order")){
			makeOrder(wordCount, bookkeeper, input);
			std::cout << "Order successfully made." << std::endl;
			return true;
		}
		if (!strcmp(command, "wallet-info")){
			walletInfo(wordCount, bookkeeper, input);
			return true;
		}
		// attract-investors and quit are single word commands
		// so they are only executed when wordCount == 1
		if (wordCount == 1){
			if (!strcmp(command, "attract-investors")){
				bookkeeper.attractInvestors();
				return true;
			}
			if (!strcmp(command, "quit")){
				bookkeeper.saveToFile();
				std::cout << "All data saved." << std::endl;
				return false;
			}
		}
		std::cerr << "Unknown command." << std::endl;
		return true;
	}
	catch (std::exception&){
		throw;
	}
}

// add-wallet **fiatMoney** **name**
void Parser::addWallet(int wordCount, Bookkeeper& bookkeeper, const String& input){
	try{
		if (wordCount > 3){
			throw std::exception("Too many arguements for funcion <add-wallet>.");
		}
		if (wordCount == 2 || wordCount == 1){
			throw std::exception("Too few arguements for funcion <add-wallet>.");
		}
		String fiatMoneyStr = input.wordNumber(2);
		if (!isDouble(fiatMoneyStr)){
			throw std::exception("Second argument in function <add-wallet> must be a floating point number.");
		}
		String name = input.wordNumber(3);
		double fiatMoney = atof(fiatMoneyStr);
		bookkeeper.addWallet(fiatMoney, name);

	}
	catch (std::exception&){
		throw;
	}
}

// make-order **type** **fmiCoins** **walletId**
void Parser::makeOrder(int wordCount, Bookkeeper& bookkeeper, const String& input){
	try{
		if (wordCount > 4){
			throw std::exception("Too many arguements for funcion <make-order>.");
		}
		if (wordCount == 3 || wordCount == 2 || wordCount == 1){
			throw std::exception("Too few arguements for funcion <make-order>.");
		}
		String typeStr = input.wordNumber(2);
		if (!isOrderType(typeStr)){
			throw std::exception("Second argument in function <make-order> must me either SELL or BUY.");
		}
		String fmiCoinsStr = input.wordNumber(3);
		if (!isDouble(fmiCoinsStr)){
			throw std::exception("Third argument in function <make-order> must be a floating point number.");
		}
		String walletIDStr = input.wordNumber(4);
		if (!isUnsigned(walletIDStr)){
			throw std::exception("Fourth argument in function <make-order> must be an unsigned integer.");
		}
		OrderType type = toOrderType(typeStr);
		double fmiCoins = atof(fmiCoinsStr);
		unsigned walletID = (unsigned)atoi(walletIDStr);
		bookkeeper.makeOrder(type, fmiCoins, walletID);
	}
	catch (std::exception&){
		throw;
	}
}

// wallet-info **walletId**
void Parser::walletInfo(int wordCount, Bookkeeper& bookkeeper, const String& input){
	try{
		if (wordCount > 2){
			throw std::exception("Too many arguements for funcion <wallet-info>.");
		}
		if (wordCount == 1){
			throw std::exception("Too few arguements for funcion <wallet-info>.");
		}
		String walletIDStr = input.wordNumber(2);
		if (!isUnsigned(walletIDStr)){
			throw std::exception("Second argument in function <wallet-info> must be an unsigned integer.");
		}
		unsigned walletID = (unsigned)atoi(walletIDStr);
		bookkeeper.walletInfo(walletID);
	}
	catch (std::exception&){
		throw;
	}
}

bool Parser::isDouble(const String& str){
	int dotCounter = 0;
	for (int i = 0; i < str.getLength(); ++i){
		if (str[i] == '.'){
			++dotCounter;
		}
		if (str[0]<'0' || str[0]>'9'){
			return false;
		}
	}
	if (dotCounter>1){
		return false;
	}
	return true;
}

bool Parser::isOrderType(const String& order){
	if (!strcmp(order, "SELL") || !strcmp(order, "BUY")){
		return true;
	}
	return false;
}

bool Parser::isUnsigned(const String& str){
	if (str[0] == '-'){
		return false;
	}
	int length = str.getLength();
	if ((str[0] == '+' && length == 1) || (str[0]<'0' || str[0]>'9')){
		return false;
	}
	for (int i = 1; i < length; ++i){
		if (str[0]<'0' || str[0]>'9'){
			return false;
		}
	}
	return true;
}

OrderType Parser::toOrderType(const String& order){
	try{
		if (!strcmp(order, "SELL")){
			return SELL;
		}
		if (!strcmp(order, "BUY")){
			return BUY;
		}
		throw std::exception("Incorrect order type.");
		return SELL;
	}
	catch (std::exception&){
		throw;
	}
}