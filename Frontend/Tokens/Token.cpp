#include "Token.h"


std::ostream& operator<<(std::ostream& os, const Token& head)
{
	os << "(" << static_cast<int>(head.type) << ": " << head.value << ")";
	return os;
}

