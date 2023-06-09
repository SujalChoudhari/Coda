#pragma once
#include <string>
#include <vector>
#include "../Tokens/Token.h"
#include "../../Error/Position.h"

using namespace Coda::Error;

namespace Coda {
	namespace Frontend {
        /*
            Lex/ Tokenize the source code.
            Converts the source code into a vector of tokens.
        */
		class Lexer {
		public:
            // Create a new Lexer.
			Lexer();

        public:
            // Create a new Lexer with the source code.
			std::vector<Token> tokenize(std::string sourceCode);


		private:
			std::vector<Token> mTokens = {};
			std::string mSourceCode;
			unsigned int mCurrentIndex = -1;
			char mCurrentChar;
			Position mCurrentPosition;

        private:
            void advance();
            void handleNewLine();
            void handleWhitespace();
            void handleSymbol();
            void handleBinaryOperator();
            void handleStringLiteral();
            void handleCharacter();
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
	} // namespace Frontend
} // namespace Coda