#pragma once
namespace Coda {
	namespace Frontend {
		// The type of the node.
		enum class NodeType {

			// Statements
			INVALID,
			ARGUMENT_LIST,

			VARIABLE_DECLARATION,
			CONSTANT_DECLARATION,
			BLOCK_STATEMENT,

			// Expressions
				//Literals
			BYTE_LITERAL,
			INTEGER_LITERAL,
			LONG_INT_LITERAL,
			FLOATING_POINT_LITERAL,
			DOUBLE_LITERAL,
			CHARACTER_LITERAL,
			STRING_LITERAL,
			PROPERTY,
			OBJECT_LITERAL,
			FUNCTION_LITERAL,

				//Other
			IDENTIFIER,
			UNARY_EXPRESSION,
			BINARY_EXPRESSION,
			IF_EXPRESSION,
			FOR_EXPRESSION,
			ASSIGNMENT_EXPRESSION,
			MEMBER_EXPRESSION,
			CALL_EXPRESSION

		};

	} // namespace Frontend
} // namespace Coda