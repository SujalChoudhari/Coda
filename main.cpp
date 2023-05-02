#include <iostream>

#include "Frontend/Lexer/Lexer.h"
#include "Frontend/Tokens/Token.h"
#include "Frontend/Tokens/TokenType.h"
#include "Frontend/Node/Node.h"
#include "Frontend/Node/Program.h"
#include "Frontend/Parser/Parser.h"
#include "Error/ErrorManager.h"


int repl() {
	while (1) {


		std::string source;
		std::cout << ">> ";
		std::getline(std::cin, source);
		if (source == ":q" || source == "exit" || source == "quit") break;

		Lexer lexer = Lexer();
		std::vector<Token> tokens = lexer.tokenise(source);

		if (!ErrorManager::isSafe())
			continue;
		Parser parser = Parser();
		Program program = parser.parse(tokens);

		std::cout << program << std::endl;
	}
	return 0;
}

int main() {

	return repl();
}