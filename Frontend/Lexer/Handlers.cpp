#include "Lexer.h"

namespace Coda {
	namespace Frontend {
		void Lexer::handleNewLine() {
			advance();
		}

		void Lexer::handleWhitespace() {
			advance();
		}

		void Lexer::handleCharacter() {
			advance(); // skip the quote
			mTokens.emplace_back(TokenType::CHAR, std::to_string(mCurrentChar), mCurrentPosition);
			advance();
			if (mCurrentChar != '\'')
			{
				Error::Lexer::raise("Unterminated character literal at ", mCurrentPosition);
			}
			advance();
		}

		void Lexer::handleEquals() {
			advance();
			if (mCurrentChar == '=') {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "==", mCurrentPosition);
				advance();
			}
			else {
				mTokens.emplace_back(TokenType::ASSIGN, "=", mCurrentPosition);
			}
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

		void Lexer::handleBang() {
			advance();
			if (mCurrentChar == '=') {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "!=", mCurrentPosition);
				advance();
			}
			else {
				mTokens.emplace_back(TokenType::UNARY_OPERATOR, "!", mCurrentPosition);
			}
		}

		void Lexer::handlePlus()
		{
			Position pos = Position(mCurrentPosition);
			advance();
			if (mCurrentChar == '+') {
				mTokens.emplace_back(TokenType::UNARY_OPERATOR, "++", pos, mCurrentPosition);
				advance();
			}
			else if (mCurrentChar == '=') {
				mTokens.emplace_back(TokenType::ASSIGN, "+=", pos, mCurrentPosition);
				advance();
			}
			else {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "+", pos);
			}
		}

		void Lexer::handleMinus() {
			Position pos = Position(mCurrentPosition);
			advance();
			if (mCurrentChar == '-') {
				mTokens.emplace_back(TokenType::UNARY_OPERATOR, "--", pos, mCurrentPosition);
				advance();
			}
			else if (mCurrentChar == '=') {
				mTokens.emplace_back(TokenType::ASSIGN, "-=", pos, mCurrentPosition);
				advance();
			}
			else {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "-", pos);
			}
		}

		void Lexer::handleStar() {
			Position pos = Position(mCurrentPosition);
			advance();

			if (mCurrentChar == '=') {
				mTokens.emplace_back(TokenType::ASSIGN, "*=", pos, mCurrentPosition);
				advance();
			}
			else {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "*", pos);
			}
		}

		void Lexer::handleSlash() {
			Position pos = Position(mCurrentPosition);
			advance();

			if (mCurrentChar == '=') {
				mTokens.emplace_back(TokenType::ASSIGN, "/=", pos, mCurrentPosition);
				advance();
			}
			else if (mCurrentChar == '/') { // Comments
				while (mCurrentChar != '\n' && mCurrentChar != '\0' && mCurrentChar != ';') {
					advance();
				}
			}
			else {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "/", pos);
			}
		}

		void Lexer::handlePercent() {
			Position pos = Position(mCurrentPosition);
			mTokens.emplace_back(TokenType::BINARY_OPERATOR, "%", pos);
			advance();
		}

		void Lexer::handleLeftArrow() {
			advance();
			if (mCurrentChar == '=') {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "<=", mCurrentPosition);
				advance();
			}
			else {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "<", mCurrentPosition);
			}
		}

		void Lexer::handleRightArrow() {
			advance();
			if (mCurrentChar == '=') {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, ">=", mCurrentPosition);
				advance();
			}
			else {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, ">", mCurrentPosition);
			}
		}

		void Lexer::handlePipe() {
			advance();
			if (mCurrentChar == '|') {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "||", mCurrentPosition);
				advance();
			}
			else {
				Error::Lexer::raise("Bitwise OR (|) is not supported at ", mCurrentPosition);
			}
			// TODO: Handle Bitwise OR (|)
		}

		void Lexer::handleAmpersand() {
			advance();
			if (mCurrentChar == '&') {
				mTokens.emplace_back(TokenType::BINARY_OPERATOR, "&&", mCurrentPosition);
				advance();
			}
			else {
				Error::Lexer::raise("Bitwise AND (&) is not supported at ", mCurrentPosition);
			}
			// TODO: Handle Bitwise AND (&)
		}

		void Lexer::handleIllegalCharacter() {
			Coda::Error::Lexer::raise("Illegal Character '" + std::to_string((char)mCurrentChar) + "' found at ", mCurrentPosition);
			advance();
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

			switch (operatorChar) {
			case '+':
				handlePlus();
				break;
			case '-':
				handleMinus();
				break;
			case '*':
				handleStar();
				break;
			case '/':
				handleSlash();
				break;
			case '%':
				handlePercent();
				break;
			default:
				break;
			}
		}

		void Lexer::handleStringLiteral() {
			std::string stringLiteral;
			Position startPosition = mCurrentPosition;

			advance(); // Skip the opening double quote

			while (mCurrentChar != '\0' && mCurrentChar != '"') {
				if (mCurrentChar == '\\') {
					stringLiteral += handleEscapeSequence();
				}
				else {
					stringLiteral += mCurrentChar;
					advance();
				}
			}

			if (mCurrentChar == '"') {
				// Add the string literal token to mTokens
				mTokens.emplace_back(TokenType::STRING, stringLiteral, startPosition, mCurrentPosition);
				advance(); // Skip the closing double quote
			}
			else {
				Error::Lexer::raise("Unterminated string literal at ", startPosition);
			}
		}

		std::string Lexer::handleEscapeSequence() {
			advance(); // Skip the backslash
			switch (mCurrentChar) {
			case 'n':  return "\n";
			case 't':  return "\t";
			case '\"': return "\"";
			case '\'': return "\'";
			case 'r':  return "\r";
			case 'b':  return "\b";
			case 'f':  return "\f";
			case '/':  return "/";
			case '\\': return "\\";
			default:
				return "\\" + std::string(1, mCurrentChar);
			}
		}

		void Lexer::handleNumbers() {
			Position start = mCurrentPosition;
			std::string num = "";
			TokenType type = TokenType::INT;

			while (isDigitOrNumberComponent(mCurrentChar)) {
				if (mCurrentChar == '.' || mCurrentChar == 'e' || mCurrentChar == 'E') {
					type = TokenType::DOUBLE;
					num.push_back(mCurrentChar);
				}
				else if ((mCurrentChar == '-' || mCurrentChar == '+') && (num.empty() || (num.back() == 'e' || num.back() == 'E'))) {
					num.push_back(mCurrentChar);
				}
				else {
					num += handleDigitOrExtension();
				}
				advance();
			}

			mTokens.emplace_back(type, num, start, mCurrentPosition);
		}

		std::string Lexer::handleDigitOrExtension() {
			if (isdigit(mCurrentChar)) {
				return std::string(1, mCurrentChar);
			}
			else {
				auto it = DIGIT_EXTENTIONS.find(mCurrentChar);
				return (it != DIGIT_EXTENTIONS.end()) ? std::string(1, static_cast<char>(it->second)) : "";
			}
		}

		void Lexer::handleIdentifiers()
		{
			Position start = mCurrentPosition;
			std::string identifier = "";
			do {
				identifier.push_back(mCurrentChar);
				advance();
			} while (mCurrentChar != '\0' && (isalnum(mCurrentChar) || mCurrentChar == '_'));

			auto iden = KEYWORD.find(identifier);
			if (iden == KEYWORD.end())
			{
				mTokens.emplace_back(TokenType::IDENTIFIER, identifier, start, mCurrentPosition);
			}
			else {
				mTokens.emplace_back(KEYWORD[identifier], identifier, start, mCurrentPosition);
			}
		}
	} // namespace Frontend
} // namespace Coda