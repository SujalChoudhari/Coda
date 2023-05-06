#pragma once

#include "../RuntimeValue/Value.h"
#include "../RuntimeValue/Type.h"
#include "../../Frontend/Node/NodeType.h"
#include "../../Frontend/Node/Node.h"
#include "../../Frontend/Node/Program.h"
#include "../Environment/Environment.h"

namespace Coda {
	namespace Runtime {

		class Interpreter {

		public:
			Value evaluate(Frontend::Node astNode,Environment& env);
			Value evaluateProgram(Frontend::Program program, Environment& env);

		private:
			Value evaluateBinaryExpression(Frontend::Node binop, Environment& env);
			Value evaluateNumericBinaryExpression(Value left,std::string functor, Value right);
			Value evaluateIdentifier(Frontend::Node astNode, Environment& env);

			// TODO: Implement these functions
			Value evaluateVariableDeclaration(Frontend::Node astNode, Environment& env);
			Value evaluateConstantDeclaration(Frontend::Node astNode, Environment& env);

			template <typename T>
			void handleArithmeticOperation(Value left, std::string functor, Value right, Value& result);
			Value handleModulusOperation(Value left, Value right);

			template <typename T>
			T getValue(std::string str);

		};

	}
}