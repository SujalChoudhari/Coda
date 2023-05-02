#pragma once

#include <map>
#include <string>
#include "TokenType.h"

namespace Coda {
	namespace FrontEnd {
		std::map<std::string, TokenType>KEYWORD = {
			{"let",TokenType::LET},
			{"none",TokenType::NONE},
			{"undefined",TokenType::UNDEFINED}
		};

	}
}