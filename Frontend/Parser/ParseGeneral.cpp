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
			IF_ERROR_RETURN_NODE;
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
			IF_ERROR_RETURN_NODE;
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

				if (mCurrentToken->value == ":=") {
					declaration.properties["copy"] = std::make_shared<Node>(NodeType::INVALID, "true");
				}

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

		Node Parser::parsePrimaryExpression() {
			Node expression;
			expression.startPosition = mCurrentToken->startPosition;
			TokenType type = mCurrentToken->type;

			IF_ERROR_RETURN_NODE;

			switch (type) {
			case TokenType::IDENTIFIER:
			case TokenType::BYTE:
			case TokenType::INT:
			case TokenType::LONG:
			case TokenType::FLOAT:
			case TokenType::DOUBLE:
			case TokenType::CHAR:
			case TokenType::STRING:
				parseLiteralExpression(expression, type);
				break;

			case TokenType::BINARY_OPERATOR:
				if (isBinaryOperatorToken("+") || isBinaryOperatorToken("-")) {
					expression = parseUnaryExpression();
				}
				break;

			case TokenType::OPEN_PAREN:
				advance();
				expression = parseExpression();
				expect(TokenType::CLOSE_PAREN, " Unexpected Token found '" + mCurrentToken->value + "' was found instead of ')' at, ");
				break;

			case TokenType::SEMICOLON:
				advance();
				break;

			case TokenType::JUMP:
				parseJumpExpression(expression);
				break;

			default:
				handleInvalidToken();
				break;
			}

			expression.endPosition = mCurrentToken->endPosition;
			return expression;
		}

		void Parser::parseLiteralExpression(Node& expression, TokenType type) {
			expression.type = getTokenTypeAsNodeType(type);
			expression.value = mCurrentToken->value;
			advance();
		}


		void Parser::handleInvalidToken() {
			Node expression;
			expression.type = NodeType::INVALID;
			Coda::Error::Parser::raise("Invalid Token '" + mCurrentToken->value + "' was found at, ", mCurrentToken->startPosition);
		}

		bool Parser::isBinaryOperatorToken(const std::string& op) {
			return mCurrentToken->value == op && mCurrentToken->type == TokenType::BINARY_OPERATOR;
		}


		NodeType Parser::getTokenTypeAsNodeType(TokenType tokenType) {
			switch (tokenType) {
			case TokenType::IDENTIFIER:          return NodeType::IDENTIFIER;
			case TokenType::BYTE:                return NodeType::BYTE_LITERAL;
			case TokenType::INT:                 return NodeType::INTEGER_LITERAL;
			case TokenType::LONG:                return NodeType::LONG_INT_LITERAL;
			case TokenType::FLOAT:               return NodeType::FLOATING_POINT_LITERAL;
			case TokenType::DOUBLE:              return NodeType::DOUBLE_LITERAL;
			case TokenType::CHAR:                return NodeType::CHARACTER_LITERAL;
			case TokenType::STRING:              return NodeType::STRING_LITERAL;
			default:							 return NodeType::INVALID;
			}
		}

	}
}