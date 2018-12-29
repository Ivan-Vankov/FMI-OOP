#include "DateCell.h"

const Date DateCell::startDate = Date(01, 01, 1900);

DateCell::DateCell(const String& str) {
	if (!verifyData(str)) {
		throw std::exception("Data is not compatible with DateCell.");
	}
	int num1, num2, num3;
	getNums(str, num1, num2, num3);
	if (Date::verifyDate(num1, num2, num3)) {
		order = 0;
		toRegularForm(num1, num2, num3);
	}
	else {
		order = 1;
		toRegularForm(num3, num2, num1);
	}
}

DateCell::DateCell(const DateCell& other) {
	copyFrom(other);
}

DateCell& DateCell::operator=(const DateCell& other) {
	if (this != &other) {
		copyFrom(other);
	}
	return *this;
}

DateCell::~DateCell() {
	// No dynamic data
}

int DateCell::getLength() const {
	return content.getLength();
}

String DateCell::getContent() const {
	return content;
}

String DateCell::getValue() const {
	int dd, mm, yyyy;
	if (order == 0) {
		getNums(content, dd, mm, yyyy);
	}
	else {
		getNums(content, yyyy, mm, dd);
	}
	return std::to_string(Date(dd, mm, yyyy) - startDate);
}

void DateCell::printCell() const {
	std::cout << content;
}

Cell* DateCell::clone() const {
	return new DateCell(*this);
}

bool DateCell::verifyData(const String& content) {
	int dotCnt = 0;
	Vector<String> nums(3);
	for (int i = 0; i < content.getLength(); ++i) {
		if (content[i] == '.') {
			++dotCnt;
			if (dotCnt > 2) {
				return false;
			}
			continue;
		}
		if (content[i]<'0' || content[i]>'9') {
			return false;
		}
		nums[dotCnt].pushBack(content[i]);
	}
	int num1 = atoi(nums[0]);
	int num2 = atoi(nums[1]);
	int num3 = atoi(nums[2]);

	if (!(Date::verifyDate(num1, num2, num3) ||
		Date::verifyDate(num3, num2, num1))) {
		return false;
	}
	return true;
}

void DateCell::copyFrom(const DateCell& other) {
	order = other.order;
	content = other.content;
}

void DateCell::getNums(const String& str, int& num1, int& num2, int& num3) const {
	Vector<String> nums(3);
	int numIndex = 0;
	for (int i = 0; i < str.getLength(); ++i) {
		if (str[i] == '.') {
			++numIndex;
			continue;
		}
		nums[numIndex].pushBack(str[i]);
	}
	num1 = atoi(nums[0]);
	num2 = atoi(nums[1]);
	num3 = atoi(nums[2]);
}

void DateCell::toRegularForm(int dd, int mm, int yyyy) {
	if (order == 0) {
		if (dd < 10) {
			content + "0";
		}
		content + std::to_string(dd) + ".";
		if (mm < 10) {
			content + "0";
		}
		content + std::to_string(mm) + "." + std::to_string(yyyy);
	}
	else {
		content + std::to_string(yyyy) + ".";
		if (mm < 10) {
			content + "0";
		}
		content + std::to_string(mm) + ".";
		if (dd < 10) {
			content + "0";
		}
		content + std::to_string(dd);
	}
}