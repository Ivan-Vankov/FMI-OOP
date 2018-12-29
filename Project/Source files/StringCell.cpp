#include "StringCell.h"

StringCell::StringCell(const StringCell& rhs) : content(rhs.content) {}

StringCell::StringCell(const String& rhs) {
	if (!verifyData(rhs)) {
		throw std::exception("Data is not compatible with StringCell.");
	}
	content = rhs;
}

StringCell& StringCell::operator=(const StringCell& rhs) {
	if (this != &rhs) {
		content = rhs.content;
	}
	return *this;
}

StringCell::~StringCell() {
	//no dynamic memory to delete
}

int StringCell::getLength() const {
	//return content.getLength();///////////////////moje da ima \t v content koeto da se broi za nqkolko space-a 
	int contentLength = strlen(content) - 1;
	String tempContent(content);//za da ne trie '\\' ot istinskiq content
	int charCounter = 0;
	for (int i = 1; i < contentLength; ++i) {
		if (tempContent[i] == '\\') {
			continue;
		}
		++charCounter;
	}
	return charCounter;
}

String StringCell::getContent() const {
	return content;
}

String StringCell::getValue() const {
	if (content.countWords() > 1) {
		return "0";
	}
	String allowedFirstSymbol("0123456789+-.");
	if (!allowedFirstSymbol.isIn(content[1])) {//proverqva pyrvi simvol(sled \")
		return "0";
	}
	int contentLength = content.getLength() - 1;//bez poslednoto \"
	int dotCounter = 0;
	int i = 1;//index za obhojdane na content
	if (content[0] == '+' || content[0] == '-') {
		++i;
	}
	for (; i < contentLength; ++i) {
		if (!((content[i] >= '0' && content[i] <= '9') || content[i] == '.')) {//proverqva vseki simvol dali e 0,1,..,9 ili .
			return "0";
		}
		if (content[i] == '.') {
			++dotCounter;
		}
	}
	if (dotCounter > 1) {
		return "0";
	}

	String value;
	int j = 1;
	// Skip the "+"
	if (content[j] == '+') {
		++i;
	}
	for (; j < contentLength; ++j) {
		value.pushBack(content[j]);
	}
	return value;
}

void StringCell::printCell() const {
	// -1 in order to skip the last \"
	int contentLength = strlen(content) - 1;
	// From 1 in order to skjip the first \"
	for (int i = 1; i < contentLength; ++i) {// ot 1 zashtoto pyrvoto \" se skipva
		if (content[i] == '\\') {
			++i;
		}
		std::cout << content[i];
	}
}

Cell* StringCell::clone() const {
	return new StringCell(*this);
}

bool StringCell::verifyData(const String& content) {
	int cellLength = content.getLength();
	if (cellLength == 1) {//  "  <- tozi sluchai
		return false;
	}
	if (!(content[0] == '\"' && content[cellLength - 1] == '\"')) {//ako ne zapochva ili ne zavyrshva na "
		return false;
	}
	int end = cellLength - 2;
	if (content[end] == '\\') {// "...\" sluchaq(zashtoto v obshtata proverka end+1 = '\"' i minava)
		return false;
	}
	String specSymbols = "\'\"";
	for (int start = 1; start <= end; ++start) {//ako ima samichko ",' ili \ nqkyde(<end zashtoto \ v [end] e provereno veche)
		if (specSymbols.isIn(content[start]) && content[start - 1] != '\\') {//za ",'
			return false;
		}
		if (content[start] == '\\' && content[start + 1] == '\\') {//ako ima pravilno "\\"
			++start;
			continue;//zashtoto nqma nujda ot sledvashtata proverka ako e veren tozi if
		}
		else if (content[start] == '\\' && !(specSymbols.isIn(content[start + 1]))) {//za '\'
			return false;
		}
	}
	return true;
}