#include "BasicPost.h"

BasicPost::BasicPost(int newPostNum, const BasicUser* newAuthor, const String& newContent) :
	postNum(newPostNum), author(newAuthor), content(newContent) {}

void BasicPost::generateFile() const {
	std::ofstream ofile;
	generateOfstream(ofile);
	generateFileBeginning(ofile);
	serializeContent(ofile);
	generateFileEnd(ofile);
	ofile.close();
}


void BasicPost::generateOfstream(std::ofstream& ofile)const {
	String fileName("PostNumber");
	// We asume there wont be more than 10 digit numbers
	char* num = new char[11];
	_itoa_s(postNum, num, 10, 10);
	fileName + num + ".html";
	delete[] num;
	ofile.open(fileName, std::ios::trunc);
	if (!ofile.is_open()) {
		throw std::exception("Couldn't open file.");
	}
}

void BasicPost::generateFileBeginning(std::ofstream& ofile) {
	ofile << "<!DOCTYPE html>\n<html>\n<body>" << std::endl << std::endl;
	if (!ofile.good()) {
		throw std::exception("Couldn't write to file.");
	}
}

void BasicPost::generateFileEnd(std::ofstream& ofile) {
	ofile << "\n</html>\n</body>" << std::endl;
	if (!ofile.good()) {
		throw std::exception("Couldn't write to file.");
	}
}