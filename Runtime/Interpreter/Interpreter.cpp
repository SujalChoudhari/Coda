#include <string>

#include "Interpreter.h"
#include "../Environment/Environment.h"
#include "../RuntimeValue/Value.h"
#include "../../Error/Error.h"
#include "../NativeFunctions/NativeFunction.h"
namespace Coda {
	namespace Runtime {
		ValuePtr Interpreter::interpret(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;

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
			else if (astNode.type == Frontend::NodeType::CHARACTER_LITERAL) {
				value.type = Type::CHAR;
				value.value = astNode.value;
			}
			else if (astNode.type == Frontend::NodeType::OBJECT_LITERAL) {
				return evaluateObjectExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::LIST_LITERAL) {
				return evaluateListExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::CALL_EXPRESSION) {
				return evaluateCallExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::BLOCK_STATEMENT) {
				return evaluateBlockExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::UNARY_EXPRESSION) {
				return evaluateUnaryExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::BINARY_EXPRESSION) {
				return evaluateBinaryExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::MEMBER_EXPRESSION) {
				return evaluateMemberExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::VARIABLE_DECLARATION) {
				return evaluateVariableDeclaration(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::FUNCTION_LITERAL) {
				return evaluateFunctionDeclaration(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::CONSTANT_DECLARATION) {
				return evaluateVariableDeclaration(astNode, env, true);
			}
			else if (astNode.type == Frontend::NodeType::ASSIGNMENT_EXPRESSION) {
				return evaluateAssignmentExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::IF_EXPRESSION) {
				return evaluateIfExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::FOR_EXPRESSION) {
				return evaluateForExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::WHILE_EXPRESSION) {
				return evaluateWhileExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::DO_WHILE_EXPRESSION) {
				return evaluateDoWhileExpression(astNode, env);
			}
			else {
				Error::Runtime::raise("Unrecognized ASTNode '" + astNode.value + "'");
			}
			return std::make_shared<Value>(value);
		}

		ValuePtr Interpreter::evaluateProgram(const Frontend::Program& program, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			std::shared_ptr<Value> lastEvaluated = nullptr;
			for (Frontend::Node statement : program.body) {
				lastEvaluated = interpret(statement, env);
			}

			return lastEvaluated;
		}

		ValuePtr Interpreter::evaluateUnaryExpression(const Frontend::Node& op, Environment& env) {
			std::string unaryOperator = op.value;
			ValuePtr value = interpret(*op.left.get(), env);
			if (unaryOperator == "-") {
				value->value = ((value->value[0] == '-') ? value->value.substr(1) : "-" + value->value);
				value->value.erase(value->value.find_last_not_of('0') + 1, std::string::npos);
				value->value.erase(value->value.find_last_not_of('.') + 1, std::string::npos);
				return value;
			}
			else if (unaryOperator == "+") {
				return std::make_shared<Value>(value);
			}
			else if (unaryOperator == "!") {
				Value booleanValue = Value(Type::BOOL, "false", op.startPosition, op.endPosition);
				if (!Value::isTruthy(value))
					booleanValue.value = "true";
				return std::make_shared<Value>(booleanValue);
			}
			else if (unaryOperator == "++") {
				double num = std::stod(value->value);
				num++;
				value->value = std::to_string(num);
				value->value.erase(value->value.find_last_not_of('0') + 1, std::string::npos);
				value->value.erase(value->value.find_last_not_of('.') + 1, std::string::npos);

				return value;
			}
			else if (unaryOperator == "--") {
				double num = std::stod(value->value);
				num--;
				value->value = std::to_string(num);
				value->value.erase(value->value.find_last_not_of('0') + 1, std::string::npos);
				value->value.erase(value->value.find_last_not_of('.') + 1, std::string::npos);
				return value;
			}
			else if (unaryOperator == "typeof") {
				return std::make_shared<Value>(Type::STRING, Value::getTypeAsString(value->type), value->startPosition, value->endPosition);
			}
			else if (unaryOperator == "sizeof") {
				return std::make_shared<Value>(Type::INT, std::to_string(value->value.size()), value->startPosition, value->endPosition);
			}
			else if (unaryOperator == "delete") {
				env.remove(value->value);
				return std::make_shared<Value>(Type::UNDEFINED);
			}
			else {
				Error::Runtime::raise("Unrecognized unary operator '" + unaryOperator + "'");
			}

		}


		ValuePtr Interpreter::evaluateBinaryExpression(const Frontend::Node& binop, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			ValuePtr lhs = interpret(*binop.left.get(), env);
			ValuePtr rhs = interpret(*binop.right.get(), env);


			if (isLogical(binop.value)) {
				return handleLogicalOperation(lhs, binop.value, rhs);
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


		ValuePtr Interpreter::evaluateIdentifier(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			return env.lookupSymbol(astNode.value);
		}


		ValuePtr Interpreter::evaluateObjectExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			ValuePtr object = std::make_shared<Value>(Type::OBJECT);
			object->value = "<object>";
			object->endPosition = astNode.endPosition;
			object->startPosition = astNode.startPosition;

			for (const auto& entry : astNode.properties) {
				const std::string& key = entry.first;
				const std::shared_ptr<Frontend::Node>& value = entry.second;

				ValuePtr runtimeValue;

				if (value.get()->type == Frontend::NodeType::PROPERTY) {
					runtimeValue = env.lookupSymbol(key);
				}
				else {
					runtimeValue = interpret(*value.get(), env);
				}

				object->properties.emplace(key, std::make_shared<Value>(runtimeValue));
			}
			return object;
		}

		ValuePtr Interpreter::evaluateListExpression(const Frontend::Node& list, Environment& env)
		{
			Value listValue = Value();
			listValue.value = "<list>";
			listValue.type = Type::LIST;

			for (auto& it : list.properties) {
				listValue.properties.insert({ std::to_string(listValue.properties.size()), interpret(*it.second.get(), env) });
			}

			env.declareOrAssignVariable("name", std::make_shared<Value>(listValue), false);

			return std::make_shared<Value>(listValue);
		}

		ValuePtr Interpreter::evaluateCallExpression(const Frontend::Node& callexp, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			Value args = Value();
			ValuePtr name = interpret(*callexp.left.get(), env);

			unsigned int argCount = 1;
			for (auto& arg : callexp.properties) {
				ValuePtr value = interpret(*arg.second.get(), env);
				IF_ERROR_RETURN_VALUE_PTR;
				args.properties.insert({ std::to_string(argCount), value });
				argCount++;
			}

			if (name->type == Type::NATIVE_FUNCTION) {
				return env.callFunction(name->value, std::make_shared<Value>(args), env);
			}
			else if (name->type == Type::FUNCTION) {
				ValuePtr function = env.lookupSymbol(name->value);
				Environment::UserDefinedFunction* functionContent;
				if (function->type == Type::OBJECT) {
					functionContent = env.getFunction(name->value);
				}
				else {
					functionContent = env.getFunction(function->value);
				}
				Environment scope = Environment(std::get<1>(*functionContent));


				// create variables for each parameter
				if (std::get<2>(*functionContent).left->properties.size() != args.properties.size()) {
					Error::Runtime::raise("Function " + name->value + " expects " + std::to_string(std::get<2>(*functionContent).left->properties.size()) + " arguments, but " + std::to_string(args.properties.size()) + " were given.");
					IF_ERROR_RETURN_VALUE_PTR;
				}

				for (int i = 0; i < std::get<2>(*functionContent).left->properties.size(); i++) {
					auto& it = std::get<2>(*functionContent).left->properties[std::to_string(i)];
					const std::string& name = it->value;
					scope.declareOrAssignVariable(name, args.properties[std::to_string(i + 1)], true);
				}

				// Run the function
				ValuePtr result = nullptr;
				for (auto& it : std::get<2>(*functionContent).right->properties) {
					result = interpret(*it.second.get(), scope);
				}

				return result;
			}
			else {
				Error::Runtime::raise("Calling a non function identifier");
				return nullptr;
			}
		}

		ValuePtr Interpreter::performAssignmentOperation(ValuePtr left, const ValuePtr& interpreted, const std::function<double(double, double)>& operation)
		{
			if (isNumericType(left->type) && isNumericType(interpreted->type)) {
				double num = std::stod(left->value);
				double rightNum = std::stod(interpreted->value);
				num = operation(num, rightNum);
				left->value = std::to_string(num);
				left->type = (left->type == Type::DOUBLE) ? Type::DOUBLE : Type::FLOAT;
				left->value.erase(left->value.find_last_not_of('0') + 1, std::string::npos);
				left->value.erase(left->value.find_last_not_of('.') + 1, std::string::npos);
				return left;
			}
			else {
				Error::Runtime::raise("Cannot assign '" + interpreted->value + "' to '" + left->value + "' as both types must be numeric.");
				return nullptr;
			}
		}

		ValuePtr Interpreter::evaluateAssignmentExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			if (astNode.value == "=") {
				if (astNode.left->type == Frontend::NodeType::IDENTIFIER) {
					return env.declareOrAssignVariable(astNode.left->value, interpret(*astNode.right.get(), env));
				}
				else if (astNode.left->type == Frontend::NodeType::MEMBER_EXPRESSION) {
					return env.declareOrAssignVariable(*astNode.left.get(), interpret(*astNode.right.get(), env));
				}
				else {
					Error::Runtime::raise("Invalid Assignment Operation, at ", astNode.endPosition);
					return nullptr;
				}
			}
			else if (astNode.value == "+=") {
				ValuePtr left = env.lookupSymbol(astNode.left->value);
				ValuePtr interpreted = interpret(*astNode.right.get(), env);
				return performAssignmentOperation(left, interpreted, [](double a, double b) { return a + b; });
			}
			else if (astNode.value == "-=") {
				ValuePtr left = env.lookupSymbol(astNode.left->value);
				ValuePtr interpreted = interpret(*astNode.right.get(), env);
				return performAssignmentOperation(left, interpreted, [](double a, double b) { return a - b; });
			}
			else if (astNode.value == "*=") {
				ValuePtr left = env.lookupSymbol(astNode.left->value);
				ValuePtr interpreted = interpret(*astNode.right.get(), env);
				return performAssignmentOperation(left, interpreted, [](double a, double b) { return a * b; });
			}
			else if (astNode.value == "/=") {
				ValuePtr left = env.lookupSymbol(astNode.left->value);
				ValuePtr interpreted = interpret(*astNode.right.get(), env);
				return performAssignmentOperation(left, interpreted, [](double a, double b) { return a / b; });
			}
		}


		ValuePtr Interpreter::evaluateBlockExpression(const Frontend::Node& astNode, Environment& env)
		{
			ValuePtr result;
			for (auto& it : astNode.properties) {
				result = interpret(*it.second.get(), env);
			}
			return result;
		}

		ValuePtr Interpreter::evaluateIfExpression(const Frontend::Node& astNode, Environment& env)
		{
			ValuePtr ifCondition = interpret(*astNode.left.get(), env);

			if (Value::isTruthy(ifCondition)) {
				return interpret(*astNode.right.get(), env);
			}

			for (auto it = astNode.properties.begin(); it != astNode.properties.end(); it++) {
				if (it->second != nullptr) {
					Frontend::Node elifExpression = *it->second.get();

					ValuePtr elifCondition = interpret(*elifExpression.left.get(), env);
					if (Value::isTruthy(elifCondition)) {
						return interpret(*elifExpression.right.get(), env);
					}
				}
				else {
					return interpret(*astNode.right.get(), env);
				}
			}

			// No else statement
			return std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);
		}

		ValuePtr Interpreter::evaluateForExpression(const Frontend::Node& astNode, Environment& env)
		{
			Environment forEnv(&env);
			ValuePtr init = interpret(*astNode.left.get(), forEnv);
			env.declareOrAssignVariable(astNode.left->value, init, false);
			Frontend::Node condition = *astNode.right.get();
			Frontend::Node body = *astNode.properties.at("body").get();
			Frontend::Node increment = *astNode.properties.at("increment").get();
			ValuePtr result = std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);

			IF_ERROR_RETURN_VALUE_PTR;

			while (1) {
				ValuePtr conditionValue = interpret(condition, forEnv);
				if (!Value::isTruthy(conditionValue)) {
					break;
				}
				result = interpret(body, forEnv);
				interpret(increment, forEnv);
			}

			return result;
		}

