#include "Parser.h"
#include "../Node/Node.h"
#include "../../Error/Error.h"
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

Program Parser::parse(std::vector<Token> tokens)
{
	Program program = Program();
	mTokens = &tokens;
	advance();
	while (mCurrentIndex < mTokens->size()
		&& mCurrentToken->type != TokenType::END_OF_FILE) {
		Node s = parseStatement();
		program.body.emplace_back(s);
		advance();
	}
	return program;
}



Node Parser::parseStatement()
{
	return parseExpression();
}

Node Parser::parseExpression()
{
	return parseAdditiveExpression();
}

Node Parser::parseAdditiveExpression()
{
	Node left = parseMultiplacativeExpression();

	while (mCurrentToken->value == "+" || mCurrentToken->value == "-") {
		Token operatorToken = *mCurrentToken;
		advance();

		Node right = parseMultiplacativeExpression();

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
	Node left = parsePrimaryExpression();

	while (mCurrentToken->value == "/" || mCurrentToken->value == "*" || mCurrentToken->value == "%") {
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


Node Parser::parsePrimaryExpression()
{
	Node expression;
	TokenType* type = &mCurrentToken->type;

	if (*type == TokenType::IDENTIFIER) {

		expression.type = NodeType::IDENTIFIER;
		expression.value = mCurrentToken->value;
	}
	else if (*type == TokenType::NONE) {
		advance();
		expression.type = NodeType::NONE_LITERAL;
		expression.value = "none";
	}
	else if (*type == TokenType::NUMBER) {
		expression.type = NodeType::NUMERIC_LITERAL;
		expression.value = mCurrentToken->value;
	}
	else if (*type == TokenType::OPEN_PAREN) {

		advance(); // skip the paren
		expression = parseExpression();
		if (mCurrentToken->type != TokenType::CLOSE_PAREN) {
			ParserError::raiseUnexpectedTokenError(")", mCurrentToken->value, mCurrentToken->startPosition);
			exit(1);
		}
		else {
			advance(); // eat the closing paren
		}
		return expression;

	}
	else {
		expression.type = NodeType::INVALID;
		ParserError::raiseInvalidTokenFoundError(mCurrentToken->value, mCurrentToken->startPosition);
		exit(1);
	}

	advance();

	return expression;
}


