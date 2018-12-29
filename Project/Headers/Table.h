#ifndef _TABLE_HEADER_
#define _TABLE_HEADER_
#include "TableDelegate.h"
#include "CellFactory.h"
#include "String.h"
#include <fstream>

typedef bool(*Comparer)(double a, double b);

static bool isLessThan(double a, double b) {
	return a < b;
}

static bool isGreaterThan(double a, double b) {
	return a > b;
}

enum Order {
	desc,
	asc
};

class Table : public TableDelegate {
public:
	Table();
	Table(const Table&);
	Table(const String& newFileName);
	Table& operator=(const Table&);
	~Table();

	virtual void removeLine(int);
	virtual int getLongestLine() const;
	virtual int getLinesAmount() const;
	virtual const Vector<Cell*>& lineToVector(int) const;
	virtual const Vector<Cell*>& operator[](int) const;
	virtual double getCellValue(int row, int col) const;

	virtual const Cell* getCell(const String&) const;
	void setCell(const String& RandC, const String& content);

	void readFile(const String&);
	String fileInString() const;

	void print() const;
	// order 0 will be descending
	// order 1 will be ascending
	void sort(int coll, Order order = desc);
	void saveToFile(const String&) const;

	void clean();
private:
	// [0] is R<num>, [1] is C<num>
	void getRandC(const String&, int& row, int& col) const; 

	void replaceCell(int row, int col, const String& content);

	void calcLongestLine();

	void copyFrom(const Table&);

	void writeLine(std::ofstream&, int row, const String& path) const;

	// Checks if row and col are valid indexes for the table
	bool RnCCheck(int row, int col) const;

	// Highest number of cells in a line in the table
	int longestLine;
	Vector<Vector<Cell*>> table;
};

#endif // _TABLE_HEADER_