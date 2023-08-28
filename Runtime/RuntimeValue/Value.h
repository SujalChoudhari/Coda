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
			Value(std::shared_ptr<Value> other)
				: type(other->type), value(other->value), startPosition(other->startPosition), endPosition(other->endPosition), properties(other->properties) {}
			Value(Type type, Error::Position start, Error::Position end)
				: type(type), value(), startPosition(start), endPosition(end) {}
			Value(Type type, std::string value = "", Error::Position start = Error::Position(), Error::Position end = Error::Position())
				: type(type), value(value), startPosition(start), endPosition(end) {}
			Value(const Value& other) = default;

			Value copy() const;

		public:
			/*
				Check if the given value is truthy.
				"1", true, non zero string are truthy
			*/
			static bool isTruthy(const Value& value);

			/*
				Converts the type to a string.
			*/
			static std::string getTypeAsString(Type type);

		public:
			friend std::ostream& operator<<(std::ostream& os, const Value& value);
		};

		typedef std::shared_ptr<Value> ValuePtr;

	} // namespace Runtime
} // namespace Coda
