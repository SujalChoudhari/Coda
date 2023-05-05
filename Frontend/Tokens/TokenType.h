#pragma once
namespace Coda {
	namespace FrontEnd {
		enum class TokenType {
			// Literal Types
			NONE,
			UNDEFINED,
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