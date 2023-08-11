#include "Error.h"

namespace Coda {
	namespace Error {
		std::ostream& operator<<(std::ostream& os, const Position& pos) {
			os << "Line: " << pos.line
				<< " Character: " << pos.character << std::endl;
			

			if (pos.lineText.empty())
				return os;

			// Print the line of text
			os << pos.lineText << std::endl;

			// Print the arrows pointing to the location of the error
			for (unsigned int i = 0; i < pos.character -1; i++) {
				os << "~";
			}
			os << "^";

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


