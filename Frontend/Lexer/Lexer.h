#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "../Tokens/Token.h"
#include "../../Error/Position.h"
#include "../Tokens/Constants.h"
#include "../../Error/Error.h"

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
			// The tokens that have been generated.
			std::vector<Token> mTokens = {};
			
			// Input source code.
			std::string mSourceCode;

			// Current index in the source code.
			unsigned int mCurrentIndex = -1;
			
			// A pointer to the current character.
			char mCurrentChar;

			// The current line number.
			Position mCurrentPosition;

		private:
			/*
				Go to the next character in the source code.
				Used when the current character has been processed completely.
			*/
			void advance();

			//	Handlers for different types of characters.

			void handleNewLine();
			void handleWhitespace();
			void handleSymbol();
			void handleBinaryOperator();
			void handleStringLiteral();
			std::string handleEscapeSequence();
			void handleCharacter();
			void handleEquals();
			void handleSemicolon();
			void handleColon();
			void handleComma();
			void handleDot();
			void handleBang();
			void handlePlus();
			void handleMinus();
			void handleStar();
			void handleSlash();
			void handlePercent();
			void handleLeftArrow();
			void handleRightArrow();
			void handlePipe();
			void handleAmpersand();
			void handleNumbers();
			std::string handleDigitOrExtension();
			void handleIdentifiers();
			void handleIllegalCharacter();

			/*
				Checkers for specific types of characters.
				returns true if the character is of the specified type.
			*/
			bool isSymbolChar(char c);
			bool isBinaryOperator(char c);
			bool isUnaryOperator(std::string c);
			bool isLetter(char c);
			bool isSupportedDigit(char ch);
			bool isDigitOrNumberComponent(char c);
		};
	} // namespace Frontend
} // namespace Coda