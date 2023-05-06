#include "Error.h"

namespace Coda {
	namespace Error {
		std::ostream& operator<<(std::ostream& os, const Position& head) {
			os << "Line: " << head.line
				<< " Character: " << head.character;
			return os;
		}

		
		void Lexer::raise(std::string error, const Position& pos) {
			Manager::raiseError();
			std::cout << "[LEXER]: " << error << pos << std::endl;
		}

		void Parser::raise(std::string error, const Position& pos) {
			Manager::raiseError();
			std::cout << "[PARSER]: " << error << pos << std::endl;
		}

		void Runtime::raise(std::string error, const Position& pos) {
			Manager::raiseError();
			std::cout << "[RNTIME]: " << error << pos << std::endl;
		}
		void Runtime::raise(std::string error) {
			Manager::raiseError();
			std::cout << "[RNTIME]: " << error << std::endl;
		}


	}
}


