#include "Constants.h"


namespace Coda {
	namespace Frontend {
		std::map<std::string, TokenType> KEYWORD = {
			{"let",TokenType::LET}
		};

		std::string DIGITS = "0123456789";
		std::string FIRST_VALID_DIGITS = DIGITS + ".";
		std::string DIGITS_EXTRA = "_,iIfFdDlLbB" + FIRST_VALID_DIGITS;

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
