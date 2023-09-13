#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <tuple>
#include "../../FFI/IEnvironment.h"
#include "../../Error/Error.h"
#include "../RuntimeValue/Value.h"
#include "../../Frontend/Node/Node.h"


namespace Coda {
	namespace Runtime {
		/*
			An Environment is used to store the variables and their values.
			A new environment is created for every function call.
			Environment is responsible for variable lookup and assignment.
		*/

		class Environment :public IEnvironment {
			// A function is a callable object.
		public:
			typedef std::function<IValuePtr(IValuePtr value, IEnvironment* scope)> Function;

			// <name, declaration environment, body (AST)>
			typedef std::tuple<std::string, Coda::Runtime::Environment, Coda::Frontend::Node> UserDefinedFunction;



		public:

			// Creates a new environment also setting its parent. 
			Environment(Environment* parentEnvironment);

			// Generate a new environment with the root environment as its parent.
			static Environment root();

			virtual IValuePtr declareFunctionParameter(const std::string name, const IValuePtr& value) override;

			/*
				Declares a new variable or assigns a value to an existing variable.
				Throws an error if the variable is constant.
				@param name - The name of the variable.
				@param value - The value of the variable.
				@param isConstant - Whether the variable is constant or not.
				@return - The value of the variable.
			*/
			virtual IValuePtr declareOrAssignVariable(const std::string& name, const IValuePtr& value, bool isConstant = false) override;

			/*
				Overload of declareOrAssignVariable(const std::string&, const Value&, bool).
			*/
			IValuePtr declareOrAssignVariable(const Frontend::Node& name, const IValuePtr& value, bool isConstant = false);

			/*
				Declare native function.
				@param name - The name of the function.
				@param function - The function.
				@return - The value of the function.
			*/
			virtual IValuePtr declareNativeFunction(const std::string& name, IEnvironment::Function function) override;

			/*
				Declare a user defined function.
				@param name - The name of the function.
				@param astNode - The AST of the function which will be evaluated when the function is called.
				@return - The value of the function.
			*/
			virtual IValuePtr declareUserDefinedFunction(const std::string& name, const INode& astNode) override;

			/*
				call a UserDefinedFunction.
				@param name - The name of the function.
				@param args - The arguments of the function.
				@param env - The environment in which the function will be called.
				@return - The last evaluated value of the function.
			*/
			virtual IValuePtr callFunction(const std::string& name, IValuePtr args, IEnvironment& env) override;

			/*
				looks for the given symbol in the current environment and its parents.
				returns the value of the symbol if found.
				throws an error if the symbol is not found.
				@param varname - The name of the symbol.
				@return - The value of the symbol.
			*/
			virtual IValuePtr lookupSymbol(std::string varname) override;

			/*
				Looks for the user defined function with the given name.
				@param name - The name of the function.
				@return - The function.
			*/
			UserDefinedFunction* getFunction(const std::string& name);

			/*
				Add a user defined function to the environment.
				@param name - The name of the function.
				@param function - The function.
			*/
			ValuePtr addFunction(const std::string& name, const Frontend::Node& astNode, Environment& env);

			/*
				removes a symbol
			*/
			void remove(const std::string& name);

			/*
				Add a scope as a child of the current environment
			*/
			void addScope(const std::string& name, std::shared_ptr<Environment> env);

			/*
				Gets a Scope with the given name
			*/
			std::shared_ptr<Environment> getScope(const std::string& name);

			/*
				Gets the scope which contains the given function name
			*/
			std::shared_ptr<Environment> getScopeForFunctionInScope(const std::string& name);


		private:
			// Parent environment
			Environment* mParent;

			// variables declared in this scope
			std::map<std::string, IValuePtr> mSymbols;

			// functions declared in this scope
			std::map<std::string, Function> mFunctions;

			// constants declared in this scope
			std::set<std::string> mConstants;

			// user defined functions declared in this scope
			std::vector<UserDefinedFunction> mUserDefinedFunctions;

			// child scopes declared in the current scope
			std::map<std::string, std::shared_ptr<Environment>> mScopes;

		private:
			// gets the environment with the given function name
			Environment* resolveWithParentAndScope(std::string name);
			Environment* resolveWithScope(std::string name);

			// Creates a new environment.
			Environment();

		};
	} // namespace Runtime
} // namespace Coda