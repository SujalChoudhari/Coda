#include "Interpreter.h"

namespace Coda {
	namespace Runtime {

		ValuePtr Interpreter::evaluateObjectExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			ValuePtr object = std::make_shared<Value>(Type::OBJECT);
			object->value = "<object>";
			object->endPosition = astNode.endPosition;
			object->startPosition = astNode.startPosition;

			for (const auto& entry : astNode.properties) {
				const std::string& key = entry.first;
				const std::shared_ptr<Frontend::Node>& value = std::dynamic_pointer_cast<Node>(entry.second);

				ValuePtr runtimeValue;

				if (value.get()->type == Frontend::NodeType::PROPERTY) {
					runtimeValue = IVALUE_TO_VALUE(env.lookupSymbol(key));
				}
				else {
					runtimeValue = interpret(*value.get(), env);
				}

				object->properties.emplace(key, std::make_shared<Value>(runtimeValue));
			}
			return object;
		}

		ValuePtr Interpreter::evaluateScopeExpression(const Frontend::Node& astNode, Environment& env) {
			IF_ERROR_RETURN_VALUE_PTR;
			Environment scope = Environment(&env);
			ValuePtr result = interpret(*astNode.right.get(), scope);
			env.addScope(astNode.left->value, std::make_shared<Environment>(scope));
			return result;
		}

		ValuePtr Interpreter::evaluateListExpression(const Frontend::Node& list, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			Value listValue = Value();
			listValue.value = "<list>";
			listValue.type = Type::LIST;

			for (auto& it : list.properties) {
				listValue.properties.insert({ std::to_string(listValue.properties.size()), interpret(*std::dynamic_pointer_cast<Node>(it.second).get(), env) });
			}

			env.declareOrAssignVariable("name", std::make_shared<Value>(listValue), false);

			return std::make_shared<Value>(listValue);
		}

		ValuePtr Interpreter::evaluateCallExpression(const Frontend::Node& callExpression, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			Value args = Value();
			ValuePtr name = interpret(*callExpression.left.get(), env);

			unsigned int argCount = 1;
			for (auto& arg : callExpression.properties) {
				ValuePtr value = interpret(*std::dynamic_pointer_cast<Node>(arg.second).get(), env);
				IF_ERROR_RETURN_VALUE_PTR;
				args.properties.insert({ std::to_string(argCount), value });
				argCount++;
			}

			if (name->type == Type::NATIVE_FUNCTION) {
				return IVALUE_TO_VALUE(env.callFunction(name->value, std::make_shared<Value>(args), env));
			}
			else if (name->type == Type::FUNCTION) {
				ValuePtr function = IVALUE_TO_VALUE(env.lookupSymbol(name->value));
				Environment::UserDefinedFunction* functionContent;
				if (function->type == Type::OBJECT) {
					functionContent = env.getFunction(name->value);
				}
				else {
					functionContent = env.getFunction(function->value);
				}
				Environment scope = Environment(&env);

				// create variables for each parameter
				if (std::get<2>(*functionContent).left->properties.size() != args.properties.size()) {
					Error::Runtime::raise("Function " + name->value + " expects " + std::to_string(std::get<2>(*functionContent).left->properties.size()) + " arguments, but " + std::to_string(args.properties.size()) + " were given.");
					IF_ERROR_RETURN_VALUE_PTR;
				}

				for (int i = 0; i < std::get<2>(*functionContent).left->properties.size(); i++) {
					auto& it = std::get<2>(*functionContent).left->properties[std::to_string(i)];
					const std::string& name = it->getValue();
					scope.declareFunctionParameter(name, std::dynamic_pointer_cast<Value>(args.properties[std::to_string(i + 1)]));
				}

				// Run the function
				ValuePtr result = evaluateBlockExpression(*(std::get<2>(*functionContent).right).get(), scope);

				return result;
			}
			else {
				Error::Runtime::raise("Calling a non function identifier");
				return nullptr;
			}
		}

		ValuePtr Interpreter::evaluateNativeCallExpression(const Frontend::Node& callExpression, Environment& env)
		{
			
			std::string dllFilename = callExpression.left->value;
			std::string functionName = callExpression.right->left->value;

			Value args = Value();
			unsigned int argCount = 1;
			for (auto& arg : callExpression.right->properties) {
				ValuePtr value = interpret(*std::dynamic_pointer_cast<Node>(arg.second).get(), env);
				IF_ERROR_RETURN_VALUE_PTR;
				args.properties.insert({ std::to_string(argCount), value });
				argCount++;
			}

			/*
				TODO: 
				1. Load dll file with name `dllFilename`.
				2. Get function pointer to the `functionName` function
				3. Call function inside try-catch with args same as `/Runtime/NativeFunctions`.
					using IValue and IEnvironment Interfaces inside FFI
				4. Return the generated response.
			*/
			
			return ValuePtr();
		}

			
		ValuePtr Interpreter::evaluateMemberExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			ValuePtr res;
			ValuePtr left = interpret(*astNode.left.get(), env);
			if (left->type == Type::SCOPE) {
				Environment& scope = *env.getScope(left->value).get();
				res = interpret(*astNode.right.get(), scope);
			}
			else if (left != nullptr) {
				auto it = left->properties.find(astNode.right->value);
				if (it != left->properties.end()) { // if it is in the object return it
					res = std::dynamic_pointer_cast<Value>(left->properties[astNode.right->value]);
				}
				else { // its not in object, check if env has such variable
					ValuePtr location = interpret(*astNode.right.get(), env);
					res = std::dynamic_pointer_cast<Value>(left->properties[location->value]);
				}
			}
			return res;
		}


		ValuePtr Interpreter::evaluateBlockExpression(const Frontend::Node& astNode, Environment& env)
		{
			ValuePtr result;
			for (auto& it : astNode.properties) {
				result = interpret(*std::dynamic_pointer_cast<Node>(it.second).get(), env);
				IF_ERROR_RETURN_VALUE_PTR;
				if (result->type == Type::JUMP) {
					if (result->value == "return")
						return std::dynamic_pointer_cast<Value>(result->properties["returnable"]);
					else
						return result;
				}
			}
			return std::make_shared<Value>(Type::NONE, "");
		}

		ValuePtr Interpreter::evaluateFunctionDeclaration(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			return env.addFunction(astNode.value, astNode, env);
		}
	} // namespace Runtime
} // namespace Coda