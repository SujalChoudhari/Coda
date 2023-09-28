#pragma once
#include <string>
#include <map>
#include <memory>

#define IVALUE_TO_VALUE(x) std::dynamic_pointer_cast<Value>(x)

namespace Coda {
	namespace Runtime {
		enum class Type {
			NONE,
			UNDEFINED,

			BOOL,
			CHAR,
			BYTE,
			INT,
			LONG,
			FLOAT,
			DOUBLE,
			STRING,
			POINTER,

			SCOPE,
			OBJECT,
			LIST,
			NATIVE_FUNCTION,
			FUNCTION,

			JUMP
		};
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
	virtual void setValue(std::string value) = 0;

	virtual std::map<std::string, std::shared_ptr<IValue>> getProperties() const = 0;
	virtual void setProperties(std::map<std::string, std::shared_ptr<IValue>> props) = 0;

	virtual Coda::Error::Position getStartPosition() const = 0;
	virtual void setStartPosition(Coda::Error::Position pos) = 0;
	virtual Coda::Error::Position getEndPosition() const = 0;
	virtual void setEndPosition(Coda::Error::Position pos) = 0;
	virtual std::shared_ptr<IValue> createNewValue(Coda::Runtime::Type type, std::string value) = 0;

public:
	static bool isTruthy(const IValue& value);
	virtual std::string getTypeAsString(Coda::Runtime::Type type) = 0;
};
typedef std::shared_ptr<IValue> IValuePtr;
