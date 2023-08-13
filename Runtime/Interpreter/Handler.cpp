#include "Interpreter.h"

namespace Coda {
	namespace Runtime {

	
		ValuePtr Interpreter::handleModulusOperation(const ValuePtr& left, const ValuePtr& right)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			if (left->type == Type::INT
				&& right->type == Type::INT
				&& std::stoi(right->value) != 0) {
				return std::make_shared<Value>(Type::INT,
					std::to_string(std::stoi(left->value) % std::stoi(right->value)),
					left->startPosition,
					right->endPosition);
			}
			else {
				return std::make_shared<Value>(Type::UNDEFINED, "undefined", left->startPosition, right->endPosition);
			}
		}

		ValuePtr Interpreter::handleNumericRelationalOperation(const ValuePtr& left, const std::string& functor, const ValuePtr& right)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			bool result = false;

			if (functor == "==" && left->value == right->value)
				result = true;
			else if (functor == "!=" && left->value != right->value)
				result = true;
			else if (functor == ">=" && std::stod(left->value) >= std::stod(right->value))
				result = true;
			else if (functor == "<=" && std::stod(left->value) <= std::stod(right->value))
				result = true;
			else if (functor == ">" && std::stod(left->value) > std::stod(right->value))
				result = true;
			else if (functor == "<" && std::stod(left->value) < std::stod(right->value))
				result = true;

			return std::make_shared<Value>(Type::BOOL, std::to_string(result), left->startPosition, right->endPosition);
		}

		ValuePtr Interpreter::handleStringRelationalOperation(const ValuePtr& left, const std::string& functor, const ValuePtr& right)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			bool result = false;

			if (functor == "==" && left->value == right->value)
				result = true;
			else if (functor == "!=" && left->value != right->value)
				result = true;
			else if (functor == ">=" && (left->value).length() >= (right->value).length())
				result = true;
			else if (functor == "<=" && (left->value).length() <= (right->value).length())
				result = true;
			else if (functor == ">" && (left->value).length() > (right->value).length())
				result = true;
			else if (functor == "<" && (left->value).length() < (right->value).length())
				result = true;

			return std::make_shared<Value>(Type::BOOL, std::to_string(result), left->startPosition, right->endPosition);
		}

		ValuePtr Interpreter::handleLogicalOperation(const ValuePtr& left, const std::string& functor, const ValuePtr& right)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			bool result = false;
			if (functor == "||"
				&& (Value::isTruthy(*left.get()) || Value::isTruthy(*right.get()))) {
				result = true;
			}
			else if (functor == "&&"
				&& (Value::isTruthy(*left.get()) && Value::isTruthy(*right.get()))) {
				result = true;
			}

			return std::make_shared<Value>(Type::BOOL, std::to_string(result), left->startPosition, right->endPosition);

		}

		bool Interpreter::isNumericType(Type type)
		{
			return type == Type::INT || type == Type::BOOL || type == Type::BYTE ||
				type == Type::LONG || type == Type::FLOAT || type == Type::DOUBLE || type == Type::CHAR;
		}

		bool Interpreter::isStringType(Type type)
		{
			return type == Type::STRING;
		}

		bool Interpreter::isUndefinedType(Type type)
		{
			return type == Type::UNDEFINED;
		}

		bool Interpreter::isRelational(std::string type)
		{
			return (type == "==" || type == "!=" || type == ">=" || type == "<=" || type == "<" || type == ">");
		}

		bool Interpreter::isLogical(std::string type)
		{
			return (type == "&&" || type == "||");
		}




	} // namespace Runtime
} // namespace Coda