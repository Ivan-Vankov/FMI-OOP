#ifndef _PARSER_HEADER_
#define _PARSER_HEADER_
#include "TextEditor.h"

class Parser {
public:
	// Only returns false on the exit command
	static bool parseAndExecute(TextEditor&, const String& input);
private:
	static void makeHeading(TextEditor&, const String& input, int wordCount);
	static void formatText(TextEditor&, const String& input, FormatType format, int wordCount);
	static void addLine(TextEditor&, const String& input, int wordCount);
	static void removeLine(TextEditor&, const String& input, int wordCount);
	// For this check we asume that the line is the second word of the input
	static bool checkLine(const String& input, String& line, int& lineNumber);
	static bool wordAmmountCheck(int wordCount, int wordAmmount);
	static bool isUnsigned(const String&);
};

#endif