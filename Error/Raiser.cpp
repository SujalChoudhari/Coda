#include "Error.h"
#include "../Utils/Colors.h"
namespace Coda {
	namespace Error {
		std::ostream& operator<<(std::ostream& os, const Position& pos) {
			os << Utils::Colors::WARNING << "Line: " << pos.line
				<< " Character: " << pos.character << Utils::Colors::ERROR << std::endl;


			if (pos.lineText.empty())
				return os;

			// Print the line of text
			os << pos.lineText << std::endl;

			// Print the arrows pointing to the location of the error
			for (unsigned int i = 0; i < pos.character - 1; i++) {
				os << "~";
			}
			os << "^" << Utils::Colors::RESET;

			return os;
		}

		void Importer::raise(std::string error) {
			Manager::raiseError();
			std::cout << Utils::Colors::ACCENT << "[IMPTR]: " << Utils::Colors::ERROR << error << Utils::Colors::RESET << std::endl;
		}

		void Lexer::raise(std::string error, const Position& pos) {
			Manager::raiseError();
			std::cout << Utils::Colors::ACCENT << "[LEXER]: " << Utils::Colors::ERROR << error << pos << std::endl;
		}

		void Parser::raise(std::string error, const Position& pos) {
			Manager::raiseError();
			std::cout << Utils::Colors::ACCENT << "[PARSER]: " << Utils::Colors::ERROR << error << pos << std::endl;
		}

		void Runtime::raise(std::string error, const Position& pos) {
			Manager::raiseError();
			std::cout << Utils::Colors::ACCENT << "[RNTIME]: " << Utils::Colors::ERROR << error << pos << std::endl;
		}
		void Runtime::raise(std::string error) {
			Manager::raiseError();
			std::cout << Utils::Colors::ACCENT << "[RNTIME]: " << Utils::Colors::ERROR << error << std::endl;
		}


	}
}