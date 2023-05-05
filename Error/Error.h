#pragma once
#include <iostream>
#include "Position.h"
#include "Manager.h"

namespace Coda {
	namespace Error {
		namespace Lexer {
			void raiseIllegalCharacterError(const char character, const Position& pos);
			void raiseUnexpectedCharacterError(const char character, const Position& pos);
		}

		namespace Parser {
			void raiseUnexpectedTokenError(
				const std::string expected,
				const std::string got,
				const Position& pos);
			void raiseInvalidTokenFoundError(const std::string found, const Position& pos);
		}

		namespace Runtime {
			void raiseUnrecognisedASTNodeError(
				const std::string got,
				const Position& pos
			);
			void raiseDivisionByZeroError(const Position& pos);
			void raiseTypeError(std::string message, const Position& pos);
		}
	}

}

