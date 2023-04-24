#pragma once
#include <string>
#include "TokenType.h"
struct Token {
public:
	Token();
	Token(TokenType type);
	Token(TokenType type, std::string value);

public:
	std::string value;
	TokenType type;
};

