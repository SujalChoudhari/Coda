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
			advance();
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

				IF_ERROR_RETURN_PROGRAM;
				program.body.emplace_back(s);
				
				expect(TokenType::SEMICOLON, "Expected a ';' at the end of ");
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

				Node value = parseExpression();

				obj.properties.insert_or_assign(key.value, std::make_shared<Node>(value));

				if (mCurrentToken->type != TokenType::CLOSE_BRACE) {
					expect(TokenType::COMMA, "Expected a ',' or '}' in object literal");
					IF_ERROR_RETURN_NODE;
				}


			}

			expect(TokenType::CLOSE_BRACE, "Expected a '}' in object literal");
			IF_ERROR_RETURN_NODE;

			obj.value = "<object>";
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


			identifier = mCurrentToken->value;
			expect(TokenType::IDENTIFIER, "Expected a <indetifier> at ");

			IF_ERROR_RETURN_NODE;


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
			return declaration;
		}


		Node Parser::parseMultiplacativeExpression()
		{
			IF_ERROR_RETURN_NODE;


			Error::Position currPos = mCurrentToken->startPosition;
			Node left = parseCallMemberExpression();

			while (
				mCurrentToken->value == "/"
				|| mCurrentToken->value == "*"
				|| mCurrentToken->value == "%") {
				Token operatorToken = *mCurrentToken;
				advance();

				Node right = parseCallMemberExpression();

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

		Node Parser::parseCallMemberExpression()
		{
			IF_ERROR_RETURN_NODE;
			Node member = parseMemberExpression();
			if (mCurrentToken->type == TokenType::OPEN_PAREN) {
				return parseCallExpression(member);
			}
			return member;
		}

		Node Parser::parseCallExpression(const Node& caller)
		{
			IF_ERROR_RETURN_NODE;
			Node callExpression = Node(NodeType::CALL_EXPRESSION);
			callExpression.left = std::make_shared<Node>(caller);
			callExpression.properties = parseArguments().properties;

			// Check for additional calls
			while (mCurrentToken->type == TokenType::OPEN_PAREN) {
				// Expect a closing parenthesis after each argument list
				expect(TokenType::CLOSE_PAREN, "Expected a ')' at, ");
				IF_ERROR_RETURN_NODE;
				callExpression = parseCallExpression(callExpression);
			}

			return callExpression;
		}


		Node Parser::parseArguments()
		{
			IF_ERROR_RETURN_NODE;
			expect(TokenType::OPEN_PAREN, "Expected an '(' at, ");
			IF_ERROR_RETURN_NODE;

			Node argList = parseArgumentList();
			if (mCurrentToken->type == TokenType::CLOSE_PAREN) {
				advance(); // Consume the closing parenthesis
				return argList;
			}
			else {
				Error::Parser::raise("Expected a ')' at, ", mCurrentToken->startPosition);
				return Node(); // Return an empty Node to indicate the error
			}
		}


		Node Parser::parseArgumentList()
		{
			IF_ERROR_RETURN_NODE;
			Node args = Node(NodeType::ARGUMENT_LIST, "<args>");
			args.properties.insert({ "0",std::make_shared<Node>(parseExpression()) });
			int index = 1;
			while (mCurrentToken->type != TokenType::END_OF_FILE && mCurrentToken->type == TokenType::COMMA) {
				advance();
				args.properties.insert({ std::to_string(index),std::make_shared<Node>(parseAssignmentExpression()) });
				index++;
				IF_ERROR_RETURN_NODE;
			}
			return args;
		}

		Node Parser::parseMemberExpression()
		{
			IF_ERROR_RETURN_NODE;
			Node object = parsePrimaryExpression();


			while (mCurrentToken->type == TokenType::DOT || mCurrentToken->type == TokenType::OPEN_BRACKET) {
				Token functor = *mCurrentToken;
				advance();
				std::string computed = "";

				Node property;
				if (functor.type == TokenType::DOT) {
					computed = "<call>";
					property = parsePrimaryExpression();
					IF_ERROR_RETURN_NODE;
					if (property.type != NodeType::IDENTIFIER) {
						Error::Parser::raise("Cannot use '.' Dot operator without right hand side, at ", functor.endPosition);
					}
					IF_ERROR_RETURN_NODE;

				}
				else {
					computed = "<computed-call>";
					property = parseExpression();
					expect(TokenType::CLOSE_BRACKET, "Missing ']' in computed value at, ");
					IF_ERROR_RETURN_NODE;
				}

				object = Node(NodeType::MEMBER_EXPRESSION, computed, std::make_shared<Node>(object));
			}

			return object;
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

			return expression;

		}


	}
}