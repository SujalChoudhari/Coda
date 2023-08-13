#include "Lexer.h"

namespace Coda {
	namespace Frontend {
		Lexer::Lexer()
		{
			mTokens = std::vector<Token>();
			mSourceCode = "";
			mCurrentIndex = -1;
			mCurrentPosition = Position(0, 1, "");
		}

		void Lexer::advance()
		{
			mCurrentIndex++;
			if (mCurrentIndex < mSourceCode.size()) {
				mCurrentChar = mSourceCode[mCurrentIndex];
			}
			else {
				mCurrentChar = '\0';
			}
			mCurrentPosition.character++;
			if (mCurrentChar == '\n') {
				// Extract the current line's text
				int lineStart = mCurrentIndex;
				while (lineStart > 0 && mSourceCode[lineStart - 1] != '\n') {
					lineStart--;
				}
				mCurrentPosition.lineText = mSourceCode.substr(lineStart, mCurrentIndex - lineStart);
				// Update the current position
				mCurrentPosition.line++;
				mCurrentPosition.character = 1;
			}
			else {
				mCurrentPosition.lineText += mCurrentChar;
			}
		}

		std::vector<Token> Lexer::tokenize(std::string sourceCode)
		{
			mSourceCode = sourceCode;
			mCurrentPosition = Position(0, 1, "");
			advance();

			while (mCurrentChar != '\0') {
				if (mCurrentChar == '\n') {
					handleNewLine();
				}
				else if (isspace(mCurrentChar)) {
					handleWhitespace();
				}
				else if (isLetter(mCurrentChar)) {
					handleIdentifiers();
				}
				else if (isdigit(mCurrentChar)) {
					handleNumbers();
				}
				else if (isSymbolChar(mCurrentChar)) {
					handleSymbol();
				}
				else if (isBinaryOperator(mCurrentChar)) {
					handleBinaryOperator();
				}
				else if (mCurrentChar == '\'') {
					handleCharacter();
				}
				else if (mCurrentChar == '"') {
					handleStringLiteral();
				}
				else if (mCurrentChar == '=') {
					handleEquals();
				}
				else if (mCurrentChar == ';') {
					handleSemicolon();
				}
				else if (mCurrentChar == ':') {
					handleColon();
				}
				else if (mCurrentChar == ',') {
					handleComma();
				}
				else if (mCurrentChar == '.') {
					handleDot();
				}
				else if (mCurrentChar == '!') {
					handleBang();
				}
				else if (mCurrentChar == '<') {
					handleLeftArrow();
				}
				else if (mCurrentChar == '>') {
					handleRightArrow();
				}
				else if (mCurrentChar == '|') {
					handlePipe();
				}
				else if (mCurrentChar == '&') {
					handleAmpersand();
				}
				else {
					handleIllegalCharacter();
				}
			}

			mTokens.emplace_back(TokenType::END_OF_FILE, "\\0", mCurrentPosition);
			return mTokens;
		}

		bool Lexer::isSymbolChar(char c) {
			return (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}');
		}

		bool Lexer::isBinaryOperator(char c) {
			return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
		}

		bool Lexer::isUnaryOperator(std::string c) {
			return (c == "!" || c == "++" || c == "--");
		}

		bool Lexer::isSupportedDigit(char c)
		{
			return DIGITS_EXTRA.find(c) != std::string::npos;
		}

		bool Lexer::isLetter(char c) {
			return isalpha(c);
		}
	}
}