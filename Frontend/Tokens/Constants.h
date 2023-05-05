#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include "TokenType.h"

namespace Coda {
	namespace FrontEnd {
		std::map<std::string, TokenType>KEYWORD = {
			{"let",TokenType::LET},
			{"none",TokenType::NONE},
			{"undefined",TokenType::UNDEFINED}
		};

		std::string DIGITS = "0123456789";
		std::string DIGITS_EXTRA = "._,iIfFdDlLbB" + DIGITS;

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