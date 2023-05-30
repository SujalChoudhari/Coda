#pragma once
#include <string>
#include <iostream>
#include "TokenType.h"
#include "../../Error/Position.h"

using namespace Coda::Error;

namespace Coda {
	namespace Frontend {
		/*
			A token is a building block of a program.
			Sourcecode is converted into a list of tokens.
			These tokens are then converted into a program.
		*/
		class Token {
		public:
			// The value of the token.
			std::string value;

			// The type of the token.
			TokenType type;

			// The position of the token in the source code.
			Position startPosition;

			// The position of the token in the source code.
			Position endPosition;
			friend std::ostream& operator<<(std::ostream& os, const Token& head);

		public:
			// Creates a new token with all the required information.
			Token(TokenType type, std::string value, Position startPosition, Position endPosition)
				:type(type), value(value), startPosition(startPosition), endPosition(endPosition) {}

			// Creates a new token withouth the end position.
			Token(TokenType type, std::string value, Position startPosition)
				:type(type), value(value), startPosition(startPosition), endPosition(startPosition) {}
		};
	} // namespace Frontend
} // namespace Coda
