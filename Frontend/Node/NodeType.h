#pragma once
namespace Coda {
	namespace FrontEnd {
		enum class NodeType {
			INVALID,
			PROGRAM,

			NONE_LITERAL,
			UNDEFINED_LITERAL,
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