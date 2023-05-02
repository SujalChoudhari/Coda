#pragma once
#include <iostream>
#include "Position.h"
#include "ErrorManager.h"
namespace LexerError {
	void raiseIllegalCharacterError(const char character, const Position& pos);
	
}

namespace ParserError {
	void raiseExpectedTokenError(const std::string expected, const std::string got, const Position& pos);
}

