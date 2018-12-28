#ifndef _PARSES_HEADER_
#define _PARSER_HEADER_
#include "FmiBook.h"

class Parser {
public:
	static bool parseAndExecute(const String& input, FmiBook&);

	static bool isUnsigned(const String&);
private:
	static bool wordAmmountCheck(int wordCount, int wordAmmount);

	static bool parseAddUser(const String& actor, const String& input, UserType type, int wordCount, FmiBook& fmiBook);
	static bool parseRemoveUser(const String& actor, const String& input, int wordCount, FmiBook& fmiBook);
	// true for block, false for unblock
	static bool parseBlocking(bool blockOrUnblock, const String& actor, const String& input, int wordCount, FmiBook& fmiBook);
	static bool parsePost(const String& actor, const String& input, int wordCount, FmiBook& fmiBook);
	static bool parseRemovePost(const String& actor, const String& input, int wordCount, FmiBook& fmiBook);
	static bool parseViewPost(const String& input, int wordCount, FmiBook& fmiBook);
	static bool parseViewAllPost(const String& input, int wordCount, FmiBook& fmiBook);
	static bool parseRename(const String& actor, const String& input, int wordCount, FmiBook& fmiBook);

	static bool toPostType(const String&, PostType& type);
};

#endif // _PARSER_HEADER_