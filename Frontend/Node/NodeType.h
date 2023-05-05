#pragma once
namespace Coda {
	namespace Frontend {
		enum class NodeType {
			INVALID,
			PROGRAM,

			BYTE_LITERAL,
			INTEGER_LITERAL,
			LONG_INT_LITERAL,
			FLOATING_POINT_LITERAL,
			DOUBLE_LITERAL,

			IDENTIFIER,
			BINARY_EXPRESSION,
		};

	}
}