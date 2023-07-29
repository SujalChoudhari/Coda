#pragma once

#include <tuple>

#include "../Environment/Environment.h"
#include "../RuntimeValue/Type.h"
#include "../RuntimeValue/Value.h"
#include "../../Frontend/Node/Node.h"
#include "../../Frontend/Node/NodeType.h"
#include "../../Frontend/Node/Program.h"



#define IF_ERROR_RETURN_VALUE_PTR if (!Error::Manager::isSafe()) return nullptr

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
			ValuePtr interpret(const Frontend::Node& astNode, Environment& env);

			/*
				evaluate the Program.
				@param program - the program to be evaluated.
				@param env - the environment in which the program will be evaluated.
				@return - the value of the program.
			*/
			ValuePtr evaluateProgram(const Frontend::Program& program, Environment& env);

		private:
			ValuePtr evaluateBinaryExpression(const Frontend::Node& binop, Environment& env);
			ValuePtr evaluateNumericBinaryExpression(const ValuePtr& left, const std::string& functor, const ValuePtr& right);
			ValuePtr evaluateStringBinaryExpression(const ValuePtr& left, const std::string& functor, const ValuePtr& right);
			ValuePtr evaluateIdentifier(const Frontend::Node& astNode, Environment& env);
			ValuePtr evaluateObjectExpression(const Frontend::Node& object, Environment& env);
			ValuePtr evaluateCallExpression(const Frontend::Node& callexp, Environment& env);
			
			ValuePtr evaluateAssignmentExpression(const Frontend::Node& astNode, Environment& env);
			ValuePtr evaluateMemberExpression(const Frontend::Node& astNode, Environment& env);
			ValuePtr evaluateVariableDeclaration(const Frontend::Node& astNode, Environment& env, bool isConstant = false);
			ValuePtr evaluateFunctionDeclaration(const Frontend::Node& astNode, Environment& env);

			bool isNumericType(Type type);
			bool isStringType(Type type);
			bool isUndefinedType(Type type);


			template <typename T>
			void handleArithmeticOperation(const ValuePtr& left, const std::string& functor, const ValuePtr& right, ValuePtr& result);
			ValuePtr handleModulusOperation(const ValuePtr& left, const ValuePtr& right);
			template <typename T>
			T getValue(const std::string& str);

		
		};

	}  // namespace Runtime
}  // namespace Coda
