#pragma once

#include "Coda.h"
#include "../Utils/FileReader.h"
#include "../Utils/ArgParser.h"
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
		const std::string title = "\033[1;96m\n\
                                                    dddddddd                  \n\
        CCCCCCCCCCCCC                               d::::::d                   \n\
     CCC::::::::::::C                               d::::::d                   \n\
   CC:::::::::::::::C                               d::::::d           \033[0mv1.0.1\033[1;96m        \n\
  C:::::CCCCCCCC::::C                               d:::::d                    \n\
 C:::::C       CCCCCC    ooooooooooo        ddddddddd:::::d    aaaaaaaaaaaaa   \n\
C:::::C                oo:::::::::::oo    dd::::::::::::::d    a::::::::::::a  \n\
C:::::C               o:::::::::::::::o  d::::::::::::::::d    aaaaaaaaa:::::a \n\
C:::::C               o:::::ooooo:::::o d:::::::ddddd:::::d             a::::a \n\
C:::::C               o::::o     o::::o d::::::d    d:::::d      aaaaaaa:::::a \n\
C:::::C               o::::o     o::::o d:::::d     d:::::d    aa::::::::::::a \n\
C:::::C               o::::o     o::::o d:::::d     d:::::d   a::::aaaa::::::a \n\
 C:::::C       CCCCCC o::::o     o::::o d:::::d     d:::::d  a::::a    a:::::a \n\
  C:::::CCCCCCCC::::C o:::::ooooo:::::o d::::::ddddd::::::dd a::::a    a:::::a \n\
   CC:::::::::::::::C o:::::::::::::::o  d:::::::::::::::::d a:::::aaaa::::::a \n\
     CCC::::::::::::C  oo:::::::::::oo    d:::::::::ddd::::d  a::::::::::aa:::a\n\
        CCCCCCCCCCCCC    ooooooooooo       ddddddddd   ddddd   aaaaaaaaaa  aaaa\033[0m";

		const std::string helpMessage = "\033[0;33m\n\
========================================\n\
   Welcome to Coda Programming Language\n\
========================================\033[0m\n\
\n\
   \033[1;36m.-=[ Resources ]=-.\033[0m\n\
   \n\
   Documentation: \033[4;34mhttps://github.com/SujalChoudhari/Coda/wiki\033[0m\n\
   Issue Tracker: \033[4;34mhttps://github.com/SujalChoudhari/Coda/issues\033[0m\n\
   Repository: \033[4;34mhttps://github.com/SujalChoudhari/Coda\033[0m\n\n\
   View command list: \033[36mCoda commands\033[0m\n\
   Visit the wiki for more information: \033[4;34mhttps://github.com/SujalChoudhari/Coda/wiki\033[0m\n\
\n\
\033[0;33m========================================\033[0m\n";

		const std::map<std::string, std::string> commandsMap = {
			{"run","Run a Coda program from a specified file.\nExample: run <filename> -w(Optional : -w to wait)"},
			{"help","Display this help message."},
			{"repl","Start a terminal-based REPL for the Coda language."}
		};


	public:
		/*
			Entry point of the Application.
			Based on the arguments passed to the program, it will either
			start the REPL or execute a file.
			@param argc - The number of arguments passed to the program.
			@param argv - The arguments passed to the program.
		*/
		int run(int argc, char** argv);

		/*
			A Read-Eval-Print-Loop that allows the user to execute Coda code line by line.
			There is no need of file to be passed to the program.
		*/
		int repl();

		/*
			Combines the three steps of the interpreter - tokenization, parsing and interpretation.
			@param source - The source code to be executed.
			@param env - The environment in which the code will be executed.
			@return - 0 if the execution was successful, 1 otherwise.
		*/
		int execute(std::string sourc, Coda::Runtime::Environment& env);

		/*
			Tokenizes the source code.
			Internally, it uses the Lexer class.
			@param source - The source code to be tokenized.
			@return - A vector of tokens.
		*/
		std::vector<Coda::Frontend::Token> tokenize(std::string source);

		/*
			Parses the tokens.
			Internally, it uses the Parser class.
			@param tokens - The tokens to be parsed.
			@return - A Program object.
		*/
		Frontend::Program parse(std::vector<Coda::Frontend::Token>& tokens);

		/*
			Interprets the program.
			Internally, it uses the Interpreter class.
			@param program - The program to be interpreted.
			@param env - The environment in which the program will be interpreted.
			@return - The last calculated value.
		*/
		Runtime::ValuePtr interpret(Frontend::Program& program, Runtime::Environment& env);


		void printHelpMessage();
		void printCommandMessage();

	private:
		std::string mMainFileName;

	private:
		int runnable(int argc, char** argv);
	};
} // namespace Coda