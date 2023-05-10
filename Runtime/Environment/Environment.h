#pragma once
#include <map>
#include <set>
#include <string>
#include "../RuntimeValue/Value.h"

namespace Coda {
	namespace Runtime {
		class Environment {
		public:
			Environment();
			Environment(Environment* parentEnvironment);
			static Environment root();

			Value declareOrAssignVariable(const std::string& name, const Value& value, bool isConstant = false);
			Value lookupSymbol(std::string varname);

		private:
			Environment* parent;
			std::map<std::string, Value> symbols;
			std::set<std::string> constants;
			
		private:
			Environment* resolve(std::string name);
		};
	}
}