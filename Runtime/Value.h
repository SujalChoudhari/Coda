#pragma once
#include <string>
#include <ostream>
#include "Type.h"
#include "../Error/Position.h"

namespace Coda {
	namespace Runtime {


		class Value {
		public:
			Type type;
			std::string value;
			Error::Position startPosition;
			Error::Position endPosition;

		public:
			Value()
				:type(Type::NONE), value("none") {}
			Value(Type type, Error::Position start, Error::Position end)
				:type(type), value("none"), startPosition(start), endPosition(end) {}
			Value(Type type, std::string value, Error::Position start, Error::Position end)
				:type(type), value(value), startPosition(start), endPosition(end) {}

			friend std::ostream& operator<< (std::ostream& os, const Value& value);
		};

	}
}