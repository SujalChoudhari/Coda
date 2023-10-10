#include "Lexer.h"

namespace Coda {
	namespace Frontend {
		Lexer::Lexer(const std::string& scopeName)
		{
			mTokens = std::vector<Token>();
			mSourceCode = "";
			mCurrentIndex = -1;
			mCurrentPosition = Position(0, 1, "");
			mScopeName = scopeName;
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
				if (mCurrentIndex > 0 && mSourceCode[mCurrentIndex - 1] == '\n') {
					mCurrentPosition.lineText = "";
				}
				mCurrentPosition.lineText += mCurrentChar;
			}
			mCurrentPosition.scope = mScopeName;
		}

		std::vector<Token> Lexer::tokenize(std::string sourceCode)
		{
			mSourceCode = sourceCode;
			mCurrentPosition = Position(0, 1, "");
			advance();

			while (mCurrentChar != '\0') {
				switch (mCurrentChar) {
					case '\n':
						handleNewLine();
						break;
					case ' ':
					case '\t':
					case '\r':
					case '\f':
					case '\v':
						handleWhitespace();
						break;
					case '\'':
						handleCharacter();
						break;
					case '"':
						handleStringLiteral();
						break;
					case '=':
						handleEquals();
						break;
					case ';':
						handleSemicolon();
						break;
					case ':':
						handleColon();
						break;
					case ',':
						handleComma();
						break;
					case '.':
						handleDot();
						break;
					case '!':
						handleBang();
						break;
					case '<':
						handleLeftArrow();
						break;
					case '>':
						handleRightArrow();
						break;
					case '|':
						handlePipe();
						break;
					case '&':
						handleAmpersand();
						break;
					default:
						if (isdigit(mCurrentChar)) {
							handleNumbers();
						} else if (isLetter(mCurrentChar)) {
							handleIdentifiers();
						} else if (isSymbolChar(mCurrentChar)) {
							handleSymbol();
						} else if (isBinaryOperator(mCurrentChar)) {
							handleBinaryOperator();
						} else {
							handleIllegalCharacter();
						}
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

		bool Lexer::isDigitOrNumberComponent(char c) {
			return isdigit(c) || c == '.' || c == 'e' || c == 'E' || c == '-' || c == '+';
		}
	}
}