#include "Parser.h"

namespace Coda {
	namespace Frontend {
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



		Node Parser::parseObjectExpression()
		{
			IF_ERROR_RETURN_NODE;

			if (mCurrentToken->type != TokenType::OPEN_BRACE) {
				return parseListExpression();
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
				else if (mCurrentToken->type == TokenType::OPEN_BRACKET) {
					value = parseListExpression();
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

		Node Parser::parseListExpression()
		{
			if (mCurrentToken->type != TokenType::OPEN_BRACKET) {
				return parseLogicalOperatorExpression();
			}
			advance();

			Node list = Node(NodeType::LIST_LITERAL);
			list.value = "<list>";
			list.startPosition = mCurrentToken->startPosition;
			while (mCurrentToken->type != TokenType::END_OF_FILE
				&& mCurrentToken->type != TokenType::CLOSE_BRACKET) {
				Node value = parseExpression();
				IF_ERROR_RETURN_NODE;

				list.properties.insert({ std::to_string(list.properties.size()), std::make_shared<Node>(value) });

				if (mCurrentToken->type != TokenType::CLOSE_BRACKET) {
					expect(TokenType::COMMA, "Expected a ',' or ']' in list literal");
					IF_ERROR_RETURN_NODE;
				}
			}
			expect(TokenType::CLOSE_BRACKET, "Expected a ']' in list literal");
			list.endPosition = mCurrentToken->endPosition;
			return list;
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

	} // namespace Frontend
} // namespace Coda