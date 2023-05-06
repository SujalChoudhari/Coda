#pragma once
#include <iostream>
#include "Position.h"
#include "Manager.h"

namespace Coda {
	namespace Error {
		namespace Lexer {
			void raise(std::string, const Position& pos);
		}

		namespace Parser {
			void raise(std::string, const Position& pos);
		}

		namespace Runtime {
			void raise(std::string, const Position& pos);
			void raise(std::string);
		}
	}

}

