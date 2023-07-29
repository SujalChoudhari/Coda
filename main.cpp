#include <iostream>

#include "Utils/FileReader.h"
#include"Utils/ArgParser.h"
#include "Coda.h"

namespace Coda {
	/*
		The Application class is the entry point of the Coda interpreter.
		Depending on the arguments passed to the program, it will either
		start the REPL or execute a file.
	*/
	class Application {
	public:
		/*
			Entry point of the Application.
			Based on the arguments passed to the program, it will either
			start the REPL or execute a file.
			@param argc - The number of arguments passed to the program.
			@param argv - The arguments passed to the program.
		*/
		static int run(int argc, char** argv) {
			Coda::Utils::ArgParser argParser = Coda::Utils::ArgParser();
			argParser.parse(argc, argv);
			std::string filename;

			try {
#if _DEBUG
				filename = "Test/debug.coda";
#else 
				filename = argParser.getStandaloneValueAt(0);
#endif
			}
			catch (std::out_of_range e) {
				return repl();
			}

			Coda::Utils::FileReader fr = { filename };
			std::string source = fr.readToString();
			Runtime::Environment env = Runtime::Environment::root();
			execute(source, env);

			if (argParser.getFlag("-w")) {
				std::cout << std::endl << "Press any key to continue...";
				std::cin.get();
			}

			return 0;
		}

		/*
			A Read-Eval-Print-Loop that allows the user to execute Coda code
			line by line. There is no need of file to be passed to the program.
		*/
		static int repl() {
			std::cout << "Welcome to Coda REPL!" << std::endl;
			std::cout << "Type :q to exit" << std::endl;

			Runtime::Environment env = Runtime::Environment::root();
			while (1) {
				Coda::Error::Manager::reset();
				std::string source;
				std::cout << ">> ";
				std::getline(std::cin, source);
				if (source == ":q" || source == "exit" || source == "quit" || source == "e" || source == "q") break;

				execute(source, env);
			}
			return 0;
		}
		/*
			Combines the three steps of the interpreter - tokenization, parsing and interpretation.
			@param source - The source code to be executed.
			@param env - The environment in which the code will be executed.
			@return - 0 if the execution was successful, 1 otherwise.
		*/
		static int execute(std::string source, Coda::Runtime::Environment& env) {
			if (source.empty()) {
				return 1;
			}
			std::vector<Coda::Frontend::Token> tokens = tokenize(source);
			IF_ERROR_RETURN(1);
			Coda::Frontend::Program program = parse(tokens);
			IF_ERROR_RETURN(1);
			interpret(program, env);
			return 0;
		}
		/*
			Tokenizes the source code.
			Internally, it uses the Lexer class.
			@param source - The source code to be tokenized.
			@return - A vector of tokens.
		*/
		static std::vector<Coda::Frontend::Token> tokenize(std::string source) {
			IF_ERROR_RETURN(std::vector<Coda::Frontend::Token>());
			Coda::Frontend::Lexer lexer = Coda::Frontend::Lexer();
			return lexer.tokenize(source);
		}
		/*
			Parses the tokens.
			Internally, it uses the Parser class.
			@param tokens - The tokens to be parsed.
			@return - A Program object.
		*/
		static Frontend::Program parse(std::vector<Coda::Frontend::Token>& tokens) {
			IF_ERROR_RETURN(Frontend::Program());
			Coda::Frontend::Parser parser = Coda::Frontend::Parser();
			return parser.parse(tokens);
		}

		/*
			Interprets the program.
			Internally, it uses the Interpreter class.
			@param program - The program to be interpreted.
			@param env - The environment in which the program will be interpreted.
			@return - The last calculated value.
		*/
		static Runtime::ValuePtr interpret(Frontend::Program& program, Runtime::Environment& env) {
			IF_ERROR_RETURN_VALUE_PTR;
			Coda::Runtime::Interpreter inter = Coda::Runtime::Interpreter();
			return inter.evaluateProgram(program, env);
		}
	};
}

int main(int argc, char** argv) {
	Coda::Application::run(argc, argv);
}

