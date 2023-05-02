#pragma once

enum class TokenType {
	// Literal Types
	NONE,
	UNDEFINED,
	NUMBER,
	IDENTIFIER,

	// Keywords
	LET,

	// Operators
	EQUALS,
	BINARY_OPERATOR,
	OPEN_PAREN,
	CLOSE_PAREN,

	// End of File
	END_OF_FILE,

};

