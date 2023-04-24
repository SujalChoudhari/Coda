#include "Token.h"

Token::Token()
{
	type = TokenType::UNDEFINED;
	value = "";
}

Token::Token(TokenType type)
{
	this->type = type;
}

Token::Token(TokenType type, std::string value)
{
	this->type = type;
	this->value = value;
}
