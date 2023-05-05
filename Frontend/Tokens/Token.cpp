#include "Token.h"

namespace Coda {
	namespace Frontend {
		std::ostream& operator<<(std::ostream& os, const Token& head)
		{
			os << "(" << static_cast<int>(head.type) << ": " << head.value << ")";
			return os;
		}
	}
}

