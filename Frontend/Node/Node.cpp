#include "Node.h"
#include "Program.h"

namespace Coda {
	namespace Frontend {

		std::ostream& operator<<(std::ostream& os, const Node& node)
		{
			static const int INDENTATION_SIZE = 2;
			static int depth = 0;
			os << std::string(depth, ' ') << "{ ";
			if (!node.value.empty()) {
				os << "\n" << std::string(depth + INDENTATION_SIZE, ' ')
					<< "value: " << node.value;
			}

			if (node.left) {
				depth += INDENTATION_SIZE;
				os << "," << "\n" << std::string(depth, ' ')
					<< "left: " << *node.left;
				depth -= INDENTATION_SIZE;
			}

			if (node.right) {
				depth += INDENTATION_SIZE;
				os << "," << "\n" << std::string(depth, ' ')
					<< "right: " << *node.right << ",";
				depth -= INDENTATION_SIZE;
			}
			os << "\n" << std::string(depth, ' ') << "}";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Program& program)
		{
			static const int INDENTATION_SIZE = 2;
			static int depth = 0;

			os << "[\n";

			if (!program.body.empty()) {
				depth += INDENTATION_SIZE;
				for (const Node& child : program.body) {
					os << "\n" << std::string(depth, ' ') << child << ",";
				}
				depth -= INDENTATION_SIZE;
			}

			os << "\n]\n";

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

			startPosition = other.startPosition;
			endPosition = other.endPosition;
			properties = other.properties;

		}
		NodeType Node::getType() const
		{
			return this->type;
		}
		std::string Node::getValue() const
		{
			return this->value;
		}
		std::shared_ptr<INode> Node::getLeft() const
		{
			return this->left;
		}
		std::shared_ptr<INode> Node::getRight() const
		{
			return this->right;
		}
		std::map<std::string, std::shared_ptr<INode>> Node::getProperties() const
		{
			return properties;
		}
		Position Node::getStartPosition() const
		{
			return startPosition;
		}
		Position Node::getEndPosition() const
		{
			return endPosition;
		}
	}
}