#pragma once
#include <vector>
#include "Node.h"
namespace Coda {
	namespace Frontend {
		/*
			A program is a collection of nodes.
			It represents entire source code as a single node arranged in a binary tree.
		*/
		class Program {
		public:
			// The body of the program.
			std::vector<Node> body = std::vector<Node>();

			friend std::ostream& operator<<(std::ostream& os, const Program& node);

		};
	} // namespace Frontend
} // namespace Coda