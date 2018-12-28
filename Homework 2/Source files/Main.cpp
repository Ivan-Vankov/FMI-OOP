#include "TextEditor.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cerr << "Please add a text file to open." << std::endl;
		return -1;
	}
	if (argc > 2) {
		std::cerr << "Too many command line arguements." << std::endl;
		return -2;
	}
	TextEditor editor(argv[argc - 1]);
	String input;
	while (true) {
		try {
			input.readLine(std::cin);
			input.removeUnnecessarySymbols(" \t");
			if (!Parser::parseAndExecute(editor, input)) { break; }
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		input.free();
	}
	return 0;
}