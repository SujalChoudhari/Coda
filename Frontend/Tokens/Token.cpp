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

std::ostream& operator<<(std::ostream& os, const Token& head)
{
	os << "(" << static_cast<int>(head.type) << ": " << head.value << ")";
	return os;
}
