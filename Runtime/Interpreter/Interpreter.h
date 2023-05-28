#pragma once

#include <tuple>

#include "../Environment/Environment.h"
#include "../RuntimeValue/Type.h"
#include "../RuntimeValue/Value.h"
#include "../../Frontend/Node/Node.h"
#include "../../Frontend/Node/NodeType.h"
#include "../../Frontend/Node/Program.h"


#define IF_ERROR_RETURN_VALUE if (!Error::Manager::isSafe()) return Value()

typedef std::tuple<std::string, Coda::Runtime::Environment, Coda::Frontend::Node> UserDefinedFunction;

namespace Coda {
	namespace Runtime {

		class Interpreter {
		public:
			Value evaluate(const Frontend::Node& astNode, Environment& env);
			Value evaluateProgram(const Frontend::Program& program, Environment& env);

		private:
			Value evaluateBinaryExpression(const Frontend::Node& binop, Environment& env);
			Value evaluateNumericBinaryExpression(const Value& left, const std::string& functor, const Value& right);
			Value evaluateStringBinaryExpression(const Value& left, const std::string& functor, const Value& right);
			Value evaluateIdentifier(const Frontend::Node& astNode, Environment& env);
			Value evaluateObjectExpression(const Frontend::Node& object, Environment& env);
			Value evaluateCallExpression(const Frontend::Node& callexp, Environment& env);

			Value evaluateAssignmentExpression(const Frontend::Node& astNode, Environment& env);
			Value evaluateVariableDeclaration(const Frontend::Node& astNode, Environment& env, bool isConstant = false);
			Value evaluateFunctionDeclaration(const Frontend::Node& astNode, Environment& env);

			template <typename T>
			void handleArithmeticOperation(const Value& left, const std::string& functor, const Value& right, Value& result);
			Value handleModulusOperation(const Value& left, const Value& right);
			template <typename T>
			T getValue(const std::string& str);

			bool isNumericType(Type type);
			bool isStringType(Type type);
			bool isUndefinedType(Type type);

			UserDefinedFunction getFunction(const std::string& name);


		private:
			std::vector<UserDefinedFunction> userDefinedFunctions;
		};

	}  // namespace Runtime
}  // namespace Coda
