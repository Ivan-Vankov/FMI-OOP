#ifndef _TXTPOST_HEADER_
#define _TXTPOST_HEADER_
#include "BasicPost.h"

class TXTPost : public BasicPost {
public:
	TXTPost(int newPostNum, const BasicUser* newAuthor, const String& text);
	virtual bool isTxt()const { return true; }
	virtual bool isImg()const { return false; }
	virtual bool isUrl()const { return false; }
	virtual ~TXTPost() {}
private:
	virtual void serializeContent(std::ofstream& ofile)const;
};

#endif // _TXTPOST_HEADER_