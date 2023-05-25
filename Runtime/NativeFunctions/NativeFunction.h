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
			NATIVE_FUNCTION(parseInt);
			NATIVE_FUNCTION(parseFloat);
			NATIVE_FUNCTION(parseDouble);
			NATIVE_FUNCTION(parseByte);
			NATIVE_FUNCTION(parseBool);

		}
	}
}