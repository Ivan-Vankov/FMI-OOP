#ifndef _STRING_HEADER_
#define _STRING_HEADER_

#include <iostream>

class String{
public:
	String();
	String(const char*);
	String(String const &);
	String& operator=(String const &);
	~String();
public:
	void free();
	void add(char);
	void remove(int);
	String& concatenate(const char*);
	void cutAfter(int);
public:
	int getAllocatedSize() const;
	int getLength() const;
	void print() const;
public:
	operator const char*()const;
	const char* cString() const;
	void setArray(const char*);
public:
	bool isIn(char)const;
public:
	int countWords()const;
private:
	void checklessStrcat(char* str1, const char*str2);
public:
	void removeUnnecessarySymbols(const String&);
public:
	void readLine(std::istream&);
public:
	operator const char*();
	char operator[](int) const;
	char& operator[](int);
	bool operator<(const String &) const;
	bool operator>(const String &) const;
	bool operator==(const String &) const;
	bool operator!=(const String &) const;
	String& operator+(const char*);
private:
	void reallocate(int NewSize);
	void copyFrom(const String &);
public:
	String wordNumber(int)const;
private:
	char* arr;
	int allocSize;
	int length;
};

static std::ostream& operator<<(std::ostream& os, const String& stuff){
	os << stuff.cString();
	return os;
}

static std::istream& operator>>(std::istream& is, String& stuff){
	String buff;
	String delims(" \t\n");
	char c;
	while (is.get(c)){
		if (!delims.isIn(c)){
			buff.add(c);
			break;
		}
	}
	while (is.get(c)){
		if (delims.isIn(c)){
			is.unget();
			break;
		}
		buff.add(c);
	}
	stuff = buff;
	return is;
}

#endif