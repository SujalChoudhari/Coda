#pragma once
#include <optional>
#include <string> 
#include <vector>
#include "NodeType.h" 


class Node {
public:
	NodeType type;
	Node* left;
	std::string value;
	Node* right;
	std::vector<Node> body; // only when Type is Program
public:
	Node(NodeType type=NodeType::PROGRAM, std::string value = "", Node* left = nullptr, Node* right = nullptr)
		: type(type), value(value), left(left), right(right) {}
};