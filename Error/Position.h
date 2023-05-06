#pragma once
#include <string>

namespace Coda {
	namespace Error {
		class Position {
		public:
			unsigned int character;
			unsigned int line;
			std::string lineText;

		public:
			Position()
				:character(1), line(1), lineText("") {}

			Position(unsigned int index, unsigned int line)
				:character(index), line(line), lineText("") {}
			Position(unsigned int index, unsigned int line, std::string lineText)
				:character(index), line(line), lineText(lineText) {}


			friend std::ostream& operator<<(std::ostream& os, const Position& pos);

		};
	}
}