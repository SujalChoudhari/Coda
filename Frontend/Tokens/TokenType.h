#pragma once
namespace Coda {
	namespace Frontend {
		enum class TokenType {
			// Literal Types
			BOOL,
			BYTE,
			INT,
			FLOAT,
			DOUBLE,
			LONG,
			IDENTIFIER,

			// Keywords
			LET,
			CONST,
			TYPE,

			// Operators
			EQUALS,
			SEMICOLON,
			BINARY_OPERATOR,
			OPEN_PAREN,
			CLOSE_PAREN,

			// End of File
			END_OF_FILE,

		};

	}
}