#pragma once
#include <vector>
#include "Node.h"
namespace Coda {
	namespace FrontEnd {
		class Program {
		public:
			std::vector<Node> body = std::vector<Node>();

			friend std::ostream& operator<<(std::ostream& os, const Program& node);

		};
	}
}