#include "IMGPost.h"

IMGPost::IMGPost(int newPostNum, const BasicUser* newAuthor, const String& imgPath) :
	BasicPost(newPostNum, newAuthor, imgPath) {}


void IMGPost::serializeContent(std::ofstream& ofile)const {
	ofile << "<img src=\"" << content << "\"> <br/>" << std::endl;
	if (!ofile.good()) {
		throw std::exception("Couldn't write to file.");
	}
}