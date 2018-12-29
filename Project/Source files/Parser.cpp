#include "Parser.h"

bool Parser::parseAndExecute(Tables& openedFiles, const String& input) {
	int wordCount = input.countWords();
	// Do nothing if there are no words in the command
	if (!wordCount) {
		return true;
	}
	// Get the first word of the input
	String command = input.wordNumber(1);
	// Search if that word matches any of the know commands
	if (!strcmp(command, "print")) {
		print(openedFiles, input, wordCount);
		return true;
	}
	if (!strcmp(command, "editcell")) {
		editcell(openedFiles, input, wordCount);
		return true;
	}
	if (!strcmp(command, "open")) {
		open(openedFiles, input, wordCount);
		return true;
	}
	if (!strcmp(command, "close")) {
		close(openedFiles, input, wordCount);
		return true;
	}
	if (!strcmp(command, "save")) {
		save(openedFiles, input, wordCount);
		return true;
	}
	if (!strcmp(command, "saveas")) {
		saveas(openedFiles, input, wordCount);
		return true;
	}
	if (!strcmp(command, "exit")) {
		if (wordCount == 1) {
			return false;
		}
	}
	std::cerr << "Unknown command." << std::endl;
	return true;
}

void Parser::print(Tables& openedFiles, const String& input, int wordCount) {
	// When the command is just "print" but there is more than one opened file
	if (openedFiles.size() > 1 && wordCount == 1) {
		std::cerr << "You have to specify which of the opened files to print.\n";
		return;
	}
	if (openedFiles.size() > 2) {
		std::cerr << "Too many words in print command.";
		return;
	}
	std::string path;
	// When there in exacly 1 opened file
	if (wordCount == 1) {
		path = toPath(openedFiles.firstFilename());
	}
	// When there is more than 1 opened files
	else {
		path = toPath(input.wordNumber(2));
		if (!openedFiles.tableIsOpened(path)) {
			std::cerr << "File " << path << " in not opened.\n";
			return;
		}
	}
	openedFiles.findTable(path).print();
}

