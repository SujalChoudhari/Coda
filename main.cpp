#include <iostream>

#include "Lexer/Lexer.h"
#include "Tokens/Token.h"
#include "Tokens/TokenType.h"
#include "AST/Node.h"

int main() {

	Lexer lexer = Lexer();

	std::string code = "let x = 32 * (4 / 4)";

	std::vector<Token> tokens;
	lexer.tokenise(code,&tokens);
	std::cout << sizeof Node <<std::endl;
	for (Token t : tokens) {
		std::cout << "(" <<  t.value  << ":" <<(int) t.type << ")" << std::endl;
	}
	return 0;
}