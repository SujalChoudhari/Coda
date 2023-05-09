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
		class Parser
		{
		public:
			Parser();
			Program parse(std::vector<Token> tokens);

		private: // in the order of prescidence
			void advance();
			Token expect(TokenType type,std::string error);
			Node parseStatement();
			Node parseExpression();
			Node parseAssignmentExpression();
			Node parseObjectExpression();
			Node parseAdditiveExpression();
			Node parseMultiplacativeExpression();

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
	}
}
