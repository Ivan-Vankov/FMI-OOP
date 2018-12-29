#ifndef _CELLFACTORY_HEADER_
#define _CELLFACTORY_HEADER_
#include "String.h"
#include "Vector.h"
#include "TableDelegate.h"
#include "Cell.h"
#include "StringCell.h"
#include "UnknownCell.h"
#include "FormulaCell.h"
#include "NumberCell.h"
#include "DateCell.h"

class CellFactory {
public:
	static Cell* createCell(int row, int coll, const String&, const TableDelegate*);
	static Cell* cloneCell(const Cell*, const TableDelegate*);
};

#endif // _CELLFACTORY_HEADER_