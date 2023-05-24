#pragma once
#include <string>
#include <vector>
#include "../Tokens/Token.h"
#include "../../Error/Position.h"

using namespace Coda::Error;

namespace Coda {
	namespace Frontend {
		class Lexer {
		public:
			Lexer();
			std::vector<Token> tokenise(std::string sourceCode);


		private:
			std::vector<Token> mTokens = {};
			std::string mSourceCode;
			unsigned int mCurrentIndex = -1;
			char mCurrentChar;
			Position mCurrentPosition;

		private:
        private:
            void advance();
            void handleNewLine();
            void handleWhitespace();
            void handleSymbol();
            void handleBinaryOperator();
            void handleStringLiteral();
            void handleEquals();
            void handleSemicolon();
            void handleColon();
            void handleComma();
            void handleDot();
            void handleNumbers();
            void handleIdentifiers();
            void handleIllegalCharacter();
            bool isSymbolChar(char c);
            bool isBinaryOperator(char c);
            bool isLetter(char c);

			bool isSupportedDigit(char ch);
		};
	}
}