#ifndef _STRINGCELL_HEADER_
#define _STRINGCELL_HEADER_
#include "Cell.h"

class StringCell : public Cell {
public:
	StringCell(const String& = "");
	StringCell(const StringCell&);
	StringCell& operator=(const StringCell&);
	virtual ~StringCell();

	virtual int getLength() const;
	virtual String getContent() const;
	virtual String getValue() const;
	virtual void printCell() const;

	static bool verifyData(const String& content);
private:
	String content;
};

#endif // _STRINGCELL_HEADER_
