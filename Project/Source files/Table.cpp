#include "Table.h"

Table::Table() : longestLine(0), table() {
}

Table::Table(const Table& other) {
	copyFrom(other);
}

Table::Table(const String& newName) : longestLine(0), table() {
	readFile(newName);
}

Table& Table::operator=(const Table& other) {
	if (this != &other) {
		copyFrom(other);
	}
	return *this;
}

Table::~Table() {
	clean();
}

int Table::getLongestLine() const {
	return longestLine;
}

int Table::getLinesAmount() const {
	return table.getSize();
}

const Vector<Cell*>& Table::lineToVector(int index) const {
	if (index >= table.getSize() || index < 0) {
		throw std::out_of_range("Invalid index at Table::lineToVector(int).");
	}
	return table[index];
}

const Vector<Cell*>& Table::operator[](int index) const {
	return lineToVector(index);
}

double Table::getCellValue(int row, int col) const {
	// If there aren't that many rows in the table
	if (!RnCCheck(row, col)) {
		return 0;
	}
	if (!table[row][col]) {
		return 0;
	}
	return atof(table[row][col]->getValue());
}

void Table::clean() {
	for (int i = 0; i < table.getSize(); ++i) {
		for (int j = 0; j < table[i].getSize(); ++j) {
			delete table[i][j];
		}
	}
	table.clean();
	longestLine = 0;
}

void Table::removeLine(int index) {
	if (index > table.getSize() || index < 0) {
		throw std::out_of_range("Invalid index at Table::removeLine.\n");
	}
	table.remove(index);
	calcLongestLine();
}

String Table::fileInString() const {
	String inString;
	int rows = table.getSize();
	// If there are any rows
	if (rows--) {
		int i = 0;
		// Go through each one of them
		for (; i < rows; ++i) {
			int currColCount = table[i].getSize();
			if (currColCount--) {
				int j = 0;
				// Do the same for the columns
				for (; j < currColCount; ++j) {
					inString + table[i][j]->getContent() + ",";
				}
				inString + table[i][j]->getContent() + "\n";
			}
		}
		// After that go through the last line without giving it a "\n" at the end
		int currColCount = table[i].getSize();
		if (currColCount--) {
			int j = 0;
			for (; j < currColCount; ++j) {
				inString + table[i][j]->getContent() + ",";
			}
			inString + table[i][j]->getContent();
		}
	}
	return inString;
}

void Table::print() const {
	Vector<int> columnLengths(longestLine, 0);

	// Get the space each cell in a line is going to take
	for (int i = 0; i < table.getSize(); ++i) {
		for (int j = 0; j < table[i].getSize(); ++j) {
			const Cell* currCell = table[i][j];
			if (currCell) {
				int currCellSize = currCell->getLength();
				if (currCellSize > columnLengths[j]) {
					columnLengths[j] = currCellSize;
				}
			}
		}
	}

	// Print the table
	for (int i = 0; i < table.getSize(); ++i) {
		std::cout << ' ';
		for (int j = 0; j < table[i].getSize(); ++j) {
			table[i][j]->printCell();
			int spacesLeft = columnLengths[j] - table[i][j]->getLength();
			// Fill the rest of the space with ' '
			for (int k = 0; k < spacesLeft; ++k) {
				std::cout << ' ';
			}
			std::cout << " | ";
		}
		// If the current line has less cells than the longest line
		// fill the rest of the cells
		for (int l = table[i].getSize(); l < longestLine; ++l) {
			for (int m = 0; m < columnLengths[l]; ++m) {
				std::cout << ' ';
			}
			std::cout << " | ";
		}
		std::cout << std::endl;
	}
}

void Table::saveToFile(const String& path) const {
	std::ofstream ofile(path, std::ios::trunc);
	if (!ofile.good()) {
		std::cerr << "Couldn't open file " << path << " for writing.\n";
		return;
	}
	int i = 0;
	for (; i < table.getSize() - 1; ++i) {
		writeLine(ofile, i, path);
		ofile << std::endl;
		if (!ofile.good()) {
			std::cerr << "Couldn't write to file " << path << ".\n";
			return;
		}
	}
	if (i) {
		writeLine(ofile, i, path);
	}
	ofile.close();
}

