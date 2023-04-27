#include "Node.h"

std::ostream& operator<<(std::ostream& os, const Node& node)
{
    static int depth = 0;
    os << std::string(depth, ' ');
    os << "{";
    if (!node.value.empty()) {
        os << "\n";
        depth += 2;
        os << std::string(depth, ' ');
        os << "value:";
        os << node.value;
        depth -= 2;
    }
    if (node.left) {
        os << "\n";
        depth += 2;
        os << std::string(depth, ' ');
        os << "left:";
        os << *node.left;
        depth -= 2;
    }
    if (node.right) {
        os << "\n";
        depth += 2;
        os << std::string(depth, ' ');
        os << "right:";
        os << *node.right;
        depth -= 2;
    }
    if (!node.body.empty()) {
        os << "\n";
        depth += 2;
        os << std::string(depth, ' ');
        os << "[";
        os << "\n";
        for (int i = 0; i < node.body.size(); i++) {
            os << node.body.at(i);
            if (i != node.body.size() - 1) {
                os << ",";
            }
            os << "\n";
        }
        os << std::string(depth, ' ');
        os << "]";
        depth -= 2;
    }
    os << "}";
    return os;
}


Node::Node(const Node& other)
	: type(other.type), value(other.value) {
	if (other.left) {
		left = std::make_unique<Node>(*other.left);
	}
	if (other.right) {
		right = std::make_unique<Node>(*other.right);
	}
	body.reserve(other.body.size());
	for (const auto& child : other.body) {
		body.emplace_back(child);
	}
}