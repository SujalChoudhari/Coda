#pragma once

/*
	Frontend

	This Sub package is used tokenize, parse and generate the AST.
	Classes such as Node, Token, TokenType, Lexer, Parser, Program are defined here.

	Tokens:
	A Token is a single unit of the source code. It is used to tokenize the source code.

	Lexer:
	Frontend::Lexer is used to tokenize the source code.

	Node:
	A Node is a single unit of the AST. It is used to generate the AST.

	Parser:
	Frontend::Parser is used to parse the tokens.
*/

#include "Lexer/Lexer.h"

#include "Node/Node.h"
#include "Node/NodeType.h"
#include "Node/Program.h"

#include "Parser/Parser.h"

#include "Tokens/Constants.h"
#include "Tokens/Token.h"
#include "Tokens/TokenType.h"