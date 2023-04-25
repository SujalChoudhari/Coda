#pragma once

enum class NodeType {
	PROGRAM,
	NUMERIC_LITERAL,
	IDENTIFIER,
	BINARY_EXPRESSION
};

class Statement {
	NodeType kind;
};