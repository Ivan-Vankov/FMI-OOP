#include "Parser.h"

int main() {
	String userInput;
	Tables openedFiles;
	while (true) {
		try {
			userInput.readLine(std::cin);
			userInput.removeUnnecessarySymbols("\t ");
			if (!Parser::parseAndExecute(openedFiles, userInput)) {
				std::cout << "Exiting the program.\n";
				break;
			}
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		userInput.free();
	}

	return 0;
}