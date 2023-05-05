#include "Parser.h"
#include "../Node/Node.h"
#include "../../Error/Error.h"

namespace Coda {
	namespace FrontEnd {
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
			if (!Error::Manager::isSafe()) return Program();
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
			if (!Error::Manager::isSafe()) return Node();
			return parseExpression();
		}

		Node Parser::parseExpression()
		{
			if (!Error::Manager::isSafe()) return Node();
			return parseAdditiveExpression();
		}

		Node Parser::parseAdditiveExpression()
		{
			if (!Error::Manager::isSafe()) return Node();
			Error::Position currPos = mCurrentToken->startPosition;
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

				// Set position property for binaryExpression.left and binaryExpression.right
				binaryExpression.left->startPosition = left.startPosition;
				binaryExpression.right->startPosition = right.startPosition;

				// Set position property for binaryExpression
				binaryExpression.startPosition = currPos;

				left = binaryExpression;
			}

			return left;
		}


		Node Parser::parseMultiplacativeExpression()
		{
			if (!Error::Manager::isSafe()) return Node();

			Error::Position currPos = mCurrentToken->startPosition;
			Node left = parsePrimaryExpression();

			while (
				mCurrentToken->value == "/"
				|| mCurrentToken->value == "*"
				|| mCurrentToken->value == "%") {
				Token operatorToken = *mCurrentToken;
				advance();

				Node right = parsePrimaryExpression();

				Node binaryExpression;
				binaryExpression.type = NodeType::BINARY_EXPRESSION;
				binaryExpression.value = operatorToken.value;
				binaryExpression.left = std::make_shared<Node>(left);
				binaryExpression.right = std::make_shared<Node>(right);

				// Set position property for binaryExpression.left and binaryExpression.right
				binaryExpression.left->startPosition = left.startPosition;
				binaryExpression.right->startPosition = right.startPosition;

				// Set position property for binaryExpression
				binaryExpression.startPosition = currPos;

				left = binaryExpression;
			}

			return left;
		}



		Node Parser::parsePrimaryExpression()
		{
			Node expression;
			TokenType* type = &mCurrentToken->type;

			if (!Error::Manager::isSafe()) return expression;

			if (*type == TokenType::IDENTIFIER) {
				expression.type = NodeType::IDENTIFIER;
				expression.value = mCurrentToken->value;
			}
			else if (*type == TokenType::NONE) {
				advance();
				expression.type = NodeType::NONE_LITERAL;
				expression.value = "none";
			}
			else if (*type == TokenType::UNDEFINED) {
				advance();
				expression.type = NodeType::UNDEFINED_LITERAL;
				expression.value = "undefined";
			}
			else if (*type == TokenType::BYTE) {
				expression.type = NodeType::BYTE_LITERAL;
				expression.value = mCurrentToken->value;
			}
			else if (*type == TokenType::INT) {
				expression.type = NodeType::INTEGER_LITERAL;
				expression.value = mCurrentToken->value;
			}
			else if (*type == TokenType::LONG) {
				expression.type = NodeType::LONG_INT_LITERAL;
				expression.value = mCurrentToken->value;
			}
			else if (*type == TokenType::FLOAT) {
				expression.type = NodeType::FLOATING_POINT_LITERAL;
				expression.value = mCurrentToken->value;
			}
			else if (*type == TokenType::DOUBLE) {
				expression.type = NodeType::DOUBLE_LITERAL;
				expression.value = mCurrentToken->value;
			}
			else if (*type == TokenType::OPEN_PAREN) {
				advance(); // skip the paren
				expression = parseExpression();
				if (mCurrentToken->type != TokenType::CLOSE_PAREN) {
					Coda::Error::Parser::raiseUnexpectedTokenError(
						")",
						mCurrentToken->value,
						mCurrentToken->startPosition);
				}
				else {
					advance(); // eat the closing paren
				}
			}
			else {
				expression.type = NodeType::INVALID;
				Coda::Error::Parser::raiseInvalidTokenFoundError(
					mCurrentToken->value,
					mCurrentToken->startPosition);
			}
			// Set position property for expression
			expression.startPosition = mCurrentToken->startPosition;
			expression.endPosition = mCurrentToken->endPosition;
			advance();
			return expression;

		}


	}
}