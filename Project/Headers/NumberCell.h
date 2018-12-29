#ifndef _NUMBERCELL_HEADER_
#define _NUMBERCELL_HEADER_
#include "Cell.h"
#include <iostream>

class NumberCell :public Cell {
public:
	NumberCell(const String&);
	NumberCell(const NumberCell&);
	NumberCell& operator=(const NumberCell&);
	virtual ~NumberCell();

	virtual int getLength() const;
	virtual String getContent() const;
	virtual String getValue() const;
	virtual void printCell() const;

	static bool verifyData(const String& content);
private:
	// Removes initial zeroes (001 -> 1)
	String toRegularForm(const String&) const;

	String content;
};

#endif // _NUMBERCELL_HEADER_