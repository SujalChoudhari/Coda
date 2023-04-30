#include <string>
#include "../Tokens/Keywords.h"
#include "Lexer.h"
Lexer::Lexer()
{
}

void Lexer::tokenise(std::string sourceCode, std::vector<Token>* outTokens)
{
	const char* src = sourceCode.c_str();

	for (unsigned int i = 0; i < sourceCode.size(); i++) {
		if (src[i] == '(')
			outTokens->emplace_back(TokenType::OPEN_PAREN, "(");
		else if (src[i] == ')')
			outTokens->emplace_back(TokenType::CLOSE_PAREN, ")");
		else if (src[i] == '+' || src[i] == '-' || src[i] == '*' || src[i] == '/' || src[i] == '*')
		{
			std::string s(1,src[i]);
			outTokens->emplace_back(TokenType::BINARY_OPERATOR, s);
		}
		else if (src[i] == '=')
			outTokens->emplace_back(TokenType::EQUALS, "=");
		else {
			// multisrc[i] token handler
			// build numbers
			if (isdigit(src[i])) {
				std::string num = "";
				while (isdigit(src[i]) && i < sourceCode.size()) {
					num.push_back(src[i]);
					i++;
				}

				outTokens->emplace_back(TokenType::NUMBER, num);
			}
			//build identifier
			else if (isalpha(src[i])) {
				std::string identifier = "";
				while (isalpha(src[i]) && i < sourceCode.size()) {
					identifier.push_back(src[i]);
					i++;
				}
				auto iden = KEYWORD.find(identifier);
				if (iden == KEYWORD.end())
				{
					outTokens->emplace_back(TokenType::IDENTIFIER, identifier);
				}
				else {
					outTokens->emplace_back(KEYWORD[identifier], identifier);
				}
			}

		}

	}

	outTokens->emplace_back(TokenType::END_OF_FILE, "\\0");

}
