#include "Parser.h"

namespace Coda {
	namespace Frontend {
		Node Parser::parseAssignmentExpression() {
			IF_ERROR_RETURN_NODE;

			Node left = parseObjectExpression(); // will switch it with objects

			if (mCurrentToken->type == TokenType::ASSIGN) {
				std::string functor = mCurrentToken->value;
				advance();

				Node right = parseAssignmentExpression();

				Node expr = Node(NodeType::ASSIGNMENT_EXPRESSION);
				expr.left = std::make_shared<Node>(left);
				expr.right = std::make_shared<Node>(right);
				expr.value = functor;
				expr.startPosition = left.startPosition;
				expr.endPosition = right.endPosition;

				return expr;
			}

			return left;
		}

		Node Parser::parseUnaryOperatorExpression()
		{
			std::string unaryOperator = "";
			Node unaryExpression;
			unaryExpression.startPosition = mCurrentToken->startPosition;
			// prefixed unary operators
			if (mCurrentToken->type == TokenType::UNARY_OPERATOR) {
				unaryOperator = mCurrentToken->value;
				advance();

				Node value = parseCallMemberExpression();

				unaryExpression.type = NodeType::UNARY_EXPRESSION;
				unaryExpression.value = unaryOperator;
				unaryExpression.left = std::make_shared<Node>(value);
				unaryExpression.endPosition = value.endPosition;
				return unaryExpression;
			}
			else {
				Node value = parseCallMemberExpression();
				if (mCurrentToken->type == TokenType::UNARY_OPERATOR) {
					unaryOperator = mCurrentToken->value;
					advance();

					if (mCurrentToken->value == "sizeof" || mCurrentToken->value == "typeof") {
						Error::Parser::raise("Cannot postfix operator " + mCurrentToken->value + ". Use as prefixed operator", mCurrentToken->startPosition);
					}

					unaryExpression.type = NodeType::UNARY_EXPRESSION;
					unaryExpression.value = unaryOperator;
					unaryExpression.left = std::make_shared<Node>(value);
					unaryExpression.endPosition = mCurrentToken->endPosition;
					return unaryExpression;
				}
				else {
					return value;
				}
			}
		}

		Node Parser::parseDeclaration(bool isConstant)
		{
			advance();

			Node declaration = Node(isConstant ? NodeType::CONSTANT_DECLARATION : NodeType::VARIABLE_DECLARATION);
			declaration.startPosition = mCurrentToken->startPosition;
			std::string identifier;
			std::string type;

			if (mCurrentToken->type != TokenType::TYPE)
			{
				type = "int";
			}
			else
			{
				type = mCurrentToken->value;
				advance();
				if (mCurrentToken->type != TokenType::IDENTIFIER) {
					Error::Lexer::raise("Invalid type '" + mCurrentToken->value + "' at ", mCurrentToken->startPosition);
				}
			}


			identifier = mCurrentToken->value;
			expect(TokenType::IDENTIFIER, "Expected a <identifier> at ");

			IF_ERROR_RETURN_NODE;


			if (mCurrentToken->type == TokenType::ASSIGN)
			{
				IF_ERROR_RETURN_NODE;

				declaration.value = type;
				declaration.left = std::make_shared<Node>(NodeType::IDENTIFIER, identifier);

				advance();
				declaration.right = std::make_shared<Node>(parseExpression());
			}
			else
			{
				if (isConstant) {
					Error::Runtime::raise("Expected an '=' in a 'const' definition at, ", mCurrentToken->endPosition);
					return declaration;
				}

				declaration.type = NodeType::VARIABLE_DECLARATION;
				declaration.left = std::make_shared<Node>(NodeType::IDENTIFIER, identifier);
				declaration.right = std::make_shared<Node>(NodeType::INTEGER_LITERAL, "0");
				declaration.value = type;
			}

			if (mCurrentToken->value == ";")
				advance();

			declaration.endPosition = mCurrentToken->endPosition;
			return declaration;
		}

		Node Parser::parseUnaryExpression() {
			std::string unaryOperator = mCurrentToken->value;
			advance();
			IF_ERROR_RETURN_NODE;

			// expressions has to be a number-like value
			Node expression = parseExpression();
			Node unaryExpression = Node(NodeType::UNARY_EXPRESSION, unaryOperator, std::make_shared<Node>(expression));
			return unaryExpression;
		}

		Node Parser::parsePrimaryExpression()
		{
			Node expression;
			expression.startPosition = mCurrentToken->startPosition;
			TokenType* type = &mCurrentToken->type;
			IF_ERROR_RETURN_NODE;
			if (*type == TokenType::IDENTIFIER) {
				expression.type = NodeType::IDENTIFIER;
				expression.value = mCurrentToken->value;
				advance();
			}
			else if (*type == TokenType::BYTE) {
				expression.type = NodeType::BYTE_LITERAL;
				expression.value = mCurrentToken->value;
				advance();
			}
			else if (*type == TokenType::INT) {
				expression.type = NodeType::INTEGER_LITERAL;
				expression.value = mCurrentToken->value;
				advance();
			}
			else if (*type == TokenType::LONG) {
				expression.type = NodeType::LONG_INT_LITERAL;
				expression.value = mCurrentToken->value;
				advance();
			}
			else if (*type == TokenType::FLOAT) {
				expression.type = NodeType::FLOATING_POINT_LITERAL;
				expression.value = mCurrentToken->value;
				advance();
			}
			else if (*type == TokenType::DOUBLE) {
				expression.type = NodeType::DOUBLE_LITERAL;
				expression.value = mCurrentToken->value;
				advance();
			}
			else if (*type == TokenType::CHAR) {
				expression.type = NodeType::CHARACTER_LITERAL;
				expression.value = mCurrentToken->value;
				advance();
			}
			else if (*type == TokenType::STRING) {
				expression.type = NodeType::STRING_LITERAL;
				expression.value = mCurrentToken->value;
				advance();
			}
			else if (*type == TokenType::BINARY_OPERATOR && (mCurrentToken->value == "+" || mCurrentToken->value == "-")) {
				expression = parseUnaryExpression();
			}
			else if (*type == TokenType::RETURN) {
				advance();
			}
			else if (*type == TokenType::OPEN_PAREN) {
				advance(); // skip the paren
				expression = parseExpression();
				expect(TokenType::CLOSE_PAREN,
					" Unexpected Token found '" +
					mCurrentToken->value + "' was found instead of ')' at, ");
			}
			else if (*type == TokenType::SEMICOLON) {
				advance(); // skip the semicolon
			}
			else if (*type == TokenType::JUMP) {
				expression.value = mCurrentToken->value;
				expression.type = NodeType::JUMP_EXPRESSION;
				if (mCurrentToken->value == "return") {
					advance();
					expression.left = std::make_shared<Node>(parseExpression());
					if (expression.left->type == NodeType::INVALID)
						expression.left = nullptr;
				}
				else
				{
					advance();
				}
			}
			else {
				expression.type = NodeType::INVALID;
				Coda::Error::Parser::raise("Invalid Token '" +
					mCurrentToken->value + "' was found at, ",
					mCurrentToken->startPosition);
			}

			expression.endPosition = mCurrentToken->endPosition;
			return expression;
		}
	}
}