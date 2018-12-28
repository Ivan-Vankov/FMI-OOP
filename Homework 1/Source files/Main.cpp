#include "Bookkeeper.h"
#include "Parser.h"

int main(){
	String input;
	Bookkeeper bookkeeper;
	while (true) {
		try {
			input.readLine(std::cin);
			input.removeUnnecessarySymbols(" \t");
			if (!Parser::parseAndExecute(bookkeeper, input)){ break; }
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		input.free();
	}

	return 0;
}