#pragma once
#include "../Node/Node.h"
#include "../Node/Program.h"
#include "../Tokens/Token.h"
#include "../Tokens/TokenType.h"
#include "../Lexer/Lexer.h"

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
			Parser();
			// Parse a vector of tokens into a program.
			Program parse(std::vector<Token> tokens);

		private: // in the order of precedence
			void advance();
			Token expect(TokenType type, std::string error);
			Node parseStatement();
			Node parseExpression();
			Node parseIfExpression();
			Node parseFunctionExpression(std::string name);
			Node parseAssignmentExpression();
			Node parseBlockExpression();
			Node parseObjectExpression();
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


		private:
			std::vector<Token>* mTokens;
			Token* mCurrentToken;
			int mCurrentIndex = -1;
		};
	} // namespace Frontend
} // namespace Coda