void Parser::editcell(Tables& openedFiles, const String& input, int wordCount) {
	if (wordCount < 3) {
		std::cerr << "Too few words in editcell command.";
		return;
	}
	std::string path;
	String RandC;
	String cellContent;
	//editcell R0C0 "abc"
	//editcell R1C1 "abc hdf"
	if (input.wordNumber(3)[0] == '\"' || input.wordNumber(3)[0] == '=') {//opitva se da otgatne ako ima potencialen String ili formula s nqkolko domi v nego
		cellContent + input.wordNumber(3);
		for (int i = 4; i <= wordCount; ++i) {
			cellContent + " " + input.wordNumber(i);
		}
		wordCount = 3;
	}
	if (wordCount > 3 && (input.wordNumber(4)[0] == '\"' || input.wordNumber(3)[0] == '=')) {//otnovo se opitva da otgatne potencialen String ili formula 
		cellContent + input.wordNumber(4);
		for (int i = 5; i <= wordCount; ++i) {
			cellContent + " " + input.wordNumber(i);
		}
		wordCount = 4;
	}
	//kak da se zameni wordCount == 3
	//editcell "asf ""as" "zel bebka kek"
	//editcell R0C0 R1C1 "ayy lmao" //ne moje da se zameni s prowerka na duma 2 i 3 zashtoto dvete mogat da sa
	//filepaths i RandC-s ednovremenno
	//editcell R0C0 R1C1 "ay\" l\"mao" 
	if (openedFiles.size() == 0) {// kato nqma otvoren fajl
		std::cerr << "There are no opened files to edit a cell in.\n";
		return;
	}
	if (openedFiles.size() > 1 && wordCount == 3) {// editcell R<num>C<num> "abc"(kato ima poveche ot 1 otvoren fajl)
		std::cerr << "You have to specify which of the opened files to edit a cell in.\n";
		return;
	}

	int inputLength = input.getLength();
	if (wordCount == 3) {//kato ima tochno 1 otvoren fail
		path = toPath(openedFiles.firstFilename());
		RandC = input.wordNumber(2);
		String temp = input.wordNumber(3);
		if (temp[0] != '\"') {//ako 3tata duma ne e potencialen String
			cellContent = temp;
		}
	}
	else {
		path = toPath(input.wordNumber(2));//kato ima >1 otvoren fail
		if (!openedFiles.tableIsOpened(path)) {
			std::cerr << "File " << path << " in not opened.\n";
			return;
		}
		RandC = input.wordNumber(3);
		String temp = input.wordNumber(4);
		if (temp[0] != '\"') {//ako 4tata duma ne e potencialen String
			cellContent = temp;
		}
	}

	int RandCLength = RandC.getLength();
	if (RandCLength < 4 || RandC[0] != 'R' || !(RandC[1] >= '0' && RandC[1] <= '9')) {
		std::cerr << "Incorrect row and column.\n";
		return;//posledniq sluchai na if-a e tuk zashtoto inache dolniq for moje da prihvane RC<num> za vqrno
	}
	for (int i = 2; i < RandCLength; ++i) {//razglejda dali e pravilno RandC-to
		if (RandC[i] == 'C') {
			int j = i + 1;
			if (j == RandCLength) {// R<num>C sluchaq
				std::cerr << "Incorrect row and column.\n";
				return;
			}
			for (; j < RandCLength; ++j) {
				if (!(RandC[j] >= '0' && RandC[j] <= '9')) {
					std::cerr << "Incorrect row and column.\n";
					return;
				}
			}
			break;
		}
		if (!(RandC[i] >= '0' && RandC[i] <= '9')) {// R<!num> sluchaq
			std::cerr << "Incorrect row and column.\n";
			return;
		}
	}
	openedFiles.setCell(path, RandC, cellContent);
	std::cout << "Cell at " << RandC << " has been edited.\n";
}

void Parser::open(Tables& openedFiles, const String& input, int wordCount) {
	if (wordCount == 1) {
		std::cerr << "Please add a file to open.\n";
		return;
	}
	if (wordCount > 2) {
		std::cerr << "Please add only one file to open.\n";
		return;
	}
	std::string path = toPath(input.wordNumber(2));
	if (openedFiles.tableIsOpened(path)) {
		std::cerr << "File " << path << " has already been opened.\n";
		return;
	}
	if (!fileExists(path)) {
		std::ofstream ofile(path);
		if (!ofile.good()) {
			std::cerr << "Couldn't create file " << path << ".\n";
			return;
		}
		openedFiles.addTable(path);
		std::cerr << "File " << path << " has been created.\n";
		return;
	}
	openedFiles.addTable(path);
	std::cout << "File " << path << " has been opened.\n";
}

void Parser::close(Tables& openedFiles, const String& input, int wordCount) {
	// When there is no opened file
	if (openedFiles.size() == 0) {
		std::cerr << "There are no opened files to close.\n";
		return;
	}
	// When there is more than one opened file  and the input is just "close"
	if (openedFiles.size() > 1 && wordCount == 1) {
		std::cerr << "You have to specify which of the opened files to close.\n";
		return;
	}
	std::string path;
	// When there is exactly one opened file
	if (wordCount == 1) {
		path = toPath(openedFiles.firstFilename());
	}
	else {
		path = toPath(input.wordNumber(2));
		if (!openedFiles.tableIsOpened(path)) {
			std::cerr << "File " << path << " in not opened.\n";
			return;
		}
	}
	openedFiles.removeTable(path);
	std::cout << "File " << path << " has been closed.\n";
}

