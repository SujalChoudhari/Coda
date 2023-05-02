#pragma once
#include <iostream>
#include <string> 
#include <vector>
#include "NodeType.h" 

namespace Coda {
	namespace FrontEnd {
		class Node {
		public:
			NodeType type;
			std::shared_ptr<Node> left;
			std::string value;
			std::shared_ptr<Node> right;

		public:

			Node(
				NodeType type = NodeType::PROGRAM,
				std::string value = "",
				std::shared_ptr<Node> left = nullptr,
				std::shared_ptr<Node> right = nullptr)
				: type(type), value(value), left(std::move(left)), right(std::move(right)) {}
			Node(const Node& other);

			friend std::ostream& operator<<(std::ostream& os, const Node& node);
		};

	}
}