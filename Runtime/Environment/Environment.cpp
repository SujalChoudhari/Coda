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


		Value Environment::declareOrAssignVariable(std::string name, Value value)
		{

			auto it = symbols.find(name);
			if (it != symbols.end()) {
				symbols.insert_or_assign(name, value);
			}
			else {
				Environment* env = resolve(name);

				if (!Error::Manager::isSafe()) return Value();

				if (env != nullptr)
					env->symbols.insert_or_assign(name, value);
				else {
					symbols.insert_or_assign(name, value);
				}
			}
			return value;
		}
		Value Environment::lookupSymbol(std::string name)
		{
			Environment* env = resolve(name);
			if (env != nullptr)
				return env->symbols[name];
			else
				Error::Runtime::raiseSymbolDoesnotExist(name);
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
