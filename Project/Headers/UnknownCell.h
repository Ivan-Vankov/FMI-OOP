#ifndef _UNKNOWNCELL_HEADER_
#define _UNKNOWNCELL_HEADER_
#include "TableDelegate.h"
#include "Cell.h"

class UnknownCell : public Cell {
public:
	UnknownCell(const String& = "");
	virtual ~UnknownCell();

	int getLength() const override;
	String getContent() const override;
	String getValue() const override;
	void printCell() const override;
	Cell* clone() const override;
private:
	String content;
};

#endif // _UNKNOWNCELL_HEADER_