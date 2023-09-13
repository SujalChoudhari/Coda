#include "Value.h"

namespace Coda {
	namespace Runtime {
		std::ostream& operator<< (std::ostream& os, const Value& value) {
			os << "runtime: " << value.value;
			return os;
		}

		Value::Value(const IValuePtr& ivalue)
		{
			this->value = ivalue->getValue();
			this->type = ivalue->getType();
			this->properties = ivalue->getProperties();
			this->startPosition = ivalue->getStartPosition();
			this->endPosition = ivalue->getEndPosition();
		}

		Value Value::copy() const {
			return Value(Type(type), std::string(value), Error::Position(startPosition), Error::Position(endPosition));
		}

		bool Value::isTruthy(const IValue& value) {
			if (value.getType() == Type::BOOL
				&& (value.getValue() == "true" || value.getValue() == "1")) {
				return true;
			}
			else if (!value.getValue().empty() && value.getValue() != "0") {
				return true;
			}
			else return false;
		}
		std::string Value::getTypeAsString(Type type)
		{
			std::string types[] = {
				"<none>",
				"<undefined>",
				"<bool>",
				"<char>",
				"<byte>",
				"<int>",
				"<long>",
				"<float>",
				"<double>",
				"<string>",
				"<scope>",
				"<object>",
				"<list>",
				"<native_function>",
				"<function>",
				"<jump>"
			};
			return types[(int)type];
		}
		Type Value::getType() const
		{
			return type;
		}
		void Value::setType(Type type)
		{
			this->type = type;
		}
		std::string Value::getValue() const
		{
			return this->value;
		}
		void Value::setType(std::string value)
		{
			this->value = value;
		}
		std::map<std::string, std::shared_ptr<IValue>> Value::getProperties() const
		{
			return properties;
		}
		Error::Position Value::getStartPosition() const
		{
			return startPosition;
		}
		Error::Position Value::getEndPosition() const
		{
			return endPosition;
		}
	}
}