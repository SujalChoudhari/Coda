#include "Environment.h"
#include "../../Error/Error.h"
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

			env.declareOrAssignVariable("none", Value(Type::NONE, "none"), true);
			env.declareOrAssignVariable("undefined", Value(Type::UNDEFINED, "undefined"), true);
			env.declareOrAssignVariable("true", Value(Type::BOOL, "1"), true);
			env.declareOrAssignVariable("false", Value(Type::BOOL, "0"), true);

			env.declareOrAssignVariable("print", Value(Type::NATIVE_FUNCTION, "<native-fn>"), true);

			return env;
		}


		Value Environment::declareOrAssignVariable(const std::string& name, const Value& value, bool isConstant)
		{
			auto symbolIt = symbols.find(name);
			if (symbolIt != symbols.end()) {
				// Variable already exists, assign to it
				if (isConstant) {
					Error::Runtime::raise("Reassignment of constant is not allowed, at ", value.endPosition);
					return value;
				}
				else if (constants.find(name) != constants.end()) {
					// Trying to assign to a constant variable
					Error::Runtime::raise("Assignment to constant variable is not allowed, at ", value.endPosition);
					return value;
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


		Value Environment::lookupSymbol(std::string name)
		{
			Environment* env = resolve(name);
			if (env != nullptr)
				return env->symbols[name];
			else
				Error::Runtime::raise("Symbol '" + name + "' does not exist");
			return Value();

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
