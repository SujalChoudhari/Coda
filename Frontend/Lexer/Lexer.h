#pragma once
#include <string>
#include <vector>
#include "../Tokens/Token.h"


class Lexer {
public:
	Lexer();
	void tokenise(std::string sourceCode, std::vector<Token>* outTokens);

private:
	std::vector<Token> mTokens = {};
};