#pragma once
namespace Coda {
	namespace Frontend {
		enum class TokenType {
			// Literal Types
			INT,
			FLOAT,
			DOUBLE,
			BYTE,
			LONG,
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

	}
}