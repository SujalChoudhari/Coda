#pragma once
namespace Coda {
	namespace Frontend {
		/*
			A token can be of multiple types.
			Each type has a different meaning, and a different purpose.
		*/
		enum class TokenType {
			INVALID,
			
			// Literal Types
			BOOL,
			BYTE,
			INT,
			FLOAT,
			DOUBLE,
			LONG,
			CHAR,
			STRING,
			IDENTIFIER,

			// Keywords
			LET,
			CONST,
			TYPE,
			DEF,
			RETURN,

			// Operators
			ASSIGN,
			SEMICOLON,
			COMMA,
			DOT,
			COLON,
			BINARY_OPERATOR,
			OPEN_PAREN,
			CLOSE_PAREN,
			OPEN_BRACKET,
			CLOSE_BRACKET,
			OPEN_BRACE,
			CLOSE_BRACE,

			// End of File
			END_OF_FILE,

		};

	}
}