#pragma once
#include <string>
#include <vector>
#include "../Tokens/Token.h"
#include "../../Error/Position.h"


class Lexer {
public:
	Lexer();
	std::vector<Token> tokenise(std::string sourceCode);

private:
	std::vector<Token> mTokens = {};
	std::string mSourceCode;
	unsigned int mCurrentIndex = -1;
	char mCurrentChar;
	Position mCurrentPosition;

private:
	void advance();
	void buildNumbers();
	void buildIdentifiers();
};