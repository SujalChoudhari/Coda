#pragma once
#include "Coda.h"
#include "Utils/FileReader.h"
#include"Utils/ArgParser.h"
#include <iostream>

/*
	The Application class is the entry point of the Coda interpreter.
	Depending on the arguments passed to the program, it will either
	start the REPL or execute a file.
*/
namespace Coda {
	class Application
	{
	public:
		/*
			Entry point of the Application.
			Based on the arguments passed to the program, it will either
			start the REPL or execute a file.
			@param argc - The number of arguments passed to the program.
			@param argv - The arguments passed to the program.
		*/
		static int run(int argc, char** argv);

		/*
			A Read-Eval-Print-Loop that allows the user to execute Coda code line by line. 
			There is no need of file to be passed to the program.
		*/
		static int repl();

		/*
			Combines the three steps of the interpreter - tokenization, parsing and interpretation.
			@param source - The source code to be executed.
			@param env - The environment in which the code will be executed.
			@return - 0 if the execution was successful, 1 otherwise.
		*/
		static int execute(std::string sourc, Coda::Runtime::Environment& env);

		/*
			Tokenizes the source code.
			Internally, it uses the Lexer class.
			@param source - The source code to be tokenized.
			@return - A vector of tokens.
		*/
		static std::vector<Coda::Frontend::Token> tokenize(std::string source);

		/*
			Parses the tokens.
			Internally, it uses the Parser class.
			@param tokens - The tokens to be parsed.
			@return - A Program object.
		*/
		static Frontend::Program parse(std::vector<Coda::Frontend::Token>& tokens);

		/*
			Interprets the program.
			Internally, it uses the Interpreter class.
			@param program - The program to be interpreted.
			@param env - The environment in which the program will be interpreted.
			@return - The last calculated value.
		*/
		static Runtime::ValuePtr interpret(Frontend::Program& program, Runtime::Environment& env);
	};
} // namespace Coda