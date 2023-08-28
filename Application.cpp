#include "Application.h"
#include "Utils/Colors.h"



namespace Coda {
	int Application::run(int argc, char** argv) {
		int result = EXIT_SUCCESS;
		try {
			return runnable(argc, argv);
		}
		catch (const std::exception& e) {
			std::cerr << Utils::Colors::ACCENT << "[CODA ]: \033[0;31mA fatal error occurred in the interpreter.\n" << Utils::Colors::RESET << e.what() << std::endl;
			result = EXIT_FAILURE;
		}
		catch (...) {
			std::cerr << Utils::Colors::ACCENT << "[CODA ]: \033[0;31mUnknown error occurred while executing the program." << Utils::Colors::RESET << std::endl;
			result = EXIT_FAILURE;
		}
		return EXIT_FAILURE;
	}

	int Application::runnable(int argc, char** argv)
	{
		Coda::Utils::ArgParser argParser = Coda::Utils::ArgParser();
		argParser.parse(argc, argv);

		if (argParser.getFlag("-h")) {
			printHelpMessage();
			std::cin.get();
			return EXIT_SUCCESS;
		}

#if _DEBUG
		mMainFileName = "Test/debug.coda";
#else 
		mMainFileName = argParser.getStandaloneValueAt(0);
#endif

		if (mMainFileName.empty()) {
			return repl();
		}

		Frontend::Importer importer = Frontend::Importer();
		std::string source = importer.import(mMainFileName);

		Runtime::Environment env = Runtime::Environment::root();
		if (source.empty()) {
			std::cout << "Source file is empty.\nOpening REPL...\n+------------------------+" << std::endl;
			return repl();
		}
		execute(source, env);

		if (argParser.getFlag("-w")) {
			std::cout << std::endl << "Press any key to continue...";
			std::cin.get();
		}

		return EXIT_SUCCESS;
	}


	int Application::repl() {
		std::cout << title << std::endl << std::endl;
		std::cout << Utils::Colors::WARNING << "==================================================================================" << Utils::Colors::RESET << std::endl;
		std::cout << Utils::Colors::SUCCESS << "Welcome to Coda REPL!" << Utils::Colors::RESET << std::endl;
		std::cout << Utils::Colors::ERROR << "Type 'quit' to exit REPL" << Utils::Colors::RESET << std::endl;

		Runtime::Environment env = Runtime::Environment::root();
		while (1) {
			Coda::Error::Manager::reset();
			std::string source;
			std::cout << Utils::Colors::ACCENT << ">> " << Utils::Colors::RESET;
			std::getline(std::cin, source);
			if (source == ":q" || source == "exit" || source == "quit" || source == "e" || source == "q") break;

			std::cout << Utils::Colors::SUCCESS;
			execute(source, env);
			std::cout << Utils::Colors::RESET;
		}
		return 0;
	}


	int Application::execute(std::string source, Coda::Runtime::Environment& env) {
		int result = EXIT_SUCCESS;
		if (source.empty()) {
			result = EXIT_FAILURE;
			return result;
		}

		std::vector<Coda::Frontend::Token> tokens = tokenize(source);

		IF_ERROR_RETURN(1);
		Coda::Frontend::Program program = parse(tokens);

		IF_ERROR_RETURN(1);
		interpret(program, env);


		return result;
	}

	void Application::printHelpMessage() {
		std::cout << title << std::endl;
		std::cout << helpMessage << std::endl;
	}


	std::vector<Coda::Frontend::Token> Application::tokenize(std::string source) {
		IF_ERROR_RETURN(std::vector<Coda::Frontend::Token>());
		Coda::Frontend::Lexer lexer = Coda::Frontend::Lexer();
		return lexer.tokenize(source);
	}

	Frontend::Program Application::parse(std::vector<Coda::Frontend::Token>& tokens) {
		IF_ERROR_RETURN(Frontend::Program());
		Coda::Frontend::Parser parser = Coda::Frontend::Parser(mMainFileName);
		return parser.parse(tokens);
	}


	Runtime::ValuePtr Application::interpret(Frontend::Program& program, Runtime::Environment& env) {
		IF_ERROR_RETURN_VALUE_PTR;
		Coda::Runtime::Interpreter inter = Coda::Runtime::Interpreter();
		return inter.evaluateProgram(program, env);
	}
}