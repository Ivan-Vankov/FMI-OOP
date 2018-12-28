#include "Parser.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Please enter the admin's age." << std::endl;
		return 1;
	}
	if (!Parser::isUnsigned(argv[1])) {
		std::cerr << "Please enter a valid age for the admin." << std::endl;
		return 2;
	}
	String input;
	FmiBook fmiBook(atoi(argv[1]), "Admin");
	while (true) {
		try {
			input.readLine(std::cin);
			input.removeUnnecessarySymbols(" \t");
			if (!Parser::parseAndExecute(input, fmiBook)) {
				std::cout << "Successfully exited program." << std::endl;
				break;
			}
		}
		catch (std::exception& e) {
			std::cerr << e.what();
		}
		input.free();
	}

	return 0;
}