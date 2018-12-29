#include "FormulaCell.h"

const String FormulaCell::OPERATORS("^*/+-()");

FormulaCell::FormulaCell(const FormulaCell& rhs) : row(rhs.row), coll(rhs.coll),
content(rhs.content), table(rhs.table) {}

FormulaCell::FormulaCell(const FormulaCell& rhs, const TableDelegate* newTable) : row(rhs.row), coll(rhs.coll),
content(rhs.content), table(newTable) {}

FormulaCell::FormulaCell(int row, int coll, const String& input, const TableDelegate* originTable) {
	if (!verifyData(input)) {
		throw std::exception("Data is not compatible with FormulaCell.");
	}
	if (!originTable) {
		throw std::exception("Invalid origin table.");
	}
	this->row = row;
	this->coll = coll;
	content = toRegularForm(input);
	table = originTable;
}

FormulaCell& FormulaCell::operator=(const FormulaCell&rhs) {
	if (this != &rhs) {
		content = rhs.content;
		table = rhs.table;
	}
	return *this;
}

FormulaCell::~FormulaCell() {
	// No dynamic stuff to delete
}

int FormulaCell::getRow() const {
	return row;
}

int FormulaCell::getColl() const {
	return coll;
}

int FormulaCell::getLength() const {
	return getValue().getLength();
}

String FormulaCell::getContent() const {
	return content;
}

String FormulaCell::getValue() const {
	return calcValue();
}

void FormulaCell::printCell() const {
	std::cout << getValue();
}

Cell* FormulaCell::clone() const {
	return new FormulaCell(*this);
}

Cell* FormulaCell::clone(const TableDelegate* newTable) const {
	return new FormulaCell(*this, newTable);
}

const TableDelegate* FormulaCell::getOriginTable() const {
	return table;
}

String FormulaCell::toRegularForm(const String& formula) const {
	String value;
	int i = 0;
	while (i < formula.getLength()) {
		// If we have a formula
		if (formula[i] == 'R') {
			value + getAddressFromIndex(i, formula);
			continue;
		}

		// Check if there are several + and/or - signs one after another
		if (formula[i] == '+' || formula[i] == '-') {
			value + getCorrectSignFromIndex(i, formula);
			continue;
		}

		if (formula[i] >= '0' && formula[i] <= '9') {
			value + getNumberFromIndex(i, formula);
			continue;
		}

		// Skip whitespaces and tabs
		if (formula[i] != ' ' && formula[i] != '\t') {
			value.pushBack(formula[i]);
		}
		++i;
	}
	return value;
}

// The formula is in regular form and has had its addresses removed
String FormulaCell::toPostfixNotation(const String& formula) const {
	String postFix;
	std::stack<char> operators;

	int i = 0;
	while (i < formula.getLength()) {
		if (formula[i] >= '0' && formula[i] <= '9') {
			// The space is a separator
			postFix + getNumberFromIndex(i, formula) + " ";
			continue;
		}

		// Unary '-' check
		if (formula[i] == '-') {
			if (!i ||
				isOperator(formula[i - 1])) {
				postFix + getNumberFromIndex(i, formula) + " ";
				continue;
			}
		}

		// If we reach a closing paranthesis
		if (formula[i] == ')') {
			// Pop all of the operators in the stack 
			// until an opening paranthesis is reached
			while (!operators.empty() &&
				operators.top() != '(') {
				postFix.pushBack(operators.top());
				operators.pop();
			}
			// Pop the '('
			operators.pop();
			postFix + " ";
			++i;
			continue;
		}

		if (isOperator(formula[i])) {
			// While there are operators with higher precedence 
			// on the top of the stack
			while (!operators.empty() &&
				formula[i] != '(' &&
				operatorPrecenence(operators.top()) >= operatorPrecenence(formula[i])) {
				// Add the operator at the top of the stack to postFix
				postFix.pushBack(operators.top());
				postFix + " ";
				// and remove it from the stack
				operators.pop();
			}
			operators.push(formula[i]);
		}
		++i;
	}

	// There may still be operators in the stack
	while (!operators.empty()) {
		// Add them to postFix
		postFix.pushBack(operators.top());
		operators.pop();
	}
	return postFix;
}

