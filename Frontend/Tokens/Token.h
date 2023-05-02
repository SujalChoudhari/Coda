#pragma once
#include <string>
#include <iostream>
#include "TokenType.h"
#include "../../Error/Position.h"

class Token {
public:
	std::string value;
	TokenType type;
	Position startPosition;
	Position endPosition;
	friend std::ostream& operator<<(std::ostream& os, const Token& head);

public:
	Token(TokenType type, std::string value, Position startPosition, Position endPosition)
		:type(type),value(value),startPosition(startPosition),endPosition(endPosition) {}

	Token(TokenType type, std::string value, Position startPosition)
		:type(type), value(value), startPosition(startPosition), endPosition(startPosition) {}
};

