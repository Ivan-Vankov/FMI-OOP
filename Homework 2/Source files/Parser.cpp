#include "Parser.h"

bool Parser::parseAndExecute(TextEditor& editor, const String& input) {
	int wordCount = input.countWords();
	// Do nothing if there are no words in the command
	if (!wordCount) {
		return true;
	}
	// Get the first word of the input
	String command = input.wordNumber(1);
	// Search if that word matches any of the know commands
	if (!strcmp(command, "makeHeading")) {
		makeHeading(editor, input, wordCount);
		return true;
	}
	if (!strcmp(command, "makeItalic")) {
		formatText(editor, input, italic, wordCount);
		return true;
	}
	if (!strcmp(command, "makeBold")) {
		formatText(editor, input, bold, wordCount);
		return true;
	}
	if (!strcmp(command, "makeCombine")) {
		formatText(editor, input, combine, wordCount);
		return true;
	}
	if (!strcmp(command, "addLine")) {
		addLine(editor, input, wordCount);
		return true;
	}
	if (!strcmp(command, "removeLine")) {
		removeLine(editor, input, wordCount);
		return true;
	}
	if (!strcmp(command, "exit")) {
		if (wordCount == 1) {
			editor.saveAsMarkdown();
			std::cout << "Successfully saved the file in markdown format." << std::endl;
			return false;
		}
	}
	std::cerr << "Unknown command." << std::endl;
	return true;
}

void Parser::makeHeading(TextEditor& editor, const String& input, int wordCount) {
	// Check if there number of words in the input is not valid
	if (!wordAmmountCheck(wordCount, 2)) { return; }

	String line;
	int lineNumber;
	// Check if the second word is indeed a number and get that number
	if (!checkLine(input, line, lineNumber)) { return; }
	if (!editor.makeHeading(lineNumber)) { return; }
	std::cout << "Succesfully made line number " << lineNumber << " into a heading." << std::endl;
}

void Parser::formatText(TextEditor& editor, const String& input, FormatType format, int wordCount) {
	// Check if there number of words in the input is not valid
	if (!wordAmmountCheck(wordCount, 4)) { return; }

	String line;
	int lineNumber;
	// Check if the second word is indeed a number and get that number
	if (!checkLine(input, line, lineNumber)) { return; }
	String from(input.wordNumber(3));
	if (!isUnsigned(from)) {
		std::cerr << "Third word in formatText command should be an unsigned integer" << std::endl;
		return;
	}
	String to(input.wordNumber(4));
	if (!isUnsigned(to)) {
		std::cerr << "Fourth word in formatText command should be an unsigned integer" << std::endl;
		return;
	}
	int fromNumber = atoi(from);
	int toNumber = atoi(to);
	if (!editor.formatText(lineNumber, fromNumber, toNumber, format)) { return; }
	std::cout << "Successfully formated the text." << std::endl;
}

void Parser::addLine(TextEditor& editor, const String& input, int wordCount) {
	if (wordCount < 2) {
		std::cerr << "Not enough words in addLine command." << std::endl;
		return;
	}
	String newWord;
	// Skip the initial "addline " and copy everything else in the input
	for (int i = 8; i < input.getLength(); ++i) {
		newWord.pushBack(input[i]);
	}
	if (!editor.addLine(newWord)) { return; }
	std::cout << "Successfully added the line." << std::endl;
}

void Parser::removeLine(TextEditor& editor, const String& input, int wordCount) {
	// Check if there number of words in the input is not valid
	if (!wordAmmountCheck(wordCount, 2)) { return; }

	String line;
	int lineNumber;
	// Check if the second word is indeed a number and get that number
	if (!checkLine(input, line, lineNumber)) { return; }
	if (!editor.removeLine(lineNumber)) { return; }
	std::cout << "Succesfully removed line number " << lineNumber << "." << std::endl;
}

bool Parser::checkLine(const String& input, String& line, int& lineNumber) {
	line = input.wordNumber(2);
	if (!isUnsigned(line)) {
		std::cerr << "Second word should be an unsigned integer." << std::endl;
		return false;
	}
	lineNumber = atoi(line);
	return true;
}

bool Parser::wordAmmountCheck(int wordCount, int wordAmmount) {
	if (wordCount < wordAmmount) {
		std::cerr << "Not enough words in command." << std::endl;
		return false;
	}
	if (wordCount > wordAmmount) {
		std::cerr << "Too many words in command." << std::endl;
		return false;
	}
	return true;
}

bool Parser::isUnsigned(const String& str) {
	if (str[0] == '-') {
		return false;
	}
	int length = str.getLength();
	if ((str[0] == '+' && length == 1) || (str[0]<'0' || str[0]>'9')) {
		return false;
	}
	for (int i = 1; i < length; ++i) {
		if (str[0]<'0' || str[0]>'9') {
			return false;
		}
	}
	return true;
}