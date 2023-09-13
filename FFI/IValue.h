#pragma once
#include <string>
#include <map>
#include <memory>

#define IVALUE_TO_VALUE(x) std::dynamic_pointer_cast<Value>(x)

namespace Coda {
	namespace Runtime {
		enum class Type;
	}

	namespace Error {
		class Position;
	}
}


class IValue {

public:
	IValue() = default;
	IValue copy() const;

public:
	virtual  Coda::Runtime::Type getType() const = 0;
	virtual void setType(Coda::Runtime::Type type) = 0;

	virtual std::string getValue() const = 0;
	virtual void setType(std::string value) = 0;

	virtual std::map<std::string, std::shared_ptr<IValue>> getProperties() const = 0;

	virtual Coda::Error::Position getStartPosition() const = 0;
	virtual Coda::Error::Position getEndPosition() const = 0;

public:
	static bool isTruthy(const IValue& value);
	static std::string getTypeAsString(Coda::Runtime::Type type);
};
typedef std::shared_ptr<IValue> IValuePtr;
