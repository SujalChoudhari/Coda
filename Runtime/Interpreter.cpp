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

			if (astNode.type == FrontEnd::NodeType::NUMERIC_LITERAL) {
				value.type = Type::NUMBER;
				value.value = astNode.value;
			}

			else if (astNode.type == FrontEnd::NodeType::NONE_LITERAL) {
				value.type = Type::NONE;
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

			if (lhs.type == Type::NUMBER && rhs.type == Type::NUMBER) {
				return evaluateNumericBinaryExpression(lhs, binop.value, rhs);
			}
			else {
				return Value(Type::NONE, lhs.startPosition,rhs.endPosition);
			}
		}

		Value Interpreter::evaluateNumericBinaryExpression(Value left, std::string functor, Value right)
		{
			if (!Error::Manager::isSafe()) return Value();
			Value result = Value(Type::NUMBER, left.startPosition,right.endPosition);
			if (functor == "+") {
				
				result.value = std::to_string(std::stoi(left.value) + std::stoi(right.value));
			}

			else if (functor == "-") {
				
				result.value = std::to_string(std::stoi(left.value) - std::stoi(right.value));
			}

			else if (functor == "*") {
				
				result.value = std::to_string(std::stoi(left.value) * std::stoi(right.value));
			}

			else if (functor == "/") {
				if (std::stoi(right.value) == 0)
				{
					Error::Runtime::raiseDivisionByZeroError(right.startPosition);
					return result;
				}

				result.value = std::to_string(std::stoi(left.value) / std::stoi(right.value));
			}

			else if (functor == "%") {
				
				result.value = std::to_string(std::stoi(left.value) % std::stoi(right.value));
			}
			return result;
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

	}
}

