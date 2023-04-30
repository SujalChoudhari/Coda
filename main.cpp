#include <iostream>

#include "Frontend/Lexer/Lexer.h"
#include "Frontend/Tokens/Token.h"
#include "Frontend/Tokens/TokenType.h"
#include "Frontend/AST/Node.h"
#include "Frontend/Parser/Parser.h"


int repl() {
	Lexer lexer = Lexer();
	Parser parser = Parser();
	std::string source;
	std::unique_ptr<Node> program;
	std::vector<Token> tokens;
	program = std::make_unique<Node>();
	tokens = std::vector<Token>();

	std::cout << ">> ";
	std::getline(std::cin, source);

	lexer.tokenise(source, &tokens);
	parser.parse(tokens, program.get());

	std::cout << *program.get() << std::endl;

	return 0;
}

int main() {

	return repl();
}