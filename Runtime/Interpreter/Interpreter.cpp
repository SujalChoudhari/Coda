#include "Interpreter.h"
#include <string>
#include "../../Error/Error.h"
namespace Coda {
	namespace Runtime {
		Value Interpreter::evaluate(Frontend::Node astNode, Environment& env)
		{
			if (!Error::Manager::isSafe()) return Value();

			Value value = Value();
			value.startPosition = astNode.startPosition;
			value.endPosition = astNode.endPosition;


			if (astNode.type == Frontend::NodeType::IDENTIFIER) {
				return evaluateIdentifier(astNode, env);
			}

			else if (astNode.type == Frontend::NodeType::BYTE_LITERAL) {
				value.type = Type::BYTE;
				value.value = astNode.value;
			}

			else if (astNode.type == Frontend::NodeType::INTEGER_LITERAL) {
				value.type = Type::INT;
				value.value = astNode.value;
			}

			else if (astNode.type == Frontend::NodeType::LONG_INT_LITERAL) {
				value.type = Type::LONG;
				value.value = astNode.value;
			}

			else if (astNode.type == Frontend::NodeType::FLOATING_POINT_LITERAL) {
				value.type = Type::FLOAT;
				value.value = astNode.value;
			}

			else if (astNode.type == Frontend::NodeType::DOUBLE_LITERAL) {
				value.type = Type::DOUBLE;
				value.value = astNode.value;
			}

			else if (astNode.type == Frontend::NodeType::BINARY_EXPRESSION) {
				return evaluateBinaryExpression(astNode, env);
			}

			else if (astNode.type == Frontend::NodeType::VARIABLE_DECLARATION) {
				return evaluateDeclaration(astNode, env);
			}

			else if (astNode.type == Frontend::NodeType::CONSTANT_DECLARATION) {
				return evaluateDeclaration(astNode, env, true);
			}

			else {
				Error::Runtime::raise("Unrecognised ASTNode '" + astNode.value + "'");
			}
			return value;
		}

		Value Interpreter::evaluateBinaryExpression(Frontend::Node binop, Environment& env)
		{
			if (!Error::Manager::isSafe())
				return Value();

			Value lhs = evaluate(*binop.left.get(), env);
			Value rhs = evaluate(*binop.right.get(), env);

			if (
				(lhs.type == Type::INT
					|| lhs.type == Type::BOOL
					|| lhs.type == Type::BYTE
					|| lhs.type == Type::LONG
					|| lhs.type == Type::FLOAT
					|| lhs.type == Type::DOUBLE)
				&&
				(rhs.type == Type::INT
					|| rhs.type == Type::BOOL
					|| rhs.type == Type::BYTE
					|| rhs.type == Type::LONG
					|| rhs.type == Type::FLOAT
					|| rhs.type == Type::DOUBLE))
			{
				return evaluateNumericBinaryExpression(lhs, binop.value, rhs);
			}

			else if (lhs.type == Type::UNDEFINED || rhs.type == Type::UNDEFINED) {
				return Value(Type::UNDEFINED, "undefined", lhs.startPosition, rhs.endPosition);
			}

			else if (lhs.type != Type::NONE && rhs.type == Type::NONE) {
				return lhs;
			}

			else if (lhs.type == Type::NONE && rhs.type != Type::NONE) {
				return rhs;
			}

			else return Value();
		}

		Value Interpreter::evaluateNumericBinaryExpression(Value left, std::string functor, Value right) {

			if (!Error::Manager::isSafe()) {
				return Value();
			}

			if (functor == "%") {
				return handleModulusOperation(left, right);
			}

			Type suggestedType = left.type;
			if (right.type > left.type) {
				suggestedType = right.type;
			}

			Value result = Value(suggestedType, left.startPosition, right.endPosition);

			if (suggestedType == Type::BOOL) {
				handleArithmeticOperation<bool>(left, functor, right, result);
			}
			else if (suggestedType == Type::BYTE) {
				handleArithmeticOperation<unsigned char>(left, functor, right, result);
			}
			else if (suggestedType == Type::INT) {
				handleArithmeticOperation<int>(left, functor, right, result);
			}
			else if (suggestedType == Type::LONG) {
				handleArithmeticOperation<long>(left, functor, right, result);
			}
			else if (suggestedType == Type::FLOAT) {
				handleArithmeticOperation<float>(left, functor, right, result);
			}
			else if (suggestedType == Type::DOUBLE) {
				handleArithmeticOperation<double>(left, functor, right, result);
			}
			else {
				result.type = Type::NONE;
			}

			return result;
		}

		Value Interpreter::evaluateIdentifier(Frontend::Node astNode, Environment& env)
		{
			return env.lookupSymbol(astNode.value);
		}

		Value Interpreter::evaluateDeclaration(Frontend::Node astNode, Environment& env, bool isConstant)
		{
			return env.declareOrAssignVariable(astNode.left->value, evaluate(*astNode.right.get(), env),isConstant);
		}

		Value Interpreter::handleModulusOperation(Value left, Value right)
		{
			if (!Error::Manager::isSafe()) return Value();
			if (left.type == Type::INT
				&& right.type == Type::INT
				&& std::stoi(right.value) != 0) {
				return Value(Type::INT,
					std::to_string(std::stoi(left.value) % std::stoi(right.value)),
					left.startPosition,
					right.endPosition);
			}
			else {
				return Value(Type::UNDEFINED, "undefined", left.startPosition, right.endPosition);
			}
		}


		Value Interpreter::evaluateProgram(Frontend::Program program, Environment& env)
		{
			if (!Error::Manager::isSafe()) return Value();
			Value lastEvaluated = Value();
			for (Frontend::Node statement : program.body) {
				lastEvaluated = evaluate(statement, env);
			}

			return lastEvaluated;
		}

		template<typename T>
		inline void Coda::Runtime::Interpreter::handleArithmeticOperation(Value left, std::string functor, Value right, Value& result)
		{

			if (!Error::Manager::isSafe()) return;

			T typeLeft = getValue<T>(left.value);
			T typeRight = getValue<T>(right.value);

			if (functor == "+") {
				result.value = std::to_string(typeLeft + typeRight);
			}
			else if (functor == "-") {
				result.value = std::to_string(typeLeft - typeRight);
			}
			else if (functor == "*") {
				result.value = std::to_string(typeLeft * typeRight);
			}
			else if (functor == "/") {
				if (typeRight == 0) {
					Error::Runtime::raise("Division by Zero at, ", right.startPosition);
					return;
				}
				result.value = std::to_string(typeLeft / typeRight);
			}
			else {
				result.type = Type::NONE;
			}
		}

		template <typename T>
		T Interpreter::getValue(std::string str) {
			if (std::is_same_v<T, unsigned char>) {
				return static_cast<T>(str[0]);
			}
			else {
				return (T)std::stod(str);
			}
		}

	}
}

