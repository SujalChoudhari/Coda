#include "Interpreter.h"

namespace Coda {
	namespace Runtime {

		ValuePtr Interpreter::evaluateBinaryExpression(const Frontend::Node& binop, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			ValuePtr lhs = interpret(*binop.left.get(), env);
			ValuePtr rhs = interpret(*binop.right.get(), env);


			if (isLogical(binop.value)) {
				return handleLogicalOperation(lhs, binop.value, rhs);
			}
			else if (binop.value == "in") {
				Value val = Value(Type::BOOL, "0");
				if (rhs->type == Type::LIST || rhs->type == Type::OBJECT) {

					for (const auto& kv : rhs->properties) {
						if (kv.second->getValue() == lhs->value) {
							val.value = "1";
							break;
						}
					}
				}
				else {
					auto it = rhs->value.find(lhs->value);
					val.value = it == std::string::npos ? "0" : "1";
				}
				return std::make_shared<Value>(val);
			}
			else if (isNumericType(lhs->type) && isNumericType(rhs->type)) {
				return evaluateNumericBinaryExpression(lhs, binop.value, rhs);
			}
			else if (isStringType(lhs->type) || isStringType(rhs->type)) {
				return evaluateStringBinaryExpression(lhs, binop.value, rhs);
			}
			else if (isUndefinedType(lhs->type) || isUndefinedType(rhs->type)) {
				return std::make_shared<Value>(Type::UNDEFINED, "undefined", lhs->startPosition, rhs->endPosition);
			}
			else if (lhs->type != Type::NONE && rhs->type == Type::NONE) {
				return lhs;
			}
			else if (lhs->type == Type::NONE && rhs->type != Type::NONE) {
				return rhs;
			}

			return nullptr;
		}

		ValuePtr Interpreter::evaluateNumericBinaryExpression(const ValuePtr& left, const std::string& functor, const ValuePtr& right)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			if (functor == "%") {
				return handleModulusOperation(left, right);
			}
			else if (isRelational(functor)) {
				return handleNumericRelationalOperation(left, functor, right);
			}

			Type suggestedType = left->type;
			if (right->type > left->type) {
				suggestedType = right->type;
			}

			ValuePtr result = std::make_shared<Value>(suggestedType, left->startPosition, right->endPosition);

			if (suggestedType == Type::BOOL) {
				handleArithmeticOperation<bool>(left, functor, right, result);
			}
			else if (suggestedType == Type::CHAR) {
				handleArithmeticOperation<unsigned char>(left, functor, right, result);
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
				result->type = Type::NONE;
			}

			return result;
		}

		ValuePtr Interpreter::evaluateStringBinaryExpression(const ValuePtr& left, const std::string& functor, const ValuePtr& right)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			if (functor == "+") {
				std::string concatenatedString = left->value + right->value;
				return std::make_shared<Value>(Type::STRING, concatenatedString);
			}if (isRelational(functor)) {
				return handleStringRelationalOperation(left, functor, right);
			}

			Error::Runtime::raise("Unsupported operation with strings");
			return nullptr;
		}

		template<typename T>
		inline void Interpreter::handleArithmeticOperation(const ValuePtr& left, const std::string& functor, const ValuePtr& right, ValuePtr& result)
		{
			if (!Error::Manager::isSafe())
				return;

			T typeLeft = getValue<T>(left->value);
			T typeRight = getValue<T>(right->value);

			if (functor == "+") {
				result->value = std::to_string(typeLeft + typeRight);
			}
			else if (functor == "-") {
				result->value = std::to_string(typeLeft - typeRight);
			}
			else if (functor == "*") {
				result->value = std::to_string(typeLeft * typeRight);
			}
			else if (functor == "/") {
				if (typeRight == 0) {
					Error::Runtime::raise("Division by Zero at, ", Interpreter::callStack, right->startPosition, right->endPosition);
					return;
				}
				result->value = std::to_string(typeLeft / typeRight);
			}
			else {
				result->type = Type::NONE;
			}
		}

		template <typename T>
		T Interpreter::getValue(const std::string& str)
		{
			if (std::is_same_v<T, unsigned char>) {
				return static_cast<T>(str[0]);
			}
			else {
				return (T)std::stod(str);
			}
		}

	} // namespace Frontend
} // namespace Coda