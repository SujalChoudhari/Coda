#include "Interpreter.h"

#include "../Error/Error.h"
namespace Coda {
	namespace Runtime {
		Value Interpreter::evaluate(FrontEnd::Node astNode)
		{
			Value value = Value(); // Defaults to NONE

			if (astNode.type == FrontEnd::NodeType::NUMERIC_LITERAL) {
				value.type = Type::NUMBER;
				value.value = astNode.value;
				//value.position = astNode.
			}

			else if (astNode.type == FrontEnd::NodeType::NONE_LITERAL) {
				value.type = Type::NONE;
				value.value = astNode.value;
			}

			else {
				Error::Runtime::raiseUnrecognisedASTNodeError(astNode.value, astNode.position);
				exit(1);
			}

			return value;
		}

		

	}
}