String FormulaCell::postfixEval(const String& postFixFormula) const {
	std::stack<String> nums;

	for (int i = 0; i < postFixFormula.getLength(); ++i) {
		if (postFixFormula[i] >= '0' && postFixFormula[i] <= '9') {
			// The space is a separator
			nums.push(getNumberFromIndex(i, postFixFormula));
			continue;
		}

		// Unary '-' check
		if (postFixFormula[i] == '-') {
			if (postFixFormula[i + 1] >= '0' &&
				postFixFormula[i + 1] <= '9') {
				nums.push(getNumberFromIndex(i, postFixFormula));
				continue;
			}
		}

		if (isOperator(postFixFormula[i])) {
			String num2 = nums.top();
			nums.pop();
			String num1 = nums.top();
			nums.pop();
			try {
				nums.push(eval(postFixFormula[i], num1, num2));
			}
			catch (std::invalid_argument& e) {
				return e.what();
			}
		}
	}
	return nums.top();
}

bool FormulaCell::isOperator(char c) const {
	return OPERATORS.isIn(c);
}

int FormulaCell::operatorPrecenence(char c) const {
	if (c == '(' || c == ')') {
		return 0;
	}
	if (c == '+' || c == '-') {
		return 1;
	}
	if (c == '*' || c == '/') {
		return 2;
	}
	if (c == '^') {
		return 3;
	}
	throw std::exception("Unknown operator.");
	return -1;
}

String FormulaCell::eval(char op, String num1, String num2) const {
	double left = atof(num1);
	double right = atof(num2);
	double result;

	switch (op) {
	case('^'): result = pow(left, right); break;
	case('*'): result = left * right; break;
	case('/'): {
		if (right < eps && right > -eps) {
			throw std::invalid_argument("Can't divide by zero.");
		}
		result = left / right; break;
	}
	case('+'): result = left + right; break;
	case('-'): result = left - right; break;
	default: throw std::exception("Unknown operator.");
	}
	// Check if there are only zeroes after the decimal point
	double intPart;
	double fractionalPart = modf(result, &intPart);
	if (fractionalPart == 0) {
		char charResult[50];
		_itoa_s((int)intPart, charResult, 50, 10);
		return charResult;
	}
	// If not then just return the result as a string
	return std::to_string(result).c_str();
}

bool FormulaCell::topSort(const std::pair<std::string, const Cell*>& currCell,
	std::unordered_map<std::string, const Cell*>& stack,
	Vector<const std::string*>& order) const {
	std::unordered_map<std::string, const Cell*> visited;
	bool circularReference = false;
	try {
		topSortUtil(currCell, stack, visited, order);
	}
	catch (std::exception&) {
		circularReference = true;
	}
	return circularReference;
}

void FormulaCell::topSortUtil(const std::pair<std::string, const Cell*>& currCell,
	std::unordered_map<std::string, const Cell*>& stack,
	std::unordered_map<std::string, const Cell*>& visited,
	Vector<const std::string*>& order) const {
	visited.insert(currCell);
	if (currCell.second) {
		std::unordered_map<std::string, const Cell*> neighbours;
		getAllNeighbours(currCell.second, neighbours);
		for (auto a : neighbours) {
			if (stack.find(a.first) != stack.cend()) {
				continue;
			}
			if (visited.find(a.first) != visited.cend()) {
				throw std::exception("There is a circular reference.");
			}
			topSortUtil(a, stack, visited, order);
		}
	}
	visited.erase(currCell.first);
	stack.insert(currCell);
	order.pushBack(&stack.find(currCell.first)->first);
}

