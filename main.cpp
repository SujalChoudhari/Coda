#include <iostream>
#include "Utils/FileReader.h"
#include "Coda.h"



int repl() {

	Coda::Runtime::Environment env = Coda::Runtime::Environment::root();
	while (1) {
		Coda::Error::Manager::reset();

		std::string source;
		std::cout << ">> ";
		std::getline(std::cin, source);
		if (source == ":q" || source == "exit" || source == "quit") break;


		if (source.empty()) continue;

		Coda::Frontend::Lexer lexer = Coda::Frontend::Lexer();
		std::vector<Coda::Frontend::Token> tokens = lexer.tokenise(source);

		IF_ERROR_CONTINUE;
		Coda::Frontend::Parser parser = Coda::Frontend::Parser();
		Coda::Frontend::Program program = parser.parse(tokens);

		IF_ERROR_CONTINUE;
		Coda::Runtime::Interpreter inter = Coda::Runtime::Interpreter();
		Coda::Runtime::Value out = inter.evaluateProgram(program, env);

	}
	return 0;
}

int main() {

#ifdef _DEBUG
	Coda::Runtime::Environment env = Coda::Runtime::Environment::root();

	Coda::Utils::FileReader fr = { "Test/test.coda" };
	std::string source = fr.readToString();

	if (source.empty())
		return 0;

	Coda::Frontend::Lexer lexer = Coda::Frontend::Lexer();
	std::vector<Coda::Frontend::Token> tokens = lexer.tokenise(source);

	IF_ERROR_RETURN(1);
	Coda::Frontend::Parser parser = Coda::Frontend::Parser();
	Coda::Frontend::Program program = parser.parse(tokens);

	IF_ERROR_RETURN(1);
	Coda::Runtime::Interpreter inter = Coda::Runtime::Interpreter();
	Coda::Runtime::Value out = inter.evaluateProgram(program, env);

#else
	return repl();

#endif
}