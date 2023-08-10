#include "Value.h"

namespace Coda {
	namespace Runtime {
		std::ostream& operator<< (std::ostream& os, const Value& value) {
			os << "runtime: " << value.value;
			return os;
		}

		bool Value::isTruthy(const Value& value) {
			if (value.type == Type::BOOL
				&& (value.value == "true" || value.value == "1")) {
				return true;
			}
			else if (!value.value.empty() && value.value != "0") {
				return true;
			}
			else return false;
		}
	}
}