#include "CellFactory.h"

Cell* CellFactory::createCell(int row, int coll, const String& newCellContent, const TableDelegate* originTable) {
	String cellContent = newCellContent;
	if (cellContent.countWords() == 0) {
		return new UnknownCell();
	}
	cellContent.removeUnnecessarySymbols(" \t");
	if (NumberCell::verifyData(cellContent)) {
		return new NumberCell(cellContent);
	}
	if (StringCell::verifyData(cellContent)) {
		return new StringCell(cellContent);
	}
	if (DateCell::verifyData(cellContent)) {
		return new DateCell(cellContent);
	}
	if (FormulaCell::verifyData(cellContent)) {
		return new FormulaCell(row, coll, cellContent, originTable);
	}
	return new UnknownCell(cellContent);
}

Cell* CellFactory::cloneCell(const Cell* cell, const TableDelegate* newOriginTable) {
	const FormulaCell* formulaCell = dynamic_cast<const FormulaCell*>(cell);
	if (formulaCell) {
		return formulaCell->clone(newOriginTable);
	}
	else {
		return cell->clone();
	}
}