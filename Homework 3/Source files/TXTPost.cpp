#include "TXTPost.h"

TXTPost::TXTPost(int newPostNum, const BasicUser* newAuthor, const String& text) :
	BasicPost(newPostNum, newAuthor, text) {}

void TXTPost::serializeContent(std::ofstream& ofile)const {
	ofile << content << " <br/>" << std::endl;
	if (!ofile.good()) {
		throw std::exception("Couldn't write to file.");
	}
}