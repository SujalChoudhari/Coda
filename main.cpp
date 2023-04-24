#include "Lexer/Lexer.h"
#include "Tokens/Token.h"
#include "Tokens/TokenType.h"
#include <iostream>

int main() {

	Lexer lexer = Lexer();

	std::string code = "let 1 + 2 ) 3";

	std::vector<Token> tokens = lexer.tokenise(code);
	for (Token t : tokens) {
		std::cout << t.value << std::endl;
	}
	return 0;
}