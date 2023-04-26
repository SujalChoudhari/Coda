#pragma once
#include "../AST/Node.h"
#include "../Tokens/Token.h"
#include "../Tokens/TokenType.h"
#include "../Lexer/Lexer.h"
class Parser
{
public:
	Parser();
	void parse(std::string sourceCode, Node* outProgram);

private:
	std::vector<Token> mTokens;
	Token* currentToken;

private:
	void parseStatement(Node* outStatement);
	void parseExpression(Node* outExpression);
	void parsePrimaryExpression(Node* outExpression);
};

