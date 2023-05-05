#include "Interpreter.h"
#include <string>
#include "../Error/Error.h"
namespace Coda {
	namespace Runtime {
		Value Interpreter::evaluate(FrontEnd::Node astNode)
		{
			if (!Error::Manager::isSafe()) return Value();

			Value value = Value(); // Defaults to NONE
			value.startPosition = astNode.startPosition;

			if (astNode.type == FrontEnd::NodeType::BYTE_LITERAL) {
				value.type = Type::BYTE;
				value.value = astNode.value;
			}

			else if (astNode.type == FrontEnd::NodeType::INTEGER_LITERAL) {
				value.type = Type::INT;
				value.value = astNode.value;
			}

			else if (astNode.type == FrontEnd::NodeType::LONG_INT_LITERAL) {
				value.type = Type::LONG;
				value.value = astNode.value;
			}

			else if (astNode.type == FrontEnd::NodeType::FLOATING_POINT_LITERAL) {
				value.type = Type::FLOAT;
				value.value = astNode.value;
			}

			else if (astNode.type == FrontEnd::NodeType::DOUBLE_LITERAL) {
				value.type = Type::DOUBLE;
				value.value = astNode.value;
			}

			else if (astNode.type == FrontEnd::NodeType::NONE_LITERAL) {
				value.type = Type::NONE;
				value.value = astNode.value;
			}

			else if (astNode.type == FrontEnd::NodeType::UNDEFINED_LITERAL) {
				value.type = Type::UNDEFINED;
				value.value = astNode.value;
			}

			else if (astNode.type == FrontEnd::NodeType::BINARY_EXPRESSION) {
				return evaluateBinaryExpression(astNode);
			}

			else {
				Error::Runtime::raiseUnrecognisedASTNodeError(astNode.value, astNode.startPosition);
			}

			return value;
		}

		Value Interpreter::evaluateBinaryExpression(FrontEnd::Node binop)
		{
			if (!Error::Manager::isSafe()) return Value();

			Value lhs = evaluate(*binop.left.get());
			Value rhs = evaluate(*binop.right.get());

			if (
				(lhs.type == Type::INT
					|| lhs.type == Type::BYTE
					|| lhs.type == Type::LONG
					|| lhs.type == Type::FLOAT
					|| lhs.type == Type::DOUBLE)
				&&
				(rhs.type == Type::INT
					|| rhs.type == Type::BYTE
					|| rhs.type == Type::LONG
					|| rhs.type == Type::FLOAT
					|| rhs.type == Type::DOUBLE))
			{
				return evaluateNumericBinaryExpression(lhs, binop.value, rhs);
			}
			else {
				return Value(Type::NONE, lhs.startPosition, rhs.endPosition);
			}
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

			if (suggestedType == Type::BYTE) {
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

		Value Interpreter::handleModulusOperation(Value left, Value right)
		{
			if (!Error::Manager::isSafe()) return Value();
			if (left.type == Type::INT && right.type == Type::INT) {
				return Value(Type::INT,
					std::to_string(std::stoi(left.value) % std::stoi(right.value)),
					left.startPosition,
					right.endPosition);
			}
			else {
				Error::Runtime::raiseTypeError("One or both operands of '%' operator are non-integer", left.startPosition);
			}
		}


		Value Interpreter::evaluateProgram(FrontEnd::Program program)
		{
			if (!Error::Manager::isSafe()) return Value();

			Value lastEvaluated = Value();

			for (FrontEnd::Node statement : program.body) {
				lastEvaluated = evaluate(statement);
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
					Error::Runtime::raiseDivisionByZeroError(right.startPosition);
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

