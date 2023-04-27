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
	void parseStatement(Node* outStatement);
	void parseExpression(Node* outExpression);

	void parsePrimaryExpression(Node* outExpression);
	//void parseUnaryExpression(Node* outExpression);
	void parseMultiplacativeExpression(Node* outExpression);
	void parseAdditiveExpression(Node* outExpression);

private:
	std::vector<Token>* mTokens;
	Token* mCurrentToken;
	int mCurrentIndex = -1;
};