void Parser::save(Tables& openedFiles, const String& input, int wordCount) {
	// When there is no opened file
	if (openedFiles.size() == 0) {
		std::cerr << "There are no opened files to save.\n";
		return;
	}
	// When there is more than one opened file and the input is just "save"
	if (openedFiles.size() > 1 && wordCount == 1) {
		std::cerr << "You have to specify which of the opened files to save.\n";
		return;
	}
	std::string path;
	// When there is exactly one opened file
	if (wordCount == 1) {
		path = toPath(openedFiles.firstFilename());
	}
	else {
		path = toPath(input.wordNumber(2));
		if (!openedFiles.tableIsOpened(path)) {
			std::cerr << "File " << path << " in not opened.\n";
			return;
		}
	}
	openedFiles.findTable(path).saveToFile(path);
	std::cout << path << " has been saved.\n";
}

void Parser::saveas(Tables& openedFiles, const String& input, int wordCount) {
	// When there is no opened file
	if (openedFiles.size() == 0) {
		std::cerr << "There are no opened files to save.\n";
		return;
	}
	// When there is more than one opened file
	if (openedFiles.size() > 1 && input.countWords() == 2) {
		std::cerr << "You have to specify which of the opened files to save.\n";
		return;
	}
	std::string initialPath;
	// saveas init dest (When no such path has been visited)
	if (openedFiles.size() > 1) {
		initialPath = toPath(input.wordNumber(2));
		if (!openedFiles.tableIsOpened(initialPath)) {
			std::cerr << "File " << initialPath << " in not opened.\n";
			return;
		}
	}
	String destPath;
	// saveas abc.txt
	if (wordCount == 2) {
		initialPath = toPath(openedFiles.firstFilename());
		destPath = toPath(input.wordNumber(2));
	}
	else {
		destPath = toPath(input.wordNumber(3));
	}
	openedFiles.findTable(initialPath).saveToFile(destPath);
	std::cout << "File " << initialPath << " has been saved to " << destPath << ".\n";
}

bool Parser::checkLine(const String& input, String& line, int& lineNumber) {
	line = input.wordNumber(2);
	if (!isInteger(line)) {
		std::cerr << "Second word should be an integer." << std::endl;
		return false;
	}
	lineNumber = atoi(line);
	return true;
}

bool Parser::wordAmmountCheck(int wordCount, int wordAmmount) {
	if (wordCount < wordAmmount) {
		std::cerr << "Not enough words in command." << std::endl;
		return false;
	}
	if (wordCount > wordAmmount) {
		std::cerr << "Too many words in command." << std::endl;
		return false;
	}
	return true;
}

bool Parser::isInteger(const String& str) {
	if (str[0] == '-') {
		return false;
	}
	int length = str.getLength();
	if ((str[0] == '+' && length == 1) || (str[0] < '0' || str[0] > '9')) {
		return false;
	}
	for (int i = 1; i < length; ++i) {
		if (str[0] < '0' || str[0] > '9') {
			return false;
		}
	}
	return true;
}

bool Parser::isOrder(const String& order) {
	if (!strcmp(order, "desc") ||
		!strcmp(order, "asc")) {
		return true;
	}
	return false;
}

Order Parser::toOrder(const String& order) {
	if (!strcmp(order, "desc")) {
		return desc;
	}
	if (!strcmp(order, "asc")) {
		return asc;
	}
	else {
		throw std::exception("Unknown ordering.");
		// So that the compiler shuts up
		return desc;
	}
}

bool Parser::fileExists(const String& fileName) {
	return std::ifstream(fileName).good();
}

String Parser::toPath(const String& path) {
	if (hasExtentionTxt(path)) {
		return path;
	}
	else {
		String temp(path);
		return temp + ".txt";
	}
}

bool Parser::hasExtentionTxt(const String& path) {
	int n = path.getLength();
	if (n < 4) {
		return false;
	}
	else if (path[n - 1] == 't'&&
		path[n - 2] == 'x'&&
		path[n - 3] == 't'&&
		path[n - 4] == '.') {
		return true;
	}
	else {
		return false;
	}
}