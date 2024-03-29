#pragma once
#include "../Runtime.h"

// Create a native function with the given name, arguments and env remains same
#define NATIVE_FUNCTION(x) IValuePtr x(IValuePtr args, IEnvironment* env)

namespace Coda {
	namespace Runtime {
		// Library of native functions
		namespace Native {
			// Print the argument to the console
			NATIVE_FUNCTION(print);

			// Print the argument to the console and append a new line
			NATIVE_FUNCTION(println);

			// Sleep for the given number of seconds
			NATIVE_FUNCTION(sleep);

			// Get a string from the console
			NATIVE_FUNCTION(input);

			// Convert the argument to a Int
			NATIVE_FUNCTION(parseInt);

			// Convert the argument to a Float
			NATIVE_FUNCTION(parseFloat);

			// Convert the argument to a Double
			NATIVE_FUNCTION(parseDouble);

			// Convert the argument to a Byte
			NATIVE_FUNCTION(parseByte);

			// Convert the argument to a Bool
			NATIVE_FUNCTION(parseBool);

			// Quit the program
			NATIVE_FUNCTION(quit);

		}
	} // namespace Runtime
} // namespace Coda