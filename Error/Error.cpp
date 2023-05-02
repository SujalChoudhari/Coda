#include "Error.h"


std::ostream& operator<<(std::ostream& os, const Position& head) {
	os << "Line: " << head.line
		<< " Character: " << head.character;
	return os;
}

void LexerError::raiseIllegalCharacterError(const char character, const Position& pos)
{
	ErrorManager::raiseLexerError();
	std::cout << "[LEXER]: Illegal Character found. Character '"
		<< character
		<< "' was found at, "
		<< pos << std::endl;

}

void ParserError::raiseUnexpectedTokenError(
	const std::string expected,
	const std::string got,
	const Position& pos)
{
	ErrorManager::raiseParserError();
	std::cout << "[PARSER]: Expected a Token '"
		<< expected
		<< "' but got '"
		<< got
		<< "' instead, at "
		<< pos
		<< std::endl;
}

void ParserError::raiseInvalidTokenFoundError(const std::string found, const Position& pos)
{
	ErrorManager::raiseParserError();
	std::cout << "[PARSER]: Invalid Token found. Token '"
		<< found
		<< "' was found at, "
		<< pos << std::endl;

}

