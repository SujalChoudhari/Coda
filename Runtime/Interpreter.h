#pragma once

#include "Value.h"
#include "Type.h"
#include "../Frontend/Node/NodeType.h"
#include "../Frontend/Node/Node.h"
#include "../Frontend/Node/Program.h"

namespace Coda {
	namespace Runtime {

		class Interpreter {

		public:
			Value evaluate(FrontEnd::Node astNode);
			Value evaluateProgram(FrontEnd::Program program);

		private:
			Value evaluateBinaryExpression(FrontEnd::Node binop);
			Value evaluateNumericBinaryExpression(Value left,std::string functor, Value right);

		};

	}
}