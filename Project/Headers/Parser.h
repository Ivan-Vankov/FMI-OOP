#ifndef _PARSER_HEADER_
#define _PARSER_HEADER_
#include "Tables.h"

class Parser {
public:
	// Only returns false on the exit command
	static bool parseAndExecute(Tables& openedFiles, const String& input);
private:
	static void print(Tables& openedFiles, const String& input, int wordCount);
	static void editcell(Tables& openedFiles, const String& input, int wordCount);
	static void open(Tables& openedFiles, const String& input, int wordCount);
	static void close(Tables& openedFiles, const String& input, int wordCount);
	static void save(Tables& openedFiles, const String& input, int wordCount);
	static void saveas(Tables& openedFiles, const String& input, int wordCount);

	// For this check we asume that the line is the second word of the input
	static bool checkLine(const String& input, String& line, int& lineNumber);
	static bool wordAmmountCheck(int wordCount, int wordAmmount);
	static bool isInteger(const String&);
	static bool isOrder(const String&);
	static Order toOrder(const String&);

	static bool fileExists(const String&);
	static String toPath(const String& path);

	static bool hasExtentionTxt(const String& path);
};

#endif // _PARSER_HEADER_