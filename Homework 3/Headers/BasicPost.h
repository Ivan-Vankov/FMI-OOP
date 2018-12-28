#ifndef _BASIC_POST_
#define _BASIC_POST_
#include "BasicUser.h"
#include <fstream>

enum PostType {
	txt,
	img,
	url
};

class BasicPost {
	friend class FmiBook;
public:
	BasicPost(int newPostNum, const BasicUser* newAuthor, const String& newContent);
	virtual bool isTxt()const = 0;
	virtual bool isImg()const = 0;
	virtual bool isUrl()const = 0;
	virtual void generateFile()const;
	virtual ~BasicPost() {}
protected:
	void generateOfstream(std::ofstream& ofile)const;
	static void generateFileBeginning(std::ofstream& ofile);
	virtual void serializeContent(std::ofstream& ofile)const = 0;
	static void generateFileEnd(std::ofstream& ofile);

	int postNum;
	const BasicUser* author;
	String content;
};

#endif // _BASICPOST_HEADER_