#pragma once
#include "../AST/Node.h"
#include "../Tokens/Token.h"
#include "../Tokens/TokenType.h"
#include "../Lexer/Lexer.h"
class Parser
{
public:
	Parser();
	void parse(std::vector<Token> tokens, Node* outProgram);

private: // in the order of prescidence
	void advance();
	Node parseStatement();
	Node parseExpression();

	Node parsePrimaryExpression();
	//void parseUnaryExpression(Node* outExpression);
	Node parseMultiplacativeExpression();
	Node parseAdditiveExpression();

private:
	std::vector<Token>* mTokens;
	Token* mCurrentToken;
	int mCurrentIndex = -1;
};