void Table::readFile(const String& path) {
	std::ifstream ifile(path);
	if (!ifile.is_open()) {
		std::cerr << "Coulnd't open file " << path << " for reading.\n";
		return;
	}
	// Save the table as it is just in case
	Vector<Vector<Cell*>> prev = table;
	char c;
	String cellBuffer;
	int row = 0;
	int coll = 0;
	Vector<Cell*> line;
	while (ifile.get(c)) {
		if (!ifile.good()) {
			std::cerr << "Couldn't read from file.\n";
			ifile.clear();
			// Leave the table as it was before
			table = prev;
			return;
		}
		if (c == ',' || c == '\n') {
			line.pushBack(CellFactory::createCell(row, coll++, cellBuffer, this));
			cellBuffer.free();
			if (c == '\n') {
				table.pushBack(line);
				if (longestLine < line.getSize()) {
					longestLine = line.getSize();
				}
				line.clean();
				coll = 0;
				++row;
			}
			continue;
		}
		cellBuffer.pushBack(c);
	}
	if (!ifile.good() && !ifile.eof()) {
		std::cerr << "Couldn't read from file.\n";
		// Leave the table as it was before
		table = prev;
		return;
	}
	// To catch the line
	line.pushBack(CellFactory::createCell(row, coll, cellBuffer, this));
	table.pushBack(line);
	if (longestLine < line.getSize()) {
		longestLine = line.getSize();
	}

	ifile.close();
}

const Cell* Table::getCell(const String& RandCString) const {
	int row;
	int col;
	getRandC(RandCString, row, col);
	if (!RnCCheck(row, col)) {
		return nullptr;
	}
	return table[row][col];
}

void Table::setCell(const String& RandCString, const String& content) {
	int row;
	int col;
	getRandC(RandCString, row, col);
	// If there aren't that many rows in the table
	if (!RnCCheck(row, col)) {
		throw std::out_of_range("Invalid table indexes.");
	}
	replaceCell(row, col, content);
}

void Table::getRandC(const String& RandCString, int& row, int& col) const {
	String Row;
	String Col;
	int i = 1;
	for (; i < RandCString.getLength(); ++i) {
		if (RandCString[i] == 'C') {
			++i;
			break;
		}
		Row.pushBack(RandCString[i]);
	}
	for (; i < RandCString.getLength(); ++i) {//vzima col
		Col.pushBack(RandCString[i]);
	}
	row = atoi(Row);
	col = atoi(Col);
}

void Table::replaceCell(int row, int col, const String& content) {
	delete table[row][col];
	table[row][col] = CellFactory::createCell(row, col, content, this);
}

void Table::calcLongestLine() {
	for (int i = 0; i < table.getSize(); ++i) {
		if (table[i].getSize() > longestLine) {
			longestLine = table[i].getSize();
		}
	}
}

void Table::copyFrom(const Table& other) {
	longestLine = other.longestLine;
	table.resize(other.table.getSize());
	Vector<Cell*> line;
	for (int i = 0; i < other.table.getSize(); ++i) {
		line.resize(other[i].getSize());
		for (int j = 0; j < other.table[i].getSize(); ++j) {
			line.pushBack(CellFactory::cloneCell(other.table[i][j], this));
		}
		table.pushBack(line);
		line.clean();
	}
}

void Table::writeLine(std::ofstream& ofile, int i, const String& path) const {
	int j = 0;
	for (; j < table[i].getSize() - 1; ++j) {
		ofile << table[i][j]->getContent() << ",";
		if (!ofile.good()) {
			std::cerr << "Couldn't write to file " << path << ".\n";
			return;
		}
	}
	if (j) {
		ofile << table[i][j]->getContent();
		if (!ofile.good()) {
			std::cerr << "Couldn't write to file " << path << ".\n";
			return;
		}
	}
}

bool Table::RnCCheck(int row, int col) const {
	// If there aren't that many rows in the table
	if (table.getSize() <= row) {
		return false;
	}
	// If there aren't that many columns in that row
	if (table[row].getSize() <= col) {
		return false;
	}
	return true;
}