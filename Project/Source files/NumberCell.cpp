#include "NumberCell.h"

NumberCell::NumberCell(const String& rhs) {
	if (!verifyData(rhs)) {
		throw std::exception("Data is not compatible with NumberCell.");
	}
	content = toRegularForm(rhs);
}

NumberCell::NumberCell(const NumberCell& rhs) {
	content = rhs.getContent();
}

NumberCell& NumberCell::operator=(const NumberCell& rhs) {
	if (this != &rhs) {
		content = rhs.getContent();
	}
	return *this;
}

NumberCell::~NumberCell() {
	// no dynamic memory to delete
}

int NumberCell::getLength() const {
	return content.getLength();
}

String NumberCell::getContent() const {
	return content;
}

String NumberCell::getValue() const {
	if (content[0] == '+') {
		String noPlus;
		int contentLength = content.getLength();
		for (int i = 1; i < contentLength; ++i) {
			noPlus.pushBack(content[i]);
		}
		return noPlus;
	}
	else {
		return content;
	}
}

void NumberCell::printCell() const {
	std::cout << content;
}

bool NumberCell::verifyData(const String& content) {
	if (content.countWords() > 1) {
		return false;
	}
	String allowedFirstSymbol("0123456789+-.");
	if (!allowedFirstSymbol.isIn(content[0])) {//proverqva pyrvi simvol
		return false;
	}
	int cellLength = content.getLength();
	int dotCounter = 0;
	int i = 0;//index za obhojdane na cellContent
	if (content[0] == '+' || content[0] == '-') {
		++i;
	}
	for (; i < cellLength; ++i) {
		if (!((content[i] >= '0' && content[i] <= '9') || content[i] == '.')) {//proverqva vseki simvol dali e 0,1,..,9 ili .
			return false;
		}
		if (content[i] == '.') {
			++dotCounter;
		}
	}
	if (dotCounter > 1) {
		return false;
	}
	return true;
}

String NumberCell::toRegularForm(const String& str) const {
	String buff;
	int i = 0;
	while (str[i] == '0'&& i + 1 < str.getLength()) { ++i; }
	while (i < str.getLength()) { buff.pushBack(str[i++]); }
	return buff;
}