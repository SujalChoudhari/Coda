#pragma once
#include "../Runtime.h"

#define NATIVE_FUNCTION(x) Value x(Value argument, Environment env)

namespace Coda {
	namespace Runtime {
		namespace Native {
			NATIVE_FUNCTION(print);
			NATIVE_FUNCTION(println);
			NATIVE_FUNCTION(sleep);
			NATIVE_FUNCTION(input);
		}
	}
}