void FormulaCell::removeAddressesInOrder(std::unordered_map<std::string, String>& values,
	const std::unordered_map<std::string, const Cell*>& stack,
	const Vector<const std::string*>& order) const {
	String currCellValue;
	String noAddresses;
	// Go through the cells in topologically sorted order
	for (int i = 0; i < order.getSize(); ++i) {
		const Cell* currCell = stack.find(*order[i])->second;
		if (!currCell) {
			noAddresses = "0";
			values.insert(make_pair(*order[i], noAddresses));
			noAddresses.free();
			continue;
		}
		// Get the content of the one in the i-th place in the order
		// If the cell is a formula then turn it to regular form
		if (dynamic_cast<const FormulaCell*>(currCell)) {
			currCellValue.free();
			currCellValue.resize(currCell->getContent().getLength() + 1);
			// Skip the initial '=' sign
			for (int i = 1; i < currCell->getContent().getLength(); ++i) {
				currCellValue.pushBack(currCell->getContent()[i]);
			}
		}
		else {
			currCellValue = currCell->getValue();
		}

		// Get the value of the i-th cell
		for (int j = 0; j < currCellValue.getLength();) {
			// If there is an address of another cell starting
			if (currCellValue[j] == 'R') {
				// Get that address
				std::string newAddress = getAddressFromIndex(j, currCellValue);
				// And add it to <noAddresses>
				noAddresses + values.find(newAddress)->second;
				// (Because the cells are in topSort order there will never be a time 
				// when we search in values for a cell that has not yet had it's addresses removed
			}
			// Otherwise just add the j-th element of currCellValue
			else {
				noAddresses.pushBack(currCellValue[j++]);
			}
		}
		// Insert it into values
		values.insert(make_pair(*order[i], postfixEval(toPostfixNotation(noAddresses))));
		noAddresses.free();
	}
}

// Should return all neighbours(not the current cell's address
void FormulaCell::getAllNeighbours(const Cell* cell,
	std::unordered_map<std::string, const Cell*>& allNeighbours) const {
	int i = 0;
	while (i < cell->getContent().getLength()) {
		if (cell->getContent()[i] != 'R') {
			++i;
		}
		else {
			std::string address = getAddressFromIndex(i, cell->getContent()).cString();
			allNeighbours.insert(std::make_pair(address, table->getCell(address)));
		}
	}
}

std::string FormulaCell::intToString(int num) const {
	int numLength = 1;
	if (num) {
		numLength += (int)log10(num);
	}
	char* numPtr = new char[numLength + 1];
	_itoa_s(num, numPtr, numLength + 1, 10);
	std::string colNum(numPtr);
	delete[] numPtr;
	return colNum;
}

std::string FormulaCell::getThisCellRnC() const {
	return std::string("R") + intToString(row) + std::string("C") + intToString(coll);
}

String FormulaCell::calcValue() const {
	// Row and collumn of this cell : R<num>C<num>
	std::string RnC = getThisCellRnC();

	// Create a stack stat maps the addresses of the cells to themselves
	std::unordered_map<std::string, const Cell*> stack;
	Vector<const std::string*> order;
	// And sort it topologically
	if (topSort(std::make_pair(RnC, this), stack, order)) {
		throw std::overflow_error("There are one or more circular references in the table.");
	}
	// Create a new map that uses prev one to map the addresses of the cells to their values
	// which prevents redundant calculations of same cells 
	std::unordered_map<std::string, String> values;
	removeAddressesInOrder(values, stack, order);

	// Return the value of the this cell
	return values.find(RnC)->second;
}

String FormulaCell::getAddressFromIndex(int& i, const String& content) const {
	String newAddress("R");
	// Skip the zeroes after R 
	// R000C1 -> R0C1
	while (content[++i] == '0' && content[i + 1] != 'C');
	while (i < content.getLength()) {
		if (content[i] == 'C') {
			newAddress.pushBack('C');
			++i;
			break;
		}
		newAddress.pushBack(content[i++]);
	}
	// Skip the zeroes after C 
	// R0C001 -> R0C1
	for (; content[i] == '0' &&
		i + 1 < content.getLength() &&
		content[i + 1] == '0'; ++i);
	// And add the <num> after C
	while (i < content.getLength() &&
		content[i] >= '0' && content[i] <= '9') {
		newAddress.pushBack(content[i++]);
	}
	return newAddress;
}

