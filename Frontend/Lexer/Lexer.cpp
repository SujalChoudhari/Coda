#include <string>
#include <vector>
#include <iostream>
#include "../Tokens/Constants.h"
#include "../../Error/Error.h"
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





		std::vector<Token> Lexer::tokenise(std::string sourceCode)
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
				else if (mCurrentChar == '"') {
					handleStringLiteral();
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
				else {
					handleIllegalCharacter();
				}
			}

			mTokens.emplace_back(TokenType::END_OF_FILE, "\\0", mCurrentPosition);
			return mTokens;
		}


		void Lexer::handleNewLine() {
			advance();
		}

		void Lexer::handleWhitespace() {
			advance();
		}


		void Lexer::handleEquals() {
			mTokens.emplace_back(TokenType::EQUALS, "=", mCurrentPosition);
			advance();
		}

		void Lexer::handleSemicolon() {
			mTokens.emplace_back(TokenType::SEMICOLON, ";", mCurrentPosition);
			advance();
		}

		void Lexer::handleColon() {
			mTokens.emplace_back(TokenType::COLON, ":", mCurrentPosition);
			advance();
		}

		void Lexer::handleComma() {
			mTokens.emplace_back(TokenType::COMMA, ",", mCurrentPosition);
			advance();
		}

		void Lexer::handleDot() {
			mTokens.emplace_back(TokenType::DOT, ".", mCurrentPosition);
			advance();
		}

		void Lexer::handleIllegalCharacter() {
			Coda::Error::Lexer::raise("Illegal Character '" + std::to_string((char)mCurrentChar) + "' found at ", mCurrentPosition);
			advance();
		}

		bool Lexer::isSymbolChar(char c) {
			return (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}');
		}

		bool Lexer::isBinaryOperator(char c) {
			return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
		}

		bool Lexer::isSupportedDigit(char c)
		{
			return DIGITS_EXTRA.find(c) != std::string::npos;
		}

		bool Lexer::isLetter(char c) {
			return isalpha(c);
		}

		void Lexer::handleSymbol() {
			char symbol = mCurrentChar;
			std::string symbolString(1, symbol);
			TokenType tokenType;

			switch (symbol) {
			case '(':
				tokenType = TokenType::OPEN_PAREN;
				break;
			case ')':
				tokenType = TokenType::CLOSE_PAREN;
				break;
			case '[':
				tokenType = TokenType::OPEN_BRACKET;
				break;
			case ']':
				tokenType = TokenType::CLOSE_BRACKET;
				break;
			case '{':
				tokenType = TokenType::OPEN_BRACE;
				break;
			case '}':
				tokenType = TokenType::CLOSE_BRACE;
				break;
			default:
				// Handle unrecognized symbols if needed
				break;
			}

			mTokens.emplace_back(tokenType, symbolString, mCurrentPosition);
			advance();
		}

		void Lexer::handleBinaryOperator() {
			char operatorChar = mCurrentChar;
			std::string operatorString(1, operatorChar);

			TokenType tokenType;
			// Assign the appropriate token type based on the operator character
			switch (operatorChar) {
			case '+':
				tokenType = TokenType::BINARY_OPERATOR;
				break;
			case '-':
				tokenType = TokenType::BINARY_OPERATOR;
				break;
			case '*':
				tokenType = TokenType::BINARY_OPERATOR;
				break;
			case '/':
				tokenType = TokenType::BINARY_OPERATOR;
				break;
			case '%':
				tokenType = TokenType::BINARY_OPERATOR;
				break;
			default:
				// Handle unrecognized operators if needed
				break;
			}

			mTokens.emplace_back(tokenType, operatorString, mCurrentPosition);
			advance();
		}

		void Lexer::handleStringLiteral() {
			std::string stringLiteral;
			Position startPosition = mCurrentPosition;

			advance(); // Skip the opening double quote

			while (mCurrentChar != '\0' && mCurrentChar != '"') {
				if (mCurrentChar == '\\') {
					advance();
					if (mCurrentChar == '\0') {
						break;
					}
				}

				stringLiteral += mCurrentChar;
				advance();
			}

			if (mCurrentChar == '"') {
				// Add the string literal token to mTokens
				mTokens.emplace_back(TokenType::STRING, stringLiteral, startPosition,mCurrentPosition);
				advance(); // Skip the closing double quote
			}
			else {
				 Error::Lexer::raise("Unterminated string literal at ", startPosition);
			}
		}



		void Lexer::handleNumbers() {
			Position start = mCurrentPosition;
			std::string num = "";
			bool dot = false;
			bool sci = false;
			bool signedSci = false;
			TokenType type = TokenType::INT;

			while (mCurrentChar != '\0' && (isSupportedDigit(mCurrentChar) || mCurrentChar == '.' || mCurrentChar == 'e' || mCurrentChar == 'E' || (sci && (mCurrentChar == '-' || mCurrentChar == '+')))) {
				if (mCurrentChar == '.') {
					if (dot) {
						Error::Lexer::raise("Unexpected Character '" + std::to_string(mCurrentChar) + "' found at", mCurrentPosition);
						advance();
						continue;
					}
					else {
						dot = true;
						type = TokenType::DOUBLE;
						num.push_back(mCurrentChar);
					}
				}
				else if (mCurrentChar == 'e' || mCurrentChar == 'E') {
					if (sci) {
						Error::Lexer::raise("Unexpected Character '" + std::to_string(mCurrentChar) + "' found at", mCurrentPosition);

						advance();
						continue;
					}
					else {
						sci = true;
						type = TokenType::DOUBLE;
						num.push_back(mCurrentChar);
					}
				}
				else if (sci && (mCurrentChar == '-' || mCurrentChar == '+')) {
					if (signedSci) {
						Error::Lexer::raise("Unexpected Character '" + std::to_string(mCurrentChar) + "' found at", mCurrentPosition);
						advance();
						continue;
					}
					else {
						signedSci = true;
						num.push_back(mCurrentChar);
					}
				}
				else {
					auto it = DIGIT_EXTENTIONS.find(mCurrentChar);
					if (it != DIGIT_EXTENTIONS.end()) {
						type = it->second;
					}
					else if (isdigit(mCurrentChar)) {
						num.push_back(mCurrentChar);
					}
					else {
						break;
					}
				}
				advance();
			}

			mTokens.emplace_back(type, num, start, mCurrentPosition);
		}

		void Lexer::handleIdentifiers()
		{
			Position start = mCurrentPosition;
			std::string identifier = "";
			while (mCurrentChar != '\0' && isalpha(mCurrentChar)) {
				identifier.push_back(mCurrentChar);
				advance();
			}
			auto iden = KEYWORD.find(identifier);
			if (iden == KEYWORD.end())
			{
				mTokens.emplace_back(TokenType::IDENTIFIER, identifier, start, mCurrentPosition);
			}
			else {
				mTokens.emplace_back(KEYWORD[identifier], identifier, start, mCurrentPosition);
			}
		}
	}
}