#include "Parser.h"
#include "../Node/Node.h"
#include "../../Error/Error.h"

namespace Coda {
	namespace Frontend {
		Parser::Parser(std::string filePath)
		{
			mCurrentIndex = -1;
			mCurrentToken = nullptr;
			mTokens = nullptr;
			mMainFilePath = filePath;
			size_t lastSlashIndex = filePath.find_last_of("/\\");
			mMainDir = filePath.substr(0, lastSlashIndex + 1);
		}

		void Parser::advance()
		{
			if (mCurrentToken == nullptr)
			{
				mCurrentIndex = 0;
				mCurrentToken = &mTokens->at(0);
				return;
			}

			if (mCurrentToken->type != TokenType::END_OF_FILE) {
				if (mCurrentIndex < mTokens->size() - 1) {
					mCurrentIndex++;
					mCurrentToken = &mTokens->at(mCurrentIndex);
				}
			}
		}

		Token Parser::expect(TokenType type, std::string error)
		{
			if (mCurrentToken->type != type) {
				Error::Parser::raise(error + ", at: ", mCurrentToken->endPosition);
				return Token(TokenType::INVALID, "<invalid>", mCurrentToken->endPosition);
			}
			advance();
			return *mCurrentToken;
		}

		Program Parser::parse(std::vector<Token> tokens)
		{
			IF_ERROR_RETURN_PROGRAM;
			Program program = Program();
			mTokens = &tokens;
			advance();
			while (mCurrentIndex < mTokens->size()
				&& mCurrentToken->type != TokenType::END_OF_FILE) {
				Node s = parseStatement();
				IF_ERROR_RETURN_PROGRAM;

				if (s.type != NodeType::INVALID)
					program.body.emplace_back(s);

			}
			return program;
		}

		Node Parser::parseStatement()
		{
			IF_ERROR_RETURN_NODE;

			switch (mCurrentToken->type)
			{
			case TokenType::LET:
				return parseDeclaration(false);
			case TokenType::CONST:
				return parseDeclaration(true);
			case TokenType::DEF:
				return parseFunctionExpression("");
			case TokenType::IF:
				return parseIfExpression();
			case TokenType::FOR:
				return parseForExpression();
			case TokenType::WHILE:
				return parseWhileExpression();
			case TokenType::DO:
				return parseDoWhileExpression();
			case TokenType::SCOPE:
				return parseScopeExpression();
			default:
				return parseExpression();
			}
		}

		Node Parser::parseExpression()
		{
			IF_ERROR_RETURN_NODE;
			Node exp = parseAssignmentExpression();
			if (mCurrentToken->value == ";")
				advance();
			return exp;
		}
	}
}