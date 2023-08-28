#include "Parser.h"

namespace Coda {
	namespace Frontend {
		Node Parser::parseIfExpression() {
			IF_ERROR_RETURN_NODE;
			advance(); // skip the if

			Node ifExpressionNode = Node(NodeType::IF_EXPRESSION, "<if>");
			ifExpressionNode.left = std::make_shared<Node>(parseExpression());
			ifExpressionNode.right = std::make_shared<Node>(parseBlockExpression());

			int elifCount = 0;
			while (mCurrentToken->type == TokenType::ELIF) {
				advance(); // skip the elif
				Node elifExpressionNode = Node(NodeType::IF_EXPRESSION, "<elif>");
				elifExpressionNode.left = std::make_shared<Node>(parseExpression());
				elifExpressionNode.right = std::make_shared<Node>(parseBlockExpression());

				ifExpressionNode.properties.insert({ std::to_string(elifCount) ,std::make_shared<Node>(elifExpressionNode) });
				elifCount++;
			}

			if (mCurrentToken->type == TokenType::ELSE) {
				advance();
				Node elseExpressionNode = Node(NodeType::IF_EXPRESSION, "<else>");
				elseExpressionNode.right = std::make_shared<Node>(parseBlockExpression());
				ifExpressionNode.properties.insert({ std::to_string(elifCount), std::make_shared<Node>(elseExpressionNode) });
			}

			return ifExpressionNode;
		}

		Node Parser::parseForExpression()
		{
			IF_ERROR_RETURN_NODE;
			Node forExpressionNode = Node(NodeType::FOR_EXPRESSION, "<for>");
			advance();
			expect(TokenType::OPEN_PAREN, "Expected an '(' after for");
			Node initializer = parseStatement();
			Node condition = parseExpression();
			Node increment = parseStatement();
			expect(TokenType::CLOSE_PAREN, "Expected an ')' after for");

			Node forBlock = parseBlockExpression();

			forExpressionNode.left = std::make_shared<Node>(initializer);
			forExpressionNode.right = std::make_shared<Node>(condition);
			forExpressionNode.properties.insert({ "increment", std::make_shared<Node>(increment) });
			forExpressionNode.properties.insert({ "body", std::make_shared<Node>(forBlock) });

			forExpressionNode.startPosition = initializer.startPosition;
			forExpressionNode.endPosition = forBlock.endPosition;

			return forExpressionNode;
		}

		Node Parser::parseWhileExpression()
		{
			IF_ERROR_RETURN_NODE;
			advance();
			expect(TokenType::OPEN_PAREN, "Expected an '(' after while");
			Node condition = parseExpression();
			expect(TokenType::CLOSE_PAREN, "Expected an ')' after while");
			Node whileBlock = parseBlockExpression();

			Node whileExpressionNode = Node(NodeType::WHILE_EXPRESSION, "<while>");
			whileExpressionNode.left = std::make_shared<Node>(condition);
			whileExpressionNode.right = std::make_shared<Node>(whileBlock);

			whileExpressionNode.startPosition = condition.startPosition;
			whileExpressionNode.endPosition = whileBlock.endPosition;
			return whileExpressionNode;
		}

		Node Parser::parseDoWhileExpression()
		{
			IF_ERROR_RETURN_NODE;
			advance();
			Node doBlock = parseBlockExpression();
			expect(TokenType::WHILE, "Expected a 'while' after do block");
			expect(TokenType::OPEN_PAREN, "Expected an '(' after while");
			Node condition = parseExpression();
			expect(TokenType::CLOSE_PAREN, "Expected an ')' after while");

			Node doWhileExpressionNode = Node(NodeType::DO_WHILE_EXPRESSION, "<do-while>");
			doWhileExpressionNode.left = std::make_shared<Node>(condition);
			doWhileExpressionNode.right = std::make_shared<Node>(doBlock);

			doWhileExpressionNode.startPosition = doBlock.startPosition;
			doWhileExpressionNode.endPosition = condition.endPosition;
			return doWhileExpressionNode;
		}

		void Parser::parseJumpExpression(Node& expression) {
			IF_ERROR_RETURN();
			expression.value = mCurrentToken->value;
			expression.type = NodeType::JUMP_EXPRESSION;
			if (mCurrentToken->value == "return") {
				advance();
				expression.left = std::make_shared<Node>(parseExpression());
				if (expression.left->type == NodeType::INVALID) {
					expression.left = nullptr;
				}
			}
			else {
				advance();
			}
		}

	}	// namespace Frontend
}	// namespace Coda