#include "Parser.h"
#include "../AST/Node.h"
Parser::Parser()
{

}

void Parser::parse(std::string sourceCode, Node* outProgram)
{
	Lexer lex = Lexer();
	std::vector<Token> tokens; 
	lex.tokenise(sourceCode, &tokens);

	for (Token t : tokens) {
		currentToken = &t;
		Node s = Node();
		parseStatement(&s);
		outProgram->body.emplace_back(s);

	}
}

void Parser::parseStatement(Node* outStatement)
{
	parseExpression(outStatement);
}

void Parser::parseExpression(Node* outExpression)
{

	parsePrimaryExpression(outExpression);
}

void Parser::parsePrimaryExpression(Node* outExpression)
{
}
