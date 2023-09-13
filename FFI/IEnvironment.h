#pragma once
#include <functional>
#include "INode.h"
#include "IValue.h"


class IEnvironment {
public:
	typedef std::function<IValuePtr(IValuePtr args, IEnvironment* env)> Function;
	virtual ~IEnvironment() = default;

	// Use IValuePtr instead of ValuePtr
	virtual IValuePtr declareFunctionParameter(const std::string name, const IValuePtr& value) = 0;
	virtual IValuePtr declareOrAssignVariable(const std::string& name, const IValuePtr& value, bool isConstant = false) = 0;
	virtual IValuePtr declareNativeFunction(const std::string& name, Function function) = 0;
	virtual IValuePtr declareUserDefinedFunction(const std::string& name, const INode& astNode) = 0;
	virtual IValuePtr callFunction(const std::string& name, IValuePtr args, IEnvironment& env) = 0;
	virtual IValuePtr lookupSymbol(std::string varname) = 0;
};