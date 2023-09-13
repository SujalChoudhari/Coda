#pragma once
#include <string>
#include <map>
#include "../Runtime/RuntimeValue/Type.h"
#include "../Error/Position.h"
#include <memory>

#define IVALUE_TO_VALUE(x) std::dynamic_pointer_cast<Value>(x)
using namespace Coda::Runtime;

class IValue {

public:
	IValue() = default;
	IValue copy() const;

public:
	virtual Type getType() const = 0;
	virtual void setType(Type type) = 0;

	virtual std::string getValue() const = 0;
	virtual void setType(std::string value) = 0;

	virtual std::map<std::string, std::shared_ptr<IValue>> getProperties() const = 0;

	virtual Coda::Error::Position getStartPosition() const = 0;
	virtual Coda::Error::Position getEndPosition() const = 0;

public:
	static bool isTruthy(const IValue& value);
	static std::string getTypeAsString(Type type);
};
typedef std::shared_ptr<IValue> IValuePtr;
