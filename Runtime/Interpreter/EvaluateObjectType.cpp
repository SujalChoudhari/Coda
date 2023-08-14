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

		ValuePtr Interpreter::evaluateScopeExpression(const Frontend::Node& astNode, Environment& env) {
			Environment scope = Environment(&env);
			ValuePtr result = interpret(*astNode.right.get(), scope);
			env.addScope(astNode.left->value, std::make_shared<Environment>(scope));
			return result;
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
				Environment scope = Environment(&env);

				// create variables for each parameter
				if (std::get<2>(*functionContent).left->properties.size() != args.properties.size()) {
					Error::Runtime::raise("Function " + name->value + " expects " + std::to_string(std::get<2>(*functionContent).left->properties.size()) + " arguments, but " + std::to_string(args.properties.size()) + " were given.");
					IF_ERROR_RETURN_VALUE_PTR;
				}

				for (int i = 0; i < std::get<2>(*functionContent).left->properties.size(); i++) {
					auto& it = std::get<2>(*functionContent).left->properties[std::to_string(i)];
					const std::string& name = it->value;
					scope.declareFunctionParameter(name, args.properties[std::to_string(i + 1)]);
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
				res = left->properties[astNode.right->value];
			}
			return res;
		}


		ValuePtr Interpreter::evaluateBlockExpression(const Frontend::Node& astNode, Environment& env)
		{
			ValuePtr result;
			for (auto& it : astNode.properties) {
				result = interpret(*it.second.get(), env);
				IF_ERROR_RETURN_VALUE_PTR;
				if (result->type == Type::JUMP) {
					if (result->value == "return")
						return result->properties["returnable"];
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