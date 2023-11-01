#include "Interpreter.h"

namespace Coda {
	namespace Runtime {
		ValuePtr Interpreter::evaluateIfExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			Environment ifEnviron = Environment(&env);
			ValuePtr ifCondition = interpret(*astNode.left.get(), ifEnviron);

			if (Value::isTruthy(*(std::shared_ptr<IValue>)ifCondition)) {
				return interpret(*astNode.right.get(), ifEnviron);
			}

			for (auto it = astNode.properties.begin(); it != astNode.properties.end(); it++) {
				if (it->second != nullptr) {
					std::shared_ptr<Node> elifExpression = std::dynamic_pointer_cast<Node>(it->second);
					if (elifExpression->left == nullptr) {
						return interpret(*elifExpression->right.get(), ifEnviron);
					}

					ValuePtr elifCondition = interpret(*elifExpression->left.get(), ifEnviron);
					if (Value::isTruthy(*(std::shared_ptr<IValue>)elifCondition)) {
						return interpret(*elifExpression->right.get(), ifEnviron);
					}
				}
				else {
					return interpret(*astNode.right.get(), ifEnviron);
				}
			}

			// No else statement
			return std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);
		}

		ValuePtr Interpreter::evaluateForExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			Environment forEnv(&env);
			ValuePtr init = interpret(*astNode.left.get(), forEnv);
			env.declareOrAssignVariable(astNode.left->value, init, false);
			Frontend::Node condition = *astNode.right.get();
			Frontend::Node body = *(std::dynamic_pointer_cast<Node>(astNode.properties.at("body"))).get();
			Frontend::Node increment = *(std::dynamic_pointer_cast<Node>(astNode.properties.at("increment"))).get();
			ValuePtr result = std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);

			IF_ERROR_RETURN_VALUE_PTR;

			while (1) {
				ValuePtr conditionValue = interpret(condition, forEnv);
				if (!Value::isTruthy(*(std::shared_ptr<IValue>)conditionValue)) {
					break;
				}
				result = interpret(body, forEnv);
				IF_ERROR_RETURN_VALUE_PTR;

				if (result->type == Type::JUMP) {
					if (result->value == "break")
						break;
					else if (result->value == "return")
						return std::dynamic_pointer_cast<Value>(result->properties.at("returnable"));
				}

				interpret(increment, forEnv);
			}

			return result;
		}

		ValuePtr Interpreter::evaluateForInExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			Environment forEnv(&env);
			ValuePtr iterator = interpret(*astNode.left.get(), forEnv); // the value which changes
			env.declareOrAssignVariable(astNode.left->value, iterator, false);
			ValuePtr iterable = interpret(*astNode.right.get(), forEnv); // the value which can work with for-in
			Frontend::Node body = *(std::dynamic_pointer_cast<Node>(astNode.properties.at("body"))).get();
			ValuePtr result = std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);

			IF_ERROR_RETURN_VALUE_PTR;
			if (iterable->getType() == Type::LIST) {

				for (unsigned int i = 0; i < iterable->getProperties().size(); i++) {
					auto& newValue = iterable->getProperties()[std::to_string(i)];
					iterator->value = newValue->getValue();
					iterator->type = newValue->getType();
					iterator->properties = newValue->getProperties();
					iterator->startPosition = newValue->getStartPosition();
					iterator->endPosition = newValue->getEndPosition();

					result = interpret(body, forEnv);
					IF_ERROR_RETURN_VALUE_PTR;

					if (result->type == Type::JUMP) {
						if (result->value == "break")
							break;
						else if (result->value == "return")
							return std::dynamic_pointer_cast<Value>(result->properties.at("returnable"));
					}
				}

			}
			else if (iterable->getType() == Type::STRING) {
				for (unsigned int i = 0; i < iterable->getValue().size(); i++) {
					char newValue = iterable->getValue()[i];
					iterator->value = std::string(1, newValue);
					iterator->type = Type::CHAR;

					result = interpret(body, forEnv);
					IF_ERROR_RETURN_VALUE_PTR;

					if (result->type == Type::JUMP) {
						if (result->value == "break")
							break;
						else if (result->value == "return")
							return std::dynamic_pointer_cast<Value>(result->properties.at("returnable"));
					}
				}
			}
			else {
				Error::Runtime::raise("Cannot iterate over a non iterable value '" + iterable->getValue() + "'", Interpreter::callStack, astNode.startPosition, astNode.endPosition);
			}
			return result;
		}

		ValuePtr Interpreter::evaluateWhileExpression(const Frontend::Node& astNode, Environment& env)
		{
			Environment whileEnv(&env);
			Frontend::Node condition = *astNode.left.get();
			Frontend::Node body = *astNode.right.get();
			ValuePtr result = std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);
			IF_ERROR_RETURN_VALUE_PTR;

			while (1) {
				ValuePtr conditionValue = interpret(condition, whileEnv);
				if (!Value::isTruthy(*(std::shared_ptr<IValue>)conditionValue)) {
					break;
				}

				result = interpret(body, whileEnv);
				IF_ERROR_RETURN_VALUE_PTR;

				if (result->type == Type::JUMP) {
					if (result->value == "break")
						break;
					else if (result->value == "continue")
						continue;
					else if (result->value == "return")
						return std::dynamic_pointer_cast<Value>(result->properties.at("returnable"));
				}
			}

			return result;
		}

		ValuePtr Interpreter::evaluateDoWhileExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			Environment whileEnv(&env);
			Frontend::Node condition = *astNode.left.get();
			Frontend::Node body = *astNode.right.get();
			ValuePtr result = std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);
			IF_ERROR_RETURN_VALUE_PTR;

			while (1) {
				result = interpret(body, whileEnv);

				ValuePtr conditionValue = interpret(condition, whileEnv);
				if (!Value::isTruthy(*(std::shared_ptr<IValue>)conditionValue)) {
					break;
				}

				if (result->type == Type::JUMP) {
					if (result->value == "break")
						break;
					else if (result->value == "continue")
						continue;
					else if (result->value == "return")
						return std::dynamic_pointer_cast<Value>(result->properties.at("returnable"));
				}
			}

			return result;
		}

		ValuePtr Interpreter::evaluateJumpExpression(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			if (astNode.value == "break") {
				return std::make_shared<Value>(Type::JUMP, "break", astNode.startPosition, astNode.endPosition);
			}
			else if (astNode.value == "continue") {
				return std::make_shared<Value>(Type::JUMP, "continue", astNode.startPosition, astNode.endPosition);
			}
			else if (astNode.value == "return") {
				Value value = Value(Type::JUMP);
				value.value = "return";
				value.startPosition = astNode.startPosition;
				value.endPosition = astNode.endPosition;
				if (astNode.left != nullptr) {
					value.properties.insert({ "returnable",interpret(*astNode.left.get(), env) });
				}
				else {
					value.properties.insert({ "returnable",std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition) });
				}
				return std::make_shared<Value>(value);
			}
			else {
				Error::Runtime::raise("Not a valid jump statement, at: ", Interpreter::callStack, astNode.startPosition, astNode.endPosition);
				return std::make_shared<Value>(Type::NONE, "None", astNode.startPosition, astNode.endPosition);
			}
		}


	} // namespace Runtime
} // namespace Coda