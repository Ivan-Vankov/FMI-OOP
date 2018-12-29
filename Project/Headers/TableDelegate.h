#ifndef _TABLEDELEGATE_HEADER_
#define _TABLEDELEGATE_HEADER_
#include "Cell.h"
#include "String.h"
#include "Vector.h"

class TableDelegate {
public:
	virtual ~TableDelegate() {}

	virtual void removeLine(int) = 0;
	virtual int getLongestLine() const = 0;
	virtual const Cell* getCell(const String&) const = 0;
	virtual int getLinesAmount() const = 0;
	virtual const Vector<Cell*>& lineToVector(int) const = 0;
	virtual const Vector<Cell*>& operator[](int) const = 0;
	virtual double getCellValue(int row, int col) const = 0;
};

#endif  // _TABLEDELEGETE_HEADER_