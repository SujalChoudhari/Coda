#include "Parser.h"
#include "../AST/Node.h"
Parser::Parser()
{
	mCurrentIndex = -1;
	mCurrentToken = nullptr;
	mTokens = nullptr;
}

void Parser::advance()
{
	mCurrentIndex++;
	if (mCurrentIndex < mTokens->size()
		&& mTokens->at(mCurrentIndex).type != TokenType::END_OF_FILE)
		mCurrentToken = &mTokens->at(mCurrentIndex);
}

void Parser::parse(std::vector<Token> tokens, Node* outProgram)
{
	mTokens = &tokens;
	advance();
	while (mCurrentIndex < mTokens->size()
		&& mCurrentToken->type != TokenType::END_OF_FILE) {
		Node* s = new Node();
		parseStatement(s);
		outProgram->body.emplace_back(*s);
		advance();
	}
}



void Parser::parseStatement(Node* outStatement)
{
	parseExpression(outStatement);
}

void Parser::parseExpression(Node* outExpression)
{
	parseAdditiveExpression(outExpression);
}

void Parser::parsePrimaryExpression(Node* outExpression)
{
	switch (mCurrentToken->type) {

	case TokenType::IDENTIFIER:
		outExpression->type = NodeType::IDENTIFIER;
		outExpression->value = mCurrentToken->value;
		break;
	case TokenType::NUMBER:
		outExpression->type = NodeType::NUMERIC_LITERAL;
		outExpression->value = mCurrentToken->value;
		break;

	default:
		outExpression->type = NodeType::INVALID;
		break;
	}
}

void Parser::parseAdditiveExpression(Node* outExpression)
{
	std::unique_ptr<Node>left = std::make_unique<Node>();
	parsePrimaryExpression(left.get());

	while (mCurrentToken->value == "+" || mCurrentToken->value == "-") {
		advance();
		std::string value = mCurrentToken->value;

		std::unique_ptr<Node>right = std::make_unique<Node>();
		parsePrimaryExpression(right.get());
		left->type = NodeType::BINARY_EXPRESSION;
		left->value = value;
		left->right = std::move(right);
		left->left = std::make_unique<Node>(*left.get());
		// Broken Code
	}

	outExpression = left.get();
}

void Parser::parseMultiplacativeExpression(Node* outExpression)
{
	parsePrimaryExpression(outExpression);
}

