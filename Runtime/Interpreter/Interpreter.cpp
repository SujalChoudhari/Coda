#include "Interpreter.h"
#include <string>
#include "../../Error/Error.h"
#include "../NativeFunctions/NativeFunction.h"
namespace Coda {
	namespace Runtime {
		Value Interpreter::evaluate(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE;

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
			else if (astNode.type == Frontend::NodeType::STRING_LITERAL) {
				value.type = Type::STRING;
				value.value = astNode.value;
			}
			else if (astNode.type == Frontend::NodeType::OBJECT_LITERAL) {
				return evaluateObjectExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::CALL_EXPRESSION) {
				return evaluateCallExpression(astNode, env);
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

			else if (astNode.type == Frontend::NodeType::ASSIGNMENT_EXPRESSION) {
				return evaluateAssignmentExpression(astNode, env);
			}



			else {
				Error::Runtime::raise("Unrecognised ASTNode '" + astNode.value + "'");
			}
			return value;
		}

		Value Interpreter::evaluateProgram(const Frontend::Program& program, Environment& env)
		{
			IF_ERROR_RETURN_VALUE;
			Value lastEvaluated = Value();
			for (Frontend::Node statement : program.body) {
				lastEvaluated = evaluate(statement, env);
			}

			return lastEvaluated;
		}


		Value Interpreter::evaluateBinaryExpression(const Frontend::Node& binop, Environment& env)
		{
			IF_ERROR_RETURN_VALUE;

			Value lhs = evaluate(*binop.left.get(), env);
			Value rhs = evaluate(*binop.right.get(), env);

			if ((lhs.type == Type::INT
				|| lhs.type == Type::BOOL
				|| lhs.type == Type::BYTE
				|| lhs.type == Type::LONG
				|| lhs.type == Type::FLOAT
				|| lhs.type == Type::DOUBLE)
				&& (rhs.type == Type::INT
					|| rhs.type == Type::BOOL
					|| rhs.type == Type::BYTE
					|| rhs.type == Type::LONG
					|| rhs.type == Type::FLOAT
					|| rhs.type == Type::DOUBLE))
			{
				return evaluateNumericBinaryExpression(lhs, binop.value, rhs);
			}
			else if (lhs.type == Type::STRING
				|| rhs.type == Type::STRING) {
				return evaluateStringBinaryExpression(lhs, binop.value, rhs);
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

		Value Interpreter::evaluateNumericBinaryExpression(const Value& left, const std::string& functor, const Value& right) {

			IF_ERROR_RETURN_VALUE;
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

		Value Interpreter::evaluateStringBinaryExpression(const Value& left, const std::string& functor, const Value& right)
		{
			IF_ERROR_RETURN_VALUE;

			if (functor == "+") {
				std::string concatenatedString = left.value + right.value;
				return Value(Type::STRING, concatenatedString);
			}

			Error::Runtime::raise("Unsupported operation with strings");
		}


		Value Interpreter::evaluateIdentifier(const Frontend::Node& astNode, Environment& env)
		{
			return env.lookupSymbol(astNode.value);
		}


		Value Interpreter::evaluateObjectExpression(const Frontend::Node& astNode, Environment& env)
		{

			IF_ERROR_RETURN_VALUE;

			Value object = Value();
			object.type = Type::OBJECT;
			object.value = "<object>";
			object.endPosition = astNode.endPosition;
			object.startPosition = astNode.startPosition;

			for (const auto& entry : astNode.properties) {
				const std::string& key = entry.first;
				const std::shared_ptr<Frontend::Node>& value = entry.second;

				Value runtimeValue;

				if (value.get()->type == Frontend::NodeType::PROPERTY) {
					runtimeValue = env.lookupSymbol(key);
				}
				else {
					runtimeValue = evaluate(*value.get(), env);
				}

				object.properties.emplace(key, std::make_shared<Value>(runtimeValue));
			}
			return object;
		}

		Value Interpreter::evaluateCallExpression(const Frontend::Node& callexp, Environment& env)
		{
			Value args = Value();
			Value name = evaluate(*callexp.left.get(), env);

			if (name.type != Type::NATIVE_FUNCTION) {
				Error::Runtime::raise("Calling a non function identifier");
				IF_ERROR_RETURN_VALUE;
			}
			unsigned int argCount = 1;
			for (auto& arg : callexp.properties) {
				args.properties.insert({ std::to_string(argCount), std::make_shared<Value>(evaluate(*arg.second.get(), env)) });
				argCount++;
			}

			return env.callFunction(name.value, args, env);

		}

		Value Interpreter::evaluateAssignmentExpression(const Frontend::Node& astNode, Environment& env)
		{
			if (astNode.type != Frontend::NodeType::ASSIGNMENT_EXPRESSION) {
				Error::Runtime::raise("Invalid Assignment Operation, at ");
				return Value();
			}

			return env.declareOrAssignVariable(astNode.left->value, evaluate(*astNode.right.get(), env));
		}


		Value Interpreter::evaluateDeclaration(const Frontend::Node& astNode, Environment& env, bool isConstant)
		{
			return env.declareOrAssignVariable(astNode.left->value, evaluate(*astNode.right.get(), env), isConstant);
		}

		template<typename T>
		inline void Coda::Runtime::Interpreter::handleArithmeticOperation(const Value& left, const std::string& functor, const Value& right, Value& result)
		{

			IF_ERROR_RETURN();
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

		Value Interpreter::handleModulusOperation(const Value& left, const Value& right)
		{
			IF_ERROR_RETURN_VALUE;
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

	}
}

