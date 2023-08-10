#include "Constants.h"


namespace Coda {
	namespace Frontend {
		std::map<std::string, TokenType> KEYWORD =
		{
			{"let",TokenType::LET},
			{"const",TokenType::CONST},
			{"def",TokenType::DEF},
			{"return",TokenType::RETURN},
			{"if",TokenType::IF},
			{"elif",TokenType::ELIF},
			{"else",TokenType::ELSE},
			{"int",TokenType::TYPE},
			{"bool",TokenType::TYPE},
			{"byte",TokenType::TYPE},
			{"float",TokenType::TYPE},
			{"double",TokenType::TYPE},
			{"string",TokenType::TYPE},
			{"long",TokenType::TYPE}
		};

		std::string DIGITS = "0123456789";
		std::string FIRST_VALID_DIGITS = DIGITS + ".";
		std::string DIGITS_EXTRA = "_iIfFdDlLbB" + FIRST_VALID_DIGITS;

		const std::unordered_map<char, TokenType> DIGIT_EXTENTIONS = {
			{'f', TokenType::FLOAT},
			{'F', TokenType::FLOAT},
			{'d', TokenType::DOUBLE},
			{'D', TokenType::DOUBLE},
			{'l', TokenType::LONG},
			{'L', TokenType::LONG},
			{'b', TokenType::BYTE},
			{'B', TokenType::BYTE}
		};
	}
}
