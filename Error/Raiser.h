#pragma once
#include <iostream>
#include <stack>
#include "Position.h"
#include "Manager.h"

namespace Coda {
	namespace Error {

		namespace Importer {
			// Raise an importer error.
			void raise(std::string);
		}

		namespace Lexer {
			// Raise a lexer error.
			void raise(std::string, const Position& startPos, const Position& endPos);
		}

		namespace Parser {
			// Raise a parser error.
			void raise(std::string, const Position& startPos, const Position& endPos);
		}

		namespace Runtime {
			// Raise a runtime error.
			void raise(std::string error, std::stack<Position>& callStack, const Position& startPos, const Position& endPos);

			// Raise a runtime error without a position.
			void raise(std::string);
		}
	} // namespace Error
} // namespace Coda

