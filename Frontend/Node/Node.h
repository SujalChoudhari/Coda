#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../../FFI/INode.h"
#include "NodeType.h"
#include "../../Error/Position.h"

#define agrs properties

namespace Coda {
	namespace Frontend {
		/*
			A node is a single element in the AST.
			Nodes are used to generate the AST.
			Nodes are arranged into a binary tree to represent the Abstract Syntax Tree.
		*/
		class Node : public INode {
		public:
			// The type of the node.
			NodeType type;

			// The immediate left child of the node.
			std::shared_ptr<Node> left;
			
			// The value of the node.
			std::string value;
			
			// The immediate right child of the node.
			std::shared_ptr<Node> right;
			
			// The start position of the node.
			Error::Position startPosition;
			
			// The end position of the node.
			Error::Position endPosition;
			
			// The properties of the node, used to store additional information.
			std::map<std::string, std::shared_ptr<INode>> properties;
			
		public:
			// Cretae a new node with the given information.
			Node(
				NodeType type = NodeType::INVALID,
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

			// Create a new node with the given node as a copy.
			Node(const Node& other);

			friend std::ostream& operator<<(std::ostream& os, const Node& node);

			// Inherited via INode
			virtual NodeType getType() const override;
			virtual std::string getValue() const override;
			virtual std::shared_ptr<INode> getLeft() const override;
			virtual std::shared_ptr<INode> getRight() const override;
			virtual std::map<std::string, std::shared_ptr<INode>> getProperties() const override;

			// Inherited via INode
			virtual Position getStartPosition() const override;
			virtual Position getEndPosition() const override;
		};
	} // namespace Frontend
} // namespace Coda