String FormulaCell::getNumberFromIndex(int& i, const String& content) const {
	String numberAfter;
	// Unary '-' check
	if (content[i] == '-') {
		if (!i ||
			content[i + 1] >= '0'&& content[i + 1] <= '9') {// 10/-2
			numberAfter.pushBack('-');
			++i;
		}
	}
	// Skip spaces and tabs after '-'
	if (numberAfter[0] == '-') {
		for (; content[i] == ' ' || content[i] == '\t'; ++i);
	}
	// Skip initial zeros
	for (; content[i] == '0' &&
		i + 1 < content.getLength() &&
		content[i + 1] >= '0' &&
		content[i + 1] <= '9'; ++i);
	// Add the i-th char until we reach the end of the content or a sign
	for (; i < content.getLength() &&
		!isOperator(content[i]) &&
		content[i] != ' ' &&
		content[i] != '\t'; ++i) {
		numberAfter.pushBack(content[i]);
	}
	return numberAfter;
}

String FormulaCell::getCorrectSignFromIndex(int& i, const String& content) const {
	String sign;
	int minusCounter = 0;
	// Save the current index for later
	int j = i;
	for (; i < content.getLength(); ++i) {
		if (content[i] == ' ') {
			continue;
		}
		if (!(content[i] == '+' || content[i] == '-')) {
			break;
		}
		if (content[i] == '-') {
			++minusCounter;
		}
	}
	String aritmSigns("^/*");
	// If there is an odd number of minuses in the + and - sequence
	if (minusCounter % 2 == 1) {
		// Add a - to value
		sign.pushBack('-');
	}
	else if (minusCounter % 2 == 0 && // If there is an even number of - signs
		j != 1 && // if we weren't at the beginning of the formula
		!aritmSigns.isIn(content[j - 1])) { // and there isn't an arithmetic sign before the + and - sequence
		// add a +
		sign.pushBack('+');
	}

	return sign;
}
bool FormulaCell::verifyData(const String& content) {
	// If it doesn't start with =
	if (!(content[0] == '=')) {
		return false;
	}
	String usedSymbols(".0123456789RC() *+-/^");
	int cellLength = content.getLength();
	// Check for invalid symbols
	for (int i = 1; i < cellLength; ++i) {
		if (!usedSymbols.isIn(content[i])) {
			return false;
		}
	}
	// Checks for correctness of R<num>C<num>
	for (int i = 1; i < cellLength; ++i) {
		// = C...
		if (content[i] == 'C') {
			return false;
		}
		if (content[i] == 'R') {
			++i;
			for (; i < cellLength; ++i) {
				// = R<num>C
				if (content[i] == 'C') {
					++i;
					break;
				}
				// = RC
				if (!(content[i] >= '0' && content[i] <= '9')) {
					return false;
				}
			}
			// = R<num>C<!num>
			if (!(content[i] >= '0' && content[i] <= '9')) {
				return false;
			}
			// Terminating symbols
			String termSymbols = " +=*/^";
			++i;
			for (; i < cellLength; ++i) {
				// = R<num>C<num>
				if (usedSymbols.isIn(content[i])) {
					break;
				}
				// = R<num>C<!num>
				if (!(content[i] >= '0' && content[i] <= '9')) {
					return false;
				}
			}
		}
	}

	int brackets = 0;
	// Checks for bracket correctness
	for (int i = 1; i < cellLength; ++i) {
		if (content[i] == '(') {
			// () is forbidden
			if (content[i + 1] == ')') {
				return false;
			}
			++brackets;
		}
		if (content[i] == ')') {
			--brackets;
		}
		if (brackets < 0) {
			return false;
		}
	}
	// If there was an uneven number of brackets
	if (brackets) {
		return false;
	}

	String symbolsBefore("0123456789).");
	String arithmSymbols("/*^");
	// 6 * + - 03 is OK
	String symbolsAfter("+-0123456789(R");
	// Checks for "+ *" cases
	for (int i = 1; i < cellLength; ++i) {
		if (arithmSymbols.isIn(content[i])) {
			int before = i - 1;
			int after = i + 1;
			//"1 +"   <- the ' ' between them
			if (content[before] == ' ') {
				--before;
			}
			if (!symbolsBefore.isIn(content[before])) {
				return false;
			}
			//"+ 1"   <- the ' ' between them
			if (content[after] == ' ') {
				++after;
			}
			if (!symbolsAfter.isIn(content[after])) {
				return false;
			}
		}
	}
	return true;
}