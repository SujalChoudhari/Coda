#include <iostream>

#include "Coda.h"

int repl() {

	Coda::Runtime::Environment env = Coda::Runtime::Environment();

	env.declareOrAssignVariable("none", Coda::Runtime::Value(Coda::Runtime::Type::NONE, "none", Position(), Position()));
	env.declareOrAssignVariable("undefined", Coda::Runtime::Value(Coda::Runtime::Type::UNDEFINED, "undefined", Position(), Position()));
	env.declareOrAssignVariable("true", Coda::Runtime::Value(Coda::Runtime::Type::BOOL, "1", Position(), Position()));
	env.declareOrAssignVariable("false", Coda::Runtime::Value(Coda::Runtime::Type::BOOL, "0", Position(), Position()));

	while (1) {
		Coda:: Error::Manager::reset();

		std::string source;
		std::cout << ">> ";
		std::getline(std::cin, source);
		if (source == ":q" || source == "exit" || source == "quit") break;


		if (source.empty()) continue;

		Coda::Frontend::Lexer lexer = Coda::Frontend::Lexer();
		std::vector<Coda::Frontend::Token> tokens = lexer.tokenise(source);

		if (!Coda::Error::Manager::isSafe())
			continue;

		Coda::Frontend::Parser parser = Coda::Frontend::Parser();
		Coda::Frontend::Program program = parser.parse(tokens);

		if (!Coda::Error::Manager::isSafe())
			continue;

		Coda::Runtime::Interpreter inter = Coda::Runtime::Interpreter();
		Coda::Runtime::Value out = inter.evaluateProgram(program, env);

		std::cout << out << std::endl;
	}
	return 0;
}

int main() {

	return repl();
}