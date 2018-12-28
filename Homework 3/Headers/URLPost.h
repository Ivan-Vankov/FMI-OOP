#ifndef _URLPOST_HEADER_
#define _URLPOST_HEADER_
#include "BasicPost.h"

class URLPost : public BasicPost {
public:
	URLPost(int newPostNum, const BasicUser* newAuthor, const String& url, const String& descr);
	virtual bool isTxt()const { return false; }
	virtual bool isImg()const { return false; }
	virtual bool isUrl()const { return true; }
	virtual ~URLPost() {}
private:
	virtual void serializeContent(std::ofstream& ofile)const;

	String description;
};

#endif // _URLPOST_HEADER_