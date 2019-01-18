#ifndef _STRING_HEADER_
#define _STRING_HEADER_
#include <iostream>

class String {
public:
	String();
	String(int allocAmmount);
	String(const char*);
	String(const String&);
	String& operator=(const String&);
	~String();

	void free();
	void pushBack(char);
	// WARNIGN: O(n)
	void pushFront(char);
	void remove(int);
	String& concatenate(const char*);
	String& concatenate(const String&);
	void cutAfter(int);
	void resize(int);

	int getAllocatedSize() const;
	int getLength() const;
	void print() const;

	operator const char*()const;
	const char* cString() const;
	void setArray(const char*);

	bool isIn(char)const;

	int countWords()const;

	void removeUnnecessarySymbols(const String&);

	void readLine(std::istream&);

	char operator[](int) const;
	char& operator[](int);
	bool operator<(const String &) const;
	bool operator>(const String &) const;
	bool operator==(const String &) const;
	bool operator!=(const String &) const;
	String& operator+(const char*);
	String& operator+(const String &);

	String wordNumber(int)const;
	String strToken()const;

private:
	void reallocate(int NewSize);
	// Check for sufficient size for pushBack/Front methods
	void sizeCheck();
	void copyFrom(const String &);

	void checklessStrcat(char* str1, const char*str2);

	char* arr;
	int allocSize;
	int length;
};

static std::ostream& operator<<(std::ostream& os, const String& stuff) {
	os << stuff.cString();
	return os;
}

static std::istream& operator>>(std::istream& is, String& stuff) {
	String buff;
	String delims(" \t\n");
	char c;
	while (is.get(c)) {
		if (!delims.isIn(c)) {
			buff.pushBack(c);
			break;
		}
	}
	while (is.get(c)) {
		if (delims.isIn(c)) {
			is.unget();
			break;
		}
		buff.pushBack(c);
	}
	stuff = buff;
	return is;
}

#endif