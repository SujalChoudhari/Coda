#pragma once
namespace Coda {
	namespace FrontEnd {
		enum class NodeType {
			INVALID,
			PROGRAM,

			NUMERIC_LITERAL,
			NONE_LITERAL,

			IDENTIFIER,
			BINARY_EXPRESSION,
		};

	}
}