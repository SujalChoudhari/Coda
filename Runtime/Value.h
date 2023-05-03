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
			Error::Position position;

		public:
			Value()
				:type(Type::NONE), value("none") {}
			Value(Type type, Error::Position pos)
				:type(type), value("none"), position(pos) {}
			Value(Type type, std::string value, Error::Position pos)
				:type(type), value(value), position(pos) {}

			friend std::ostream& operator<< (std::ostream& os, const Value& value);
		};

	}
}