#include "Application.h"
#include "../Utils/Colors.h"



namespace Coda {
	int Application::run(int argc, char** argv) {
		int result = EXIT_SUCCESS;
		try {
			return runnable(argc, argv);
		}
		catch (const std::exception& e) {
			std::cerr << Utils::Colors::ACCENT << "[CODA ]: \033[0;31mA fatal error occurred in the interpreter.\n" << e.what() << Utils::Colors::RESET << std::endl;
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

		std::string subCommand = argParser.getSubCommand();

		if (subCommand == "repl") {
			return repl();
		}
		else if (subCommand == "help" || subCommand == "none") {
			printHelpMessage();
			std::cin.get();
			return EXIT_SUCCESS;
		}
		else if (subCommand == "commands") {
			printCommandMessage();
			std::cin.get();
			return EXIT_SUCCESS;
		}
		else {
			Frontend::Importer importer = Frontend::Importer();
			std::string source;
			if (subCommand == "run") {
				mMainFileName = argParser.getStandaloneValueAt(0);
				mMainFileName = mMainFileName.empty() ? "./.coda" : mMainFileName;
				source = importer.import(mMainFileName);

			}
			else if (importer.fileExists(subCommand)) {
				mMainFileName = subCommand;
				source = importer.import(mMainFileName);
			}
			else {

				mMainFileName = "./.coda";
				source = importer.import(mMainFileName);

				source += "\n" + subCommand + "(";
				std::vector<std::string>* parameters = argParser.getAllStandaloneValues();
				if (parameters->size() != 0) {
					for (int i = 0; i < parameters->size() - 1; i++) {
						source += "\"" + parameters->at(i) + "\",";
					}
					source += "\"" + parameters->at(parameters->size() - 1) + "\"";
				}
				source += ");\n";
			}

			Runtime::Environment env = Runtime::Environment::root();
			execute(source, env);

			if (argParser.getFlag("-w")) {
				std::cout << std::endl << "Press any key to continue...";
				std::cin.get();
			}
			return EXIT_SUCCESS;
		}
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
			Frontend::Importer importer = Frontend::Importer();
			source = importer.import(source);
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
		auto res = interpret(program, env);
		IF_ERROR_RETURN(EXIT_FAILURE);

		if (mMainFileName.empty() && !res->getValue().empty()) {
			std::cout << res->getValue() << std::endl;
		}

		return result;
	}

	void Application::printHelpMessage() {
		std::cout << title << std::endl;
		std::cout << helpMessage << std::endl;
	}

	void Application::printCommandMessage()
	{

		std::cout << Utils::Colors::ACCENT << "\n"
			<< "   \n"
			<< Utils::Colors::ACCENT << ".-=[ Commands ]=-." << Utils::Colors::RESET << "\n"
			<< "\n"
			<< Utils::Colors::ACCENT << "Usage:" << Utils::Colors::RESET << "\n";

		for (const auto& entry : commandsMap) {
			std::cout << " - " << Utils::Colors::ACCENT << entry.first << Utils::Colors::RESET << "\n   " << entry.second << "\n";
		}

		std::cout << "\n"
			<< Utils::Colors::ACCENT << "Additional Resources:" << Utils::Colors::RESET << "\n"
			<< "   - Documentation: " << Utils::Colors::ACCENT << "\033[4;34mhttps://github.com/SujalChoudhari/Coda/wiki" << Utils::Colors::RESET << "\n"
			<< "   - Issue Tracker: " << Utils::Colors::ACCENT << "\033[4;34mhttps://github.com/SujalChoudhari/Coda/issues" << Utils::Colors::RESET << "\n"
			<< "   - Repository: " << Utils::Colors::ACCENT << "\033[4;34mhttps://github.com/SujalChoudhari/Coda" << Utils::Colors::RESET << "\n"
			<< "   - View more commands: " << Utils::Colors::ACCENT << "\033[36mCoda commands" << Utils::Colors::RESET << "\n"
			<< "   - Visit the wiki for detailed information: " << Utils::Colors::ACCENT << "\033[4;34mhttps://github.com/SujalChoudhari/Coda/wiki" << Utils::Colors::RESET << "\n"
			<< "\n"
			<< Utils::Colors::WARNING << "========================================" << Utils::Colors::RESET << "\n";
	}


	std::vector<Coda::Frontend::Token> Application::tokenize(std::string source) {
		IF_ERROR_RETURN(std::vector<Coda::Frontend::Token>());
		Coda::Frontend::Lexer lexer = Coda::Frontend::Lexer(mMainFileName.empty() ? "<repl>" : mMainFileName);
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
		inter.callStack.push(Position(0, 0, "", mMainFileName.empty() ? "<repl>" : mMainFileName));
		return inter.evaluateProgram(program, env);
	}
}