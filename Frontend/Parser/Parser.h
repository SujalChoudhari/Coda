#pragma once
#include "../Node/Node.h"
#include "../Node/Program.h"
#include "../Tokens/Token.h"
#include "../Tokens/TokenType.h"
#include "../Lexer/Lexer.h"

namespace Coda {
	namespace Frontend {
		class Parser
		{
		public:
			Parser();
			Program parse(std::vector<Token> tokens);

		private: // in the order of prescidence
			void advance();
			Node parseStatement();
			Node parseExpression();

			Node parsePrimaryExpression();
			Node parseMultiplacativeExpression();
			Node parseAdditiveExpression();

		private:
			std::vector<Token>* mTokens;
			Token* mCurrentToken;
			int mCurrentIndex = -1;
		};
	}
}