		ValuePtr Interpreter::evaluateWhileExpression(const Frontend::Node& astNode, Environment& env)
		{
			Environment whileEnv(&env);
			Frontend::Node condition = *astNode.left.get();
			Frontend::Node body = *astNode.right.get();
			ValuePtr result = std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);

			while (1) {
				ValuePtr conditionValue = interpret(condition, whileEnv);
				if (!Value::isTruthy(conditionValue)) {
					break;
				}

				result = interpret(body, whileEnv);
			}

			return result;
		}

		ValuePtr Interpreter::evaluateDoWhileExpression(const Frontend::Node& astNode, Environment& env)
		{
			Environment whileEnv(&env);
			Frontend::Node condition = *astNode.left.get();
			Frontend::Node body = *astNode.right.get();
			ValuePtr result = std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);

			while (1) {
				result = interpret(body, whileEnv);

				ValuePtr conditionValue = interpret(condition, whileEnv);
				if (!Value::isTruthy(conditionValue)) {
					break;
				}
			}

			return result;
		}



		ValuePtr Interpreter::evaluateMemberExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			ValuePtr left = interpret(*astNode.left.get(), env);
			Value res = *left->properties[astNode.right->value].get();
			return std::make_shared<Value>(res);
		}


		ValuePtr Interpreter::evaluateVariableDeclaration(const Frontend::Node& astNode, Environment& env, bool isConstant)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			return env.declareOrAssignVariable(astNode.left->value, interpret(*astNode.right.get(), env), isConstant);
		}

		ValuePtr Interpreter::evaluateFunctionDeclaration(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			return env.addFunction(astNode.value, astNode, env);
		}

		template<typename T>
		inline void Coda::Runtime::Interpreter::handleArithmeticOperation(const ValuePtr& left, const std::string& functor, const ValuePtr& right, ValuePtr& result)
		{
			IF_ERROR_RETURN();
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
					Error::Runtime::raise("Division by Zero at, ", right->startPosition);
					return;
				}
				result->value = std::to_string(typeLeft / typeRight);
			}
			else {
				result->type = Type::NONE;
			}
		}

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

