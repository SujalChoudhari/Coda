#include <string>
#include <vector>
#include <iostream>
#include "../Tokens/Constants.h"
#include "../../Error/Error.h"
#include "Lexer.h"

namespace Coda {
	namespace FrontEnd {
		Lexer::Lexer()
		{
			mTokens = std::vector<Token>();
			mSourceCode = "";
			mCurrentIndex = -1;
			mCurrentPosition = Position(0, 1);
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
		}

		bool Lexer::isSupportedDigit(char c)
		{
			return DIGITS_EXTRA.find(c) != std::string::npos;
		}


		std::vector<Token> Lexer::tokenise(std::string sourceCode)
		{
			mSourceCode = sourceCode;
			advance();

			while (mCurrentChar != '\0') {
				if (mCurrentChar == '\n') {
					mCurrentPosition.line++;
					mCurrentPosition.character = 1;
				}
				else if (isspace(mCurrentChar)) {
					advance();
				}
				else if (mCurrentChar == '(')
				{
					mTokens.emplace_back(TokenType::OPEN_PAREN, "(", mCurrentPosition);
					advance();
				}
				else if (mCurrentChar == ')')
				{
					mTokens.emplace_back(TokenType::CLOSE_PAREN, ")", mCurrentPosition);
					advance();
				}
				else if (mCurrentChar == '+'
					|| mCurrentChar == '-'
					|| mCurrentChar == '*'
					|| mCurrentChar == '/'
					|| mCurrentChar == '%')
				{
					std::string s(1, mCurrentChar);
					mTokens.emplace_back(TokenType::BINARY_OPERATOR, s, mCurrentPosition);
					advance();
				}
				else if (mCurrentChar == '=') {
					mTokens.emplace_back(TokenType::EQUALS, "=", mCurrentPosition);
					advance();
				}
				else if (isSupportedDigit(mCurrentChar)) {
					buildNumbers();
				}
				else if (isalpha(mCurrentChar)) {
					buildIdentifiers();
				}
				else {
					Coda::Error::Lexer::raiseIllegalCharacterError(mCurrentChar, mCurrentPosition);
					advance();
				}
			}

			mTokens.emplace_back(TokenType::END_OF_FILE, "\\0", mCurrentPosition);

			return mTokens;
		}

		void Lexer::buildNumbers() {
			Position start = mCurrentPosition;
			std::string num = "";
			bool dot = false;
			bool sci = false;
			bool signedSci = false;
			TokenType type = TokenType::INT;

			while (mCurrentChar != '\0' && (isSupportedDigit(mCurrentChar) || mCurrentChar == '.' || mCurrentChar == 'e' || mCurrentChar == 'E' || (sci && (mCurrentChar == '-' || mCurrentChar == '+')))) {
				if (mCurrentChar == '.') {
					if (dot) {
						Error::Lexer::raiseUnexpectedCharacterError('.', mCurrentPosition);
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
						Error::Lexer::raiseUnexpectedCharacterError(mCurrentChar, mCurrentPosition);
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
						Error::Lexer::raiseUnexpectedCharacterError(mCurrentChar, mCurrentPosition);
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

		void Lexer::buildIdentifiers()
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