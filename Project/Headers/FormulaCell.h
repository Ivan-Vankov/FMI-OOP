#ifndef _FORMULACELL_HEADER_
#define _FORMULACELL_HEADER_
#include "TableDelegate.h"
#include <unordered_map>
#include <exception>
#include "Vector.h"
#include <utility>
#include <stack>
#include <cmath>
#include <map>
#define eps 0.000001

class FormulaCell : public Cell {
public:
	FormulaCell(const FormulaCell&);
	FormulaCell(int row, int coll, const String&, const TableDelegate*);
	FormulaCell& operator=(const FormulaCell&);
	virtual ~FormulaCell();

	int getRow() const;
	int getColl() const;

	virtual String getContent() const;
	virtual String getValue() const;
	virtual int getLength() const;
	virtual void printCell() const;
	const TableDelegate* getOriginTable() const;

	static bool verifyData(const String& content);
private:
	String calcValue() const;

	// There is an address in cellContent starting at index
	// and this method returns that address 
	String getAddressFromIndex(int& index, const String& cellContent) const;

	// There is an address in cellContent starting at index
	// and this method returns that address 
	String getNumberFromIndex(int& index, const String& cellContent) const;

	// 1+-1 turns to 1-1
	// Returns "-", "+" or ""(__*+123 -> __*123)
	String getCorrectSignFromIndex(int& index, const String&) const;

	// Turns formulas from "=R001C02 + - 005*+--	04" to "=R1C2-5*4"
	String toRegularForm(const String&) const;

	// Turn a formula in regular form wiht no addresses to a formula with postfix notation
	String toPostfixNotation(const String&) const;
	// Evaluate a formula in postfix notation
	String postfixEval(const String&) const;

	// Check if the char is among ^,*,/,+,-,(,)
	bool isOperator(char) const;
	int operatorPrecenence(char) const;

	// Does operation op on num1 and num2
	String eval(char op, String num1, String num2) const;

	// Puts the cells in topologically sorted order in stack
	// and returns false if there is a cycle
	// The const char* part is the row and column of the cell
	// The vector <order> will be the order that the cells are topSorted in
	// (The vector is necessary because the quickness of the find operation in unordered_map is 
	// dependant on the string, because of which the cells are shuffled during insertion)
	bool topSort(const std::pair<std::string, const Cell*>& currCell,
		std::unordered_map<std::string, const Cell*>& stack,
		Vector<const std::string*>& order) const;
	void topSortUtil(const std::pair<std::string, const Cell*>& currCell,
		std::unordered_map<std::string, const Cell*>& stack,
		std::unordered_map<std::string, const Cell*>& visited,
		Vector<const std::string*>& order) const;

	// Uses the topologically sorted cells is <stack> and makes a map from their addresses to their numerical values
	void removeAddressesInOrder(std::unordered_map<std::string, String>& values,
		const std::unordered_map<std::string, const Cell*>& stack,
		const Vector<const std::string*>& order) const;

	// Puts all neighbours of currCell into the map
	void getAllNeighbours(const Cell* currCell, std::unordered_map<std::string, const Cell*>&) const;
	std::string FormulaCell::getThisCellRnC() const;

	std::string intToString(int) const;

	static const String OPERATORS;
	int row;
	int coll;
	const TableDelegate* table;
	String content;
};

#endif