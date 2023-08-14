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

			SCOPE,
			OBJECT,
			LIST,
			NATIVE_FUNCTION,
			FUNCTION,

			JUMP
		};

	} // namespace Runtime
} // namespace Coda


