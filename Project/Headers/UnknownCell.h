#ifndef _UNKNOWNCELL_HEADER_
#define _UNKNOWNCELL_HEADER_
#include "Cell.h"

class UnknownCell : public Cell {
public:
	UnknownCell(const String& = "");
	virtual ~UnknownCell();

	virtual int getLength() const;
	virtual String getContent() const;
	virtual String getValue() const;
	virtual void printCell() const;
private:
	String content;
};

#endif // _UNKNOWNCELL_HEADER_