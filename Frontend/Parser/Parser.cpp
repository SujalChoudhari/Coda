#include "Parser.h"
#include "../Node/Node.h"
#include "../../Error/Error.h"

namespace Coda {
	namespace Frontend {
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
			else {
				mCurrentToken = &mTokens->at(0);
			}
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

			switch (mCurrentToken->type)
			{
			case TokenType::LET:
				return parseDeclaration(false);
			case TokenType::CONST:
				return parseDeclaration(true);
			default:
				return parseExpression();
			}

		}

		Node Parser::parseExpression()
		{
			return parseAssignmentExpression();
		}

		Node Parser::parseAssignmentExpression() {
			if (!Error::Manager::isSafe())
				return Node();

			Node left = parseAdditiveExpression(); // will switch it with objects

			if (mCurrentToken->type == TokenType::EQUALS) {
				advance();

				Node right = parseAssignmentExpression();

				Node expr = Node(NodeType::ASSIGNMENT_EXPRESSION);
				expr.left = std::make_shared<Node>(left);
				expr.right = std::make_shared<Node>(right);
				expr.value = "<assignment>";
				expr.startPosition = left.startPosition;
				expr.endPosition = right.endPosition;

				return expr;	
			}

			return left;
		}

		Node Parser::parseAdditiveExpression()
		{
			if (!Error::Manager::isSafe())
				return Node();
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
			}


			if (mCurrentToken->type != TokenType::IDENTIFIER)
				Error::Parser::raise("Expected a <indetifier> at ", mCurrentToken->endPosition);

			if (!Error::Manager::isSafe())
				return Node();
			identifier = mCurrentToken->value;

			advance();

			if (mCurrentToken->type == TokenType::EQUALS)
			{
				if (!Error::Manager::isSafe())
					return Node();


				declaration.value = type;
				declaration.left = std::make_shared<Node>(NodeType::IDENTIFIER, identifier);

				advance();
				declaration.right = std::make_shared<Node>(parseExpression());

				if (mCurrentToken->type != TokenType::SEMICOLON)
					Error::Parser::raise("Expected a semicolon, at ", mCurrentToken->endPosition);

			}
			else if (mCurrentToken->type == TokenType::SEMICOLON)
			{
				if (isConstant) {
					Error::Runtime::raise("Expected an '=' in a 'const' defination at, ", mCurrentToken->endPosition);
					return declaration;
				}

				declaration.type = NodeType::VARIABLE_DECLARATION;
				declaration.left = std::make_shared<Node>(NodeType::IDENTIFIER, identifier);
				declaration.right = std::make_shared<Node>(NodeType::INTEGER_LITERAL, "0");
				declaration.value = type;
			}
			else
			{
				Error::Parser::raise("Expected an '=' or ';' token at, ", mCurrentToken->endPosition);
			}
			declaration.endPosition = mCurrentToken->endPosition;
			advance();
			return declaration;
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



		Node Parser::parsePrimaryExpression()
		{
			Node expression;
			expression.startPosition = mCurrentToken->startPosition;
			TokenType* type = &mCurrentToken->type;

			if (!Error::Manager::isSafe()) return expression;

			if (*type == TokenType::IDENTIFIER) {
				expression.type = NodeType::IDENTIFIER;
				expression.value = mCurrentToken->value;
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
					Coda::Error::Parser::raise(
						" Unexpected Token found '" +
						mCurrentToken->value + "' was found instead of ')' at, ",
						mCurrentToken->startPosition);
				}
			}
			else {
				expression.type = NodeType::INVALID;
				Coda::Error::Parser::raise("Invalid Token '" +
					mCurrentToken->value + "' was found at, ",
					mCurrentToken->startPosition);
			}

			expression.endPosition = mCurrentToken->endPosition;
			advance();
			return expression;

		}


	}
}