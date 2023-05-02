#pragma once

enum class NodeType {
	INVALID,
	PROGRAM,
	NUMERIC_LITERAL,
	NONE_LITERAL,
	IDENTIFIER,
	BINARY_EXPRESSION,
};

class Statement {
	NodeType kind;
};