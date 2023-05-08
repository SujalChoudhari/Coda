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
			if (mCurrentToken == nullptr)
			{
				mCurrentIndex = 0;
				mCurrentToken = &mTokens->at(0);
				return;
			}

			if (mCurrentToken->type != TokenType::END_OF_FILE) {
				if (mCurrentIndex < mTokens->size() - 1) {
					mCurrentIndex++;
					mCurrentToken = &mTokens->at(mCurrentIndex);
				}
			}
		}

		Token Parser::expect(TokenType type, std::string error)
		{
			if (mCurrentToken->type != type) {
				Error::Parser::raise(error, mCurrentToken->endPosition);
				return Token(TokenType::INVALID, "<invalid>", mCurrentToken->endPosition);
			}
			return *mCurrentToken;
		}

		Program Parser::parse(std::vector<Token> tokens)
		{
			IF_ERROR_RETURN_PROGRAM;
			Program program = Program();
			mTokens = &tokens;
			advance();
			while (mCurrentIndex < mTokens->size()
				&& mCurrentToken->type != TokenType::END_OF_FILE) {
				Node s = parseStatement();
				program.body.emplace_back(s);
				advance();
				IF_ERROR_RETURN_PROGRAM;
			}
			return program;
		}



		Node Parser::parseStatement()
		{
			IF_ERROR_RETURN_NODE;

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
			IF_ERROR_RETURN_NODE;

			Node left = parseObjectExpression(); // will switch it with objects

			if (mCurrentToken->type == TokenType::EQUALS) {
				advance();

				Node right = parseAssignmentExpression();
				IF_ERROR_RETURN_NODE;

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

		Node Parser::parseObjectExpression()
		{
			IF_ERROR_RETURN_NODE;

			if (mCurrentToken->type != TokenType::OPEN_BRACE) {
				return parseAdditiveExpression();
			}
			advance();
			Node obj = Node(NodeType::OBJECT_LITERAL);
			while (mCurrentToken->type != TokenType::END_OF_FILE
				&& mCurrentToken->type != TokenType::CLOSE_BRACE) {
				// key: val, key2: val
				// key, key2:val

				Token key = expect(TokenType::IDENTIFIER, "Expected an Identifier in object literal");
				IF_ERROR_RETURN_NODE;

				advance();

				if (mCurrentToken->type == TokenType::COMMA) {
					advance();
					obj.properties.emplace(key.value, std::make_shared<Node>(NodeType::PROPERTY));
					continue;
				}
				else if (mCurrentToken->type == TokenType::CLOSE_BRACE) {
					obj.properties.emplace(key.value, std::make_shared<Node>(NodeType::PROPERTY));
					continue;
				}


				expect(TokenType::COLON, "Expected a ':' in object literal");
				IF_ERROR_RETURN_NODE;

				advance();
				Node value = parseExpression();

				obj.properties.insert_or_assign(key.value, std::make_shared<Node>(value));

				if (mCurrentToken->type != TokenType::CLOSE_BRACE) {
					expect(TokenType::COMMA, "Expected a ',' or '}' in object literal");
					advance();
					IF_ERROR_RETURN_NODE;
				}


			}

			expect(TokenType::CLOSE_BRACE, "Expected a '}' in object literal");
			IF_ERROR_RETURN_NODE;

			obj.value = "<object>";
			advance();
			return obj;
		}

		Node Parser::parseAdditiveExpression()
		{
			IF_ERROR_RETURN_NODE;
			Error::Position currPos = mCurrentToken->startPosition;
			Node left = parseMultiplacativeExpression();

			while (mCurrentToken->value == "+" || mCurrentToken->value == "-") {
				Token operatorToken = *mCurrentToken;
				advance();

				Node right = parseMultiplacativeExpression();
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


			expect(TokenType::IDENTIFIER, "Expected a <indetifier> at ");

			IF_ERROR_RETURN_NODE;
			identifier = mCurrentToken->value;

			advance();

			if (mCurrentToken->type == TokenType::EQUALS)
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
					Error::Runtime::raise("Expected an '=' in a 'const' defination at, ", mCurrentToken->endPosition);
					return declaration;
				}

				declaration.type = NodeType::VARIABLE_DECLARATION;
				declaration.left = std::make_shared<Node>(NodeType::IDENTIFIER, identifier);
				declaration.right = std::make_shared<Node>(NodeType::INTEGER_LITERAL, "0");
				declaration.value = type;
			}

			declaration.endPosition = mCurrentToken->endPosition;
			advance();
			return declaration;
		}


		Node Parser::parseMultiplacativeExpression()
		{
			IF_ERROR_RETURN_NODE;


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

			IF_ERROR_RETURN_NODE;


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
				expect(TokenType::CLOSE_PAREN,
					" Unexpected Token found '" +
					mCurrentToken->value + "' was found instead of ')' at, ");
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