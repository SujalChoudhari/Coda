#pragma once

#include <map>
#include <string>
#include "TokenType.h"

std::map<std::string, TokenType>KEYWORD = {
	{"let",TokenType::LET},
	{"none",TokenType::NONE},
	{"undefined",TokenType::UNDEFINED}
};