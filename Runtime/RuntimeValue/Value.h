#pragma once

#include <string>
#include <ostream>
#include <map>
#include "Type.h"
#include "../../Error/Position.h"

namespace Coda {
	namespace Runtime {

		class Value {
		public:
			Type type = Type::NONE;
			std::string value;
			Error::Position startPosition;
			Error::Position endPosition;
			std::map<std::string, std::shared_ptr<Value>> properties = {};

		public:
			Value() = default;
			Value(Type type, Error::Position start, Error::Position end)
				: type(type), value(), startPosition(start), endPosition(end) {}
			Value(Type type, std::string value="", Error::Position start = Error::Position(), Error::Position end = Error::Position())
				: type(type), value(value), startPosition(start), endPosition(end) {}

			friend std::ostream& operator<<(std::ostream& os, const Value& value);
		};

	}
}
