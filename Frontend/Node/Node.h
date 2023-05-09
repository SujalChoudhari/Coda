#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "NodeType.h"
#include "../../Error/Position.h"

#define agrs properties

namespace Coda {
	namespace Frontend {
		class Node {
		public:
			NodeType type;
			std::shared_ptr<Node> left;
			std::string value;
			std::shared_ptr<Node> right;
			Error::Position startPosition;
			Error::Position endPosition;

			std::map<std::string, std::shared_ptr<Node>> properties;
			
		public:
			Node(
				NodeType type = NodeType::PROGRAM,
				std::string value = "",
				std::shared_ptr<Node> left = nullptr,
				std::shared_ptr<Node> right = nullptr,
				Error::Position start = Error::Position(0, 0),
				Error::Position end = Error::Position(0, 0))
				: type(type),
				value(value),
				left(std::move(left)),
				right(std::move(right)),
				startPosition(start),
				endPosition(end) {}

			Node(const Node& other);

			friend std::ostream& operator<<(std::ostream& os, const Node& node);
		};


	}
}
