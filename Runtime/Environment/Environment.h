#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include <functional>
#include "../RuntimeValue/Value.h"
#include "../../Frontend/Node/Node.h"

namespace Coda {
	namespace Runtime {
		class Environment {
			typedef std::function<Value(Value value, Environment scope)> Function;
		public:
			Environment();
			Environment(Environment* parentEnvironment);
			static Environment root();

			Value declareOrAssignVariable(const std::string& name, const Value& value, bool isConstant = false);
			Value declareOrAssignVariable(const Frontend::Node& name, const Value& value, bool isConstant = false);
			Value declareNativeFunction(const std::string& name, Function function);
			Value declareUserDefinedFunction(const std::string& name, Frontend::Node astNode);
			Value callFunction(const std::string& name, const Value& args, Environment& env);
			Value lookupSymbol(std::string varname);

		private:
			Environment* parent;
			std::map<std::string, Value> symbols;
			std::map<std::string, Function> functions;
			std::set<std::string> constants;

		private:
			Environment* resolve(std::string name);
		};
	}
}