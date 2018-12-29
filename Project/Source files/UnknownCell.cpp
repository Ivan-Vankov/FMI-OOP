#include "UnknownCell.h"

UnknownCell::UnknownCell(const String& newContent) {
	content = newContent;
}

UnknownCell::~UnknownCell() {}

String UnknownCell::getContent() const {
	return content;
}

int UnknownCell::getLength() const {
	if (content.countWords()) {
		// 5 is the lenght of "ERROR"
		return 5;
	}
	else {
		return 0;
	}
}

String UnknownCell::getValue() const {
	return "0";
}

void UnknownCell::printCell() const {
	if (content.countWords()) {
		std::cout << "ERROR";
	}
}



Cell* UnknownCell::clone() const {
	return new UnknownCell(*this);
}