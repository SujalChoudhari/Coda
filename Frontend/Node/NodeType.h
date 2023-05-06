#pragma once
namespace Coda {
	namespace Frontend {
		enum class NodeType {
			
			// Statements
			INVALID,
			PROGRAM,

			VARIABLE_DECLARATION,
			CONSTANT_DECLARATION,

			// Expressions
				//Literals
			BYTE_LITERAL,
			INTEGER_LITERAL,
			LONG_INT_LITERAL,
			FLOATING_POINT_LITERAL,
			DOUBLE_LITERAL,

				//Other
			IDENTIFIER,
			BINARY_EXPRESSION,
			TYPE_LITERAL,
		};

	}
}