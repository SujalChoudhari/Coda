#include "Value.h"

namespace Coda {
	namespace Runtime {
		std::ostream& operator<< (std::ostream& os, const Value& value) {
			os << "runtime: " << value.value;
			return os;
		}
	}
}