#ifndef _PARSER_HEADER_
#define _PARSER_HEADER_
#include "Bookkeeper.h"
#include "String.h"

class Parser{
public:
	// Returns false only if the input command is <quit>
	static bool parseAndExecute(Bookkeeper&, const String& input);
private:
	static void addWallet(int wordCount, Bookkeeper&, const String& input);
	static void makeOrder(int wordCount, Bookkeeper&, const String& input);
	static void walletInfo(int wordCount, Bookkeeper&, const String& input);

	static bool isDouble(const String&);
	static bool isOrderType(const String&);
	static bool isUnsigned(const String&);
	static OrderType toOrderType(const String&);
};

#endif