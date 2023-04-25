#include "Lexer/Lexer.h"
#include "Tokens/Token.h"
#include "Tokens/TokenType.h"
#include <iostream>

int main() {

	Lexer lexer = Lexer();

	std::string code = "let x = 32 * (4 / 4)";

	std::vector<Token> tokens = lexer.tokenise(code);
	for (Token t : tokens) {
		std::cout << "(" <<  t.value  << ":" <<(int) t.type << ")" << std::endl;
	}
	return 0;
}