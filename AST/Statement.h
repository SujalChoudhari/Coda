#include "NodeType.h" 
#include <string> 
#include <vector>

class Statement {
public:
	NodeType kind;
};

class Program : public Statement {
public:
	std::vector<Statement> body;
	Program() {
		kind = NodeType::PROGRAM;
	}
};

class Expression : public Statement {

};

class BinaryExpression : public Expression {
public:
	Expression left;
	Expression right;
	std::string op;
	BinaryExpression(Expression l,
		Expression r, const std::string o) : left(l), right(r), op(o) {
		kind = NodeType::BINARY_EXPRESSION;
	}
};

class Identifier : public Expression {
public:
	std::string symbol;
	Identifier(const std::string s)
		: symbol(s) {
		kind = NodeType::IDENTIFIER;
	}
};

class NumericLiteral : public Expression {
public:
	std::string value;
	NumericLiteral(const std::string v)
		: value(v) {
		kind = NodeType::NUMERIC_LITERAL;
	}
};