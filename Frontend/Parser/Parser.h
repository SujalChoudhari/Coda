#pragma once
#include "../Node/Node.h"
#include "../Node/Program.h"
#include "../Tokens/Token.h"
#include "../Tokens/TokenType.h"
#include "../Lexer/Lexer.h"
#include "../../Utils/FileReader.h"

#define IF_ERROR_RETURN_NODE if (!Error::Manager::isSafe()) return Node()
#define IF_ERROR_RETURN_PROGRAM if (!Error::Manager::isSafe()) return Program()


namespace Coda {
	namespace Frontend {
		/*
			Converts a vector of tokens into a programs wiz. a list of binary trees.
		*/
		class Parser
		{
		public:
			// Create a parser.
			Parser(std::string mainFilePath);
			// Parse a vector of tokens into a program.
			Program parse(std::vector<Token> tokens);

		private:
			/*
				increments to the next token
				updates current postion
			*/
			void advance();

			/*
				if next token is not of the given token type,
				it throws an error
			*/
			Token expect(TokenType type, std::string error);

			//	parse methods
			//	in order of precedance (reverse)			
			Node parseStatement();
			Node parseExpression();
			Node parseIfExpression();
			Node parseForExpression();
			Node parseWhileExpression();
			Node parseDoWhileExpression();
			Node parseFunctionExpression(std::string name);
			Node parseAssignmentExpression();
			Node parseBlockExpression();
			Node parseScopeExpression();
			Node parseObjectExpression();
			Node parseListExpression();
			Node parseLogicalOperatorExpression();
			Node parseRelationalOperatorExpression();
			Node parseAdditiveExpression();
			Node parseMultiplicativeExpression();

			// combined handler for all the binary type handlers
			Node parseBinaryOperatorExpression(Node(Parser::* parseSubExpression)(), const std::vector<std::string>& operators);
			
			Node parseUnaryOperatorExpression();
			Node parseUnaryExpression();
			Node parseCallMemberExpression();
			Node parseCallExpression(const Node& caller);
			Node parseArguments();
			Node parseArgumentList();
			Node parseMemberExpression();
			Node parsePrimaryExpression();
			Node parseDeclaration(bool isConstant);

			// helper handlers for parse methods
			void parseLiteralExpression(Node& expression, TokenType type);
			void parseJumpExpression(Node& expression);
			bool isBinaryOperatorToken(const std::string& op);
			NodeType getTokenTypeAsNodeType(TokenType tokenType);
			void handleInvalidToken();


		private:
			/*
				main file of the current interpretation
			*/
			std::string mMainFilePath;
			std::string mMainDir;

			/*
				tokens to parse from
			*/
			std::vector<Token>* mTokens;
			
			/*
				current token pointer for parsing
			*/
			Token* mCurrentToken;

			/*
				current token index for parsing
			*/
			int mCurrentIndex = -1;
		};
	} // namespace Frontend
} // namespace Coda
