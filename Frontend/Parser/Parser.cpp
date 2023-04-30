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
		Node s = parseStatement();
		outProgram->body.emplace_back(s);
		advance();
	}
}



Node Parser::parseStatement()
{
	return parseExpression();
}

Node Parser::parseExpression()
{
	return parseAdditiveExpression();
}

Node Parser::parsePrimaryExpression()
{
	Node expression;

	switch (mCurrentToken->type) {
	case TokenType::IDENTIFIER:
		expression.type = NodeType::IDENTIFIER;
		expression.value = mCurrentToken->value;
		break;
	case TokenType::NUMBER:
		expression.type = NodeType::NUMERIC_LITERAL;
		expression.value = mCurrentToken->value;
		break;
	default:
		expression.type = NodeType::INVALID;
		break;
	}

	advance();

	return expression;
}


Node Parser::parseAdditiveExpression()
{
	Node left = parsePrimaryExpression();

	while (mCurrentToken->value == "+" || mCurrentToken->value == "-") {
		Token operatorToken = *mCurrentToken;
		advance();

		Node right = parsePrimaryExpression();

		Node binaryExpression;
		binaryExpression.type = NodeType::BINARY_EXPRESSION;
		binaryExpression.value = operatorToken.value;
		binaryExpression.left = std::make_shared<Node>(left);
		binaryExpression.right = std::make_shared<Node>(right);

		left = binaryExpression;
	}

	return left;
}


Node Parser::parseMultiplacativeExpression()
{
	return parsePrimaryExpression();
}

