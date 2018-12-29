#ifndef _CELL_HEADER_
#define _CELL_HEADER_
#include "String.h"

class Cell {
public:
	// Just returns the cell's contents(unchanged)
	virtual String getContent() const = 0;
	// Returns the cell's value as an double(int String form)
	virtual String getValue() const = 0;
	// Returns how much space the cell takes up: 123 -> 3; "123" -> 3
	virtual int getLength() const = 0;
	virtual void printCell() const = 0;
	virtual ~Cell() {};
};

#endif // _CELL_HEADER_