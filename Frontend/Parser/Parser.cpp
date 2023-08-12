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

				if (s.type != NodeType::INVALID)
					program.body.emplace_back(s);

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
			case TokenType::DEF:
				return parseFunctionExpression("");
			case TokenType::IF:
				return parseIfExpression();
			case TokenType::FOR:
				return parseForExpression();
			default:
				return parseExpression();
			}
		}

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

		Node Parser::parseExpression()
		{
			Node exp =  parseAssignmentExpression();
			if(mCurrentToken->value == ";")
				advance();
			return exp;
		}

		Node Parser::parseFunctionExpression(std::string name)
		{
			advance();
			std::string functionName;
			if (name.empty()) {
				functionName = mCurrentToken->value;
				expect(TokenType::IDENTIFIER, "Expected an identifier for function declaration");
				IF_ERROR_RETURN_NODE;
			}
			else {
				functionName = name;
			}



			Node params = parseArguments();
			IF_ERROR_RETURN_NODE;

			for (auto it : params.properties) {
				if (it.second->type != NodeType::IDENTIFIER) {
					Error::Parser::raise("Expected an identifier for function argument", it.second->startPosition);
					return Node();
				}
			}

			Node block = parseBlockExpression();
			Node function = Node(NodeType::FUNCTION_LITERAL, functionName);
			function.left = std::make_shared<Node>(params);
			function.right = std::make_shared<Node>(block);

			return function;

		}

		Node Parser::parseBlockExpression()
		{
			expect(TokenType::OPEN_BRACE, "Expected an '{' after function declaration");
			IF_ERROR_RETURN_NODE;

			Node block = Node(NodeType::BLOCK_STATEMENT, "<block>");

			while (mCurrentToken->type != TokenType::CLOSE_BRACE && mCurrentToken->type != TokenType::END_OF_FILE) {
				Node s = parseStatement();
				IF_ERROR_RETURN_NODE;
				if (s.type != NodeType::INVALID)
					block.properties.insert({ std::to_string(block.properties.size()), std::make_shared<Node>(s) });
				IF_ERROR_RETURN_NODE;
			}

			expect(TokenType::CLOSE_BRACE, "Expected an '}' at the end of function declaration");
			IF_ERROR_RETURN_NODE;

			return block;

		}

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



		Node Parser::parseObjectExpression()
		{
			IF_ERROR_RETURN_NODE;

			if (mCurrentToken->type != TokenType::OPEN_BRACE) {
				return parseLogicalOperatorExpression();
			}
			advance();
			Node obj = Node(NodeType::OBJECT_LITERAL);
			while (mCurrentToken->type != TokenType::END_OF_FILE
				&& mCurrentToken->type != TokenType::CLOSE_BRACE) {
				// key: val, key2: val
				// key, key2:val

				Token key = *mCurrentToken;
				expect(TokenType::IDENTIFIER, "Expected an Identifier in object literal");
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

				Node value;

				if (mCurrentToken->type == TokenType::DEF) {
					value = parseFunctionExpression(key.value);
				}
				else {
					value = parseExpression();
				}

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

		Node Parser::parseUnaryOperatorExpression() // ++x--
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

			if(mCurrentToken->value == ";")
				advance();

			declaration.endPosition = mCurrentToken->endPosition;
			return declaration;
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

			if (mCurrentToken->type == TokenType::CLOSE_PAREN) {
				return args;
			}

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
				object.right = std::make_shared<Node>(property);
			}

			return object;
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
			else if (*type == TokenType::BINARY_OPERATOR) { // + or MINUS
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