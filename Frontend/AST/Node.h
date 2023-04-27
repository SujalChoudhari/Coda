#pragma once
#include <iostream>
#include <string> 
#include <vector>
#include "NodeType.h" 

class Node {
public:
    NodeType type;
    std::unique_ptr<Node> left;
    std::string value;
    std::unique_ptr<Node> right;
    std::vector<Node> body; // only when Type is Program

public:
    
    Node(NodeType type = NodeType::PROGRAM, std::string value = "", std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr)
        : type(type), value(value), left(std::move(left)), right(std::move(right)) {}
    Node(const Node& other);

    friend std::ostream& operator<<(std::ostream& os, const Node& node);
};


