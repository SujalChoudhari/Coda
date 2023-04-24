#pragma once
#include <string>
#include <vector>
#include "../Tokens/Token.h"


class Lexer {
public:
	Lexer();
	std::vector<Token> tokenise(std::string sourceCode);

private:
	std::vector<Token> mTokens = {};
};