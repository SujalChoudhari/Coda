#pragma once
#include <iostream>
#include <string> 
#include <vector>
#include "NodeType.h" 
#include "../../Error/Position.h"

namespace Coda {
	namespace FrontEnd {
		class Node {
		public:
			NodeType type;
			std::shared_ptr<Node> left;
			std::string value;
			std::shared_ptr<Node> right;
			Error::Position position;
		public:

			Node(
				NodeType type = NodeType::PROGRAM,
				std::string value = "",
				std::shared_ptr<Node> left = nullptr,
				std::shared_ptr<Node> right = nullptr, Error::Position pos = Error::Position(0, 0))
				: type(type), value(value), left(std::move(left)), right(std::move(right)), position(pos) {}
			Node(const Node& other);

			friend std::ostream& operator<<(std::ostream& os, const Node& node);
		};

	}
}