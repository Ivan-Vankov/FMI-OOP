#include <iostream>
#include "String.h"

String::String(){
	arr = new char;
	arr[0] = '\0';
	allocSize = 1;
	length = 0;
}

String::String(const char* rhs){
	int len = strlen(rhs);
	arr = new char[len + 1];
	checklessStrcat(arr, rhs);
	allocSize = len + 1;
	length = len;
}

String::String(String const& other){
	copyFrom(other);
}

String& String::operator=(String const& other){
	if (this != &other){
		free();
		copyFrom(other);
	}
	return *this;
}


String::~String(){
	delete[] arr;
	arr = NULL;
	allocSize = 0;
	length = 0;
}


void String::checklessStrcat(char* str1, const char*str2){
	int i = 0;
	do{
		str1[i] = str2[i];
	} while (str2[i++]);
}


void String::removeUnnecessarySymbols(const String& delims){
	String buffer;
	int i = 0;
	int wordCount = countWords();
	int wordCounter = 0;
	do{
		while (arr[i] && delims.isIn(arr[i])){
			++i;
		}
		while (arr[i] && !delims.isIn(arr[i])){
			buffer.add(arr[i++]);
		}
		if (++wordCounter < wordCount){
			buffer.add(' ');
		}
	} while (arr[i++]);
	delete[] arr;
	arr = NULL;
	*this = buffer;
}

int String::countWords()const{
	int cnt = 0;
	String delims("\t ");
	int i = 0;
	while (arr[i]){
		while (arr[i] && delims.isIn(arr[i])){
			++i;
		}
		if (arr[i] && !delims.isIn(arr[i])){
			++cnt;
			++i;
		}
		while (arr[i] && !delims.isIn(arr[i])){
			++i;
		}
	}
	return cnt;
}

bool String::isIn(char elem)const{
	int i = 0;
	while (arr[i]){
		if (arr[i++] == elem){
			return true;
		}
	}
	return false;
}

void String::copyFrom(String const & other){
	arr = new char[other.allocSize];
	checklessStrcat(arr, other);
	allocSize = other.allocSize;
	length = other.length;
}

void String::readLine(std::istream& is){
	char c;
	while (is.get(c) && c != '\n'){
		if ((length + 1) >= allocSize){
			reallocate(allocSize ? allocSize * 2 : 2);
		}
		arr[length++] = c;
	}
	if (length){
		arr[length] = '\0';
	}
}

void String::free(){
	delete[] arr;
	arr = new char;
	arr[0] = '\0';
	allocSize = 1;
	length = 0;
}

String& String::concatenate(const char* rhs){
	int rhsLength = strlen(rhs);
	for (int i = 0; i < rhsLength; ++i){
		add(rhs[i]);
	}
	return *this;
}

void String::cutAfter(int n){
	if (n < 0){
		throw std::exception("Incorrect index at String::cutAfter.");
	}
	if (n >= length){
		return;
	}
	char* buffer = new char[n + 1];
	for (int i = 0; i < n; ++i){
		buffer[i] = arr[i];
	}
	buffer[n] = '\0';
	delete[] arr;
	arr = buffer;
}

void String::reallocate(int newAllocSize){
	char* temp = new char[newAllocSize];
	for (int i = 0; i <= length; i++){
		temp[i] = arr[i];
	}
	delete[] arr;
	arr = temp;
	allocSize = newAllocSize;
}


void String::add(char element){
	if ((length + 1) >= allocSize){
		reallocate(allocSize ? allocSize * 2 : 2);
	}
	arr[length++] = element;
	if (element){
		arr[length] = '\0';
	}
}

void String::remove(int index){
	if (index >= length || index < 0){
		std::cerr << "Invalid index at String::remove.";;
		return;
	}
	if (length--){
		for (int i = index; i <= length; ++i){
			arr[i] = arr[i + 1];
		}
	}
	int halfAllocSize = allocSize / 2;
	if ((length + 1) == halfAllocSize){
		reallocate(halfAllocSize);
	}
}

int String::getAllocatedSize()const{
	return allocSize;
}


int String::getLength()const{
	return length;
}

String::operator const char*()const{
	return arr;
}

const char* String::cString()const{
	return arr;
}


void String::setArray(const char* rhs){
	if (arr != rhs){
		free();
		concatenate(rhs);
	}
}


String::operator const char*(){
	return arr;
}


void String::print() const{
	if (length > 0){
		for (int i = 0; i < length; i++){
			std::cout << arr[i];
		}
		std::cout << "\n";
	}
}

String String::wordNumber(int index)const{
	if (index<1 || index>countWords()){
		return "";
	}

	int cnt = 0;
	String delims("\t \n");
	int i = 0;
	while (cnt != index){
		while (arr[i] && delims.isIn(arr[i])){
			++i;
		}
		if (arr[i] && !delims.isIn(arr[i])){
			++cnt;
		}
		if (cnt != index){
			while (arr[i] && !delims.isIn(arr[i])){
				++i;
			}
		}
	}

	int j = i;
	while (!delims.isIn(arr[i]) && arr[i]){
		++i;
	}
	String word;
	while (j != i){
		word.add(arr[j++]);
	}
	return word;
}

char& String::operator[](int index){
	if (index > length || index < 0){
		throw std::out_of_range("Invalid index at String::operator[].");
	}
	return arr[index];
}

char String::operator[](int index) const{
	if (index > length || index < 0){
		throw std::out_of_range("Invalid index at String::operator[] const.");
	}
	return arr[index];
}

bool String::operator<(const String & rhs) const{
	return strcmp(arr, rhs.arr)<0;
}

bool String::operator>(const String & rhs) const{
	return strcmp(arr, rhs.arr)>0;
}

bool String::operator==(const String & rhs) const{
	if (!strcmp(arr, rhs.arr)){
		return true;
	}
	return false;
}

bool String::operator!=(const String & rhs) const{
	return !(*this == rhs);
}

String& String::operator+(const char* rhs){
	concatenate(rhs);
	return *this;
}