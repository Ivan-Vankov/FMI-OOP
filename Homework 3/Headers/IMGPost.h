#ifndef _IMGPOST_HEADER_
#define _IMGPOST_HEADER_
#include "BasicPost.h"

class IMGPost : public BasicPost {
public:
	IMGPost(int newPostNum, const BasicUser* newAuthor, const String& imgPath);
	virtual bool isTxt()const { return false; }
	virtual bool isImg()const { return true; }
	virtual bool isUrl()const { return false; }
	virtual ~IMGPost() {}
private:
	virtual void serializeContent(std::ofstream& ofile)const;
};

#endif // _IMGPOST_HEADER_