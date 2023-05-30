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
		/*
			Interprets the AST.
			Execution of the code is handled here.
		*/
		class Interpreter {
		public:
			/*
				interpret the AST.
				@param astNode - the root node of the AST.
				@param env - the environment in which the AST will be interpreted.
				@return - the value of the AST.
			*/
			Value interpret(const Frontend::Node& astNode, Environment& env);

			/*
				evaluate the Program.
				@param program - the program to be evaluated.
				@param env - the environment in which the program will be evaluated.
				@return - the value of the program.
			*/
			Value evaluateProgram(const Frontend::Program& program, Environment& env);

		private:
			Value evaluateBinaryExpression(const Frontend::Node& binop, Environment& env);
			Value evaluateNumericBinaryExpression(const Value& left, const std::string& functor, const Value& right);
			Value evaluateStringBinaryExpression(const Value& left, const std::string& functor, const Value& right);
			Value evaluateIdentifier(const Frontend::Node& astNode, Environment& env);
			Value evaluateObjectExpression(const Frontend::Node& object, Environment& env);
			Value evaluateCallExpression(const Frontend::Node& callexp, Environment& env);

			Value evaluateAssignmentExpression(const Frontend::Node& astNode, Environment& env);
			Value evaluateMemberExpression(const Frontend::Node& astNode, Environment& env);
			Value evaluateVariableDeclaration(const Frontend::Node& astNode, Environment& env, bool isConstant = false);
			Value evaluateFunctionDeclaration(const Frontend::Node& astNode, Environment& env);

			bool isNumericType(Type type);
			bool isStringType(Type type);
			bool isUndefinedType(Type type);

			UserDefinedFunction getFunction(const std::string& name);

			template <typename T>
			void handleArithmeticOperation(const Value& left, const std::string& functor, const Value& right, Value& result);
			Value handleModulusOperation(const Value& left, const Value& right);
			template <typename T>
			T getValue(const std::string& str);

		private:
			std::vector<UserDefinedFunction> userDefinedFunctions;
		};

	}  // namespace Runtime
}  // namespace Coda
