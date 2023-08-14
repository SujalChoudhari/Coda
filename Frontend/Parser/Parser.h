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

		private: // in the order of precedence
			void advance();
			Token expect(TokenType type, std::string error);
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
			Node parseUnaryOperatorExpression();

			Node parseBinaryOperatorExpression(Node(Parser::* parseSubExpression)(), const std::vector<std::string>& operators);

			Node parseUnaryExpression();
			Node parseCallMemberExpression();
			Node parseCallExpression(const Node& caller);
			Node parseArguments();
			Node parseArgumentList();
			Node parseMemberExpression();
			Node parsePrimaryExpression();
			Node parseDeclaration(bool isConstant);

			void parseLiteralExpression(Node& expression, TokenType type);
			void parseJumpExpression(Node& expression);
			bool isBinaryOperatorToken(const std::string& op);
			NodeType getTokenTypeAsNodeType(TokenType tokenType);
			void handleInvalidToken();


		private:
			std::string mMainFilePath;
			std::string mMainDir;
			std::vector<Token>* mTokens;
			Token* mCurrentToken;
			int mCurrentIndex = -1;
		};
	} // namespace Frontend
} // namespace Coda
