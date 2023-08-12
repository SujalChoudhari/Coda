#pragma once

namespace Coda {
	namespace Runtime {
		// Represents the Types of the Runtime::Value
		enum class Type {
			NONE,
			UNDEFINED,

			BOOL,
			CHAR,
			BYTE,
			INT,
			LONG,
			FLOAT,
			DOUBLE,
			STRING,

			OBJECT,
			LIST,
			NATIVE_FUNCTION,
			FUNCTION,
		};

	} // namespace Runtime
} // namespace Coda


