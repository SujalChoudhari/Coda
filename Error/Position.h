#pragma once
#include <string>

namespace Coda {
	namespace Error {
		
		/*
			A position of a specific character/token/astNode in the source code.
			Keeps track of the source code line, line number and the character index.
		*/
		class Position {
		public:
			// The index of the character in the source code.
			unsigned int character;

			// The line number of the character in the source code.
			unsigned int line;

			// The text of the line the character is on.
			std::string lineText;

		public:
			/*
				Create a new Position at the start of the source code.
				character = 1, line = 1, lineText = ""
			*/
			Position()
				:character(1), line(1), lineText("") {}

			/*
				Create a new Position without the line text.
			*/
			Position(unsigned int index, unsigned int line)
				:character(index), line(line), lineText("") {}

			/*
				Create a new Position with the all the information.
			*/
			Position(unsigned int index, unsigned int line, std::string lineText)
				:character(index), line(line), lineText(lineText) {}


			friend std::ostream& operator<<(std::ostream& os, const Position& pos);

		};
	} // namespace Error
} // namespace Coda