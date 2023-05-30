#pragma once
#include <iostream>
#include "Position.h"
#include "Manager.h"

namespace Coda {
	namespace Error {
		namespace Lexer {
			// Raise a lexer error.
			void raise(std::string, const Position& pos);
		}

		namespace Parser {
			// Raise a parser error.
			void raise(std::string, const Position& pos);
		}

		namespace Runtime {
			// Raise a runtime error.
			void raise(std::string, const Position& pos);

			// Raise a runtime error without a position.
			void raise(std::string);
		}
	} // namespace Error
} // namespace Coda

