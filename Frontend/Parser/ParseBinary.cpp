#include "Parser.h"

namespace Coda {
	namespace Frontend {


		Node Parser::parseBinaryOperatorExpression(Node(Parser::* parseSubExpression)(), const std::vector<std::string>& operators)
		{
			IF_ERROR_RETURN_NODE;
			Error::Position currPos = mCurrentToken->startPosition;
			Node left = (this->*parseSubExpression)();

			while (std::find(operators.begin(), operators.end(), mCurrentToken->value) != operators.end()) {
				Token operatorToken = *mCurrentToken;
				advance();

				Node right = (this->*parseSubExpression)();
				IF_ERROR_RETURN_NODE;

				Node binaryExpression;
				binaryExpression.type = NodeType::BINARY_EXPRESSION;
				binaryExpression.value = operatorToken.value;
				binaryExpression.left = std::make_shared<Node>(left);
				binaryExpression.right = std::make_shared<Node>(right);

				// Set position property for binaryExpression.left and binaryExpression.right
				binaryExpression.left->startPosition = left.startPosition;
				binaryExpression.left->endPosition = left.endPosition;
				binaryExpression.right->startPosition = right.startPosition;
				binaryExpression.right->endPosition = right.endPosition;

				// Set position property for binaryExpression
				binaryExpression.startPosition = currPos;
				binaryExpression.endPosition = mCurrentToken->endPosition;

				left = binaryExpression;
			}

			return left;
		}



		Node Parser::parseLogicalOperatorExpression()
		{
			return parseBinaryOperatorExpression(&Parser::parseRelationalOperatorExpression, { "&&","||" });
		}

		Node Parser::parseRelationalOperatorExpression()
		{
			return parseBinaryOperatorExpression(&Parser::parseAdditiveExpression, { "==","!=",">=","<=", "<",">" });
		}

		Node Parser::parseAdditiveExpression()
		{
			return parseBinaryOperatorExpression(&Parser::parseMultiplicativeExpression, { "+", "-" });
		}

		Node Parser::parseMultiplicativeExpression()
		{
			return parseBinaryOperatorExpression(&Parser::parseUnaryOperatorExpression, { "/", "*", "%" });
		}

	} // namespace Frontend
} // namespace Coda