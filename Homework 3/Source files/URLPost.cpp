#include "URLPost.h"

URLPost::URLPost(int newPostNum, const BasicUser* newAuthor, const String& url, const String& descr) :
	BasicPost(newPostNum, newAuthor, url), description(descr) {}

void URLPost::serializeContent(std::ofstream& ofile)const {
	ofile << "<a href=\"" << content << "\"> " << description << " </a> <br/>" << std::endl;
	if (!ofile.good()) {
		throw std::exception("Couldn't write to file.");
	}
}