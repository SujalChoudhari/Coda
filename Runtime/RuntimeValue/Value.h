#pragma once

#include <string>
#include <ostream>
#include <map>
#include "Type.h"
#include "../../Error/Position.h"

namespace Coda {
	namespace Runtime {
		/*
			A value represents a variable or a object or a function.
			I.e. a storable value.
		*/
		class Value {
		public:
			// The type of the value.
			Type type = Type::NONE;

			// The value of the value.
			std::string value;

			// The position of the value in the source code.
			Error::Position startPosition;

			// The position of the value in the source code.
			Error::Position endPosition;

			// The properties of the value.
			std::map<std::string, std::shared_ptr<Value>> properties = {};

		public:
			Value() = default;
			Value(Type type, Error::Position start, Error::Position end)
				: type(type), value(), startPosition(start), endPosition(end) {}
			Value(Type type, std::string value="", Error::Position start = Error::Position(), Error::Position end = Error::Position())
				: type(type), value(value), startPosition(start), endPosition(end) {}
			friend std::ostream& operator<<(std::ostream& os, const Value& value);
		};

	} // namespace Runtime
} // namespace Coda
