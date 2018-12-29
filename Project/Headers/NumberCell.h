#ifndef _NUMBERCELL_HEADER_
#define _NUMBERCELL_HEADER_
#include "TableDelegate.h"
#include <iostream>
#include "Cell.h"

class NumberCell :public Cell {
public:
	NumberCell(const String&);
	NumberCell(const NumberCell&);
	NumberCell& operator=(const NumberCell&);
	virtual ~NumberCell();

	int getLength() const override;
	String getContent() const override;
	String getValue() const override;
	void printCell() const override;
	Cell* clone() const override;

	static bool verifyData(const String& content);
private:
	// Removes initial zeroes (001 -> 1)
	String toRegularForm(const String&) const;

	String content;
};

#endif // _NUMBERCELL_HEADER_