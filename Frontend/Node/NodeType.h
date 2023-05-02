#pragma once

enum class NodeType {
	PROGRAM,
	NUMERIC_LITERAL,
	IDENTIFIER,
	BINARY_EXPRESSION,
	INVALID
};

class Statement {
	NodeType kind;
};