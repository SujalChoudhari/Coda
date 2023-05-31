#include "Environment.h"
#include <vector>
#include <algorithm>
#include "../../Error/Error.h"
#include "../RuntimeValue/Value.h"
#include "../NativeFunctions/NativeFunction.h"


namespace Coda {
	namespace Runtime {

		Environment::Environment()
		{
			parent = nullptr;
		}
		Environment::Environment(Environment* parentEnvironment)
		{
			this->parent = parentEnvironment;
		}

		Environment Environment::root()
		{
			Environment env = Environment();

			env.declareOrAssignVariable("none", std::make_shared<Value>(Type::NONE, "none"), true);
			env.declareOrAssignVariable("undefined", std::make_shared<Value>(Type::UNDEFINED, "undefined"), true);
			env.declareOrAssignVariable("true", std::make_shared<Value>(Type::BOOL, "1"), true);
			env.declareOrAssignVariable("false", std::make_shared<Value>(Type::BOOL, "0"), true);

			// native-fn
			env.declareNativeFunction("print", Native::print);
			env.declareNativeFunction("println", Native::println);
			env.declareNativeFunction("input", Native::input);
			env.declareNativeFunction("sleep", Native::sleep);

			env.declareNativeFunction("parseInt", Native::parseInt);
			env.declareNativeFunction("parseFloat", Native::parseFloat);
			env.declareNativeFunction("parseDouble", Native::parseDouble);
			env.declareNativeFunction("parseByte", Native::parseByte);
			env.declareNativeFunction("parseBool", Native::parseBool);

			return env;
		}


		ValuePtr Environment::declareOrAssignVariable(const std::string& name, const ValuePtr& value, bool isConstant)
		{
			auto symbolIt = symbols.find(name);
			if (symbolIt != symbols.end()) {
				// Variable already exists, assign to it
				if (isConstant) {
					Error::Runtime::raise("Reassignment of constant is not allowed, at ", value->endPosition);
					return nullptr;
				}
				else if (constants.find(name) != constants.end()) {
					// Trying to assign to a constant variable
					Error::Runtime::raise("Assignment to constant variable is not allowed, at ", value->endPosition);
					return nullptr;
				}
				symbolIt->second = value;
			}
			else {
				// Variable does not exist, create it
				if (isConstant) {
					constants.insert(name);
				}
				symbols.emplace(name, value);
			}
			return value;
		}

		ValuePtr Environment::declareOrAssignVariable(const Frontend::Node& name, const ValuePtr& value, bool isConstant)
		{
			if (name.type != Frontend::NodeType::MEMBER_EXPRESSION) {
				Error::Runtime::raise("Invalid variable name");
				IF_ERROR_RETURN_VALUE_PTR;
			}

			const Frontend::Node* current = &name;
			std::vector<std::string> identifierChain;

			// Navigate inwards through MemberExpression nodes
			while (current->type == Frontend::NodeType::MEMBER_EXPRESSION) {
				const Frontend::Node& left = *current->left;
				const Frontend::Node& right = *current->right;

				identifierChain.push_back(right.value);
				current = &left;
			}

			std::reverse(identifierChain.begin(), identifierChain.end());
			ValuePtr variableValue;
			variableValue = symbols[current->value];

			for (int i = 0; i < identifierChain.size() - 1; i++) {
				variableValue = variableValue->properties[identifierChain[i]];
			}

			variableValue->properties[identifierChain.back()] = std::make_shared<Value>(value);
			return value;
		}

		ValuePtr Environment::declareNativeFunction(const std::string& name, Function function)
		{
			declareOrAssignVariable(name, std::make_shared<Value>(Type::NATIVE_FUNCTION, name), true);
			functions.emplace(name, function);
			return nullptr;
		}

		ValuePtr Environment::declareUserDefinedFunction(const std::string& name, Frontend::Node astNode)
		{
			ValuePtr function = std::make_shared<Value>(Type::FUNCTION, astNode.value);
			this->declareOrAssignVariable(name, function, false);
			return function;
		}

		ValuePtr Environment::callFunction(const std::string& name, const ValuePtr& args, Environment& env) {
			auto it = functions.find(name);
			if (it != functions.end()) {
				return it->second(args, env);
			}
			else {
				Error::Runtime::raise("Function " + name + " does not exist.");
				return nullptr;
			}
		}

		ValuePtr Environment::lookupSymbol(std::string name)
		{
			Environment* env = resolve(name);
			if (env != nullptr)
				return env->symbols[name];
			else
				Error::Runtime::raise("Symbol '" + name + "' does not exist");
			return nullptr;

		}

		Environment* Environment::resolve(std::string name)
		{
			auto it = symbols.find(name);
			if (it != symbols.end()) {
				return this;
			}

			if (parent == nullptr) {
				return nullptr;
			}

			return parent->resolve(name);
		}
	}
}
