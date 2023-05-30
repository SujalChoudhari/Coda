#pragma once

/*
	Runtime

	This sub package is used to execute the AST.
	AST is interpreted and executed using the Interpreter class.
	
	Interpreters:
	Runtime::Interpreter is used to interpret the AST.

	Environment:
	Runtime::Environment is used to store the variables and their values.

	NativeFunctions:
	Runtime::NativeFunction is used to define native functions for the language.
*/

#include "Environment/Environment.h"

#include "Interpreter/Interpreter.h"

#include "RuntimeValue/Type.h"
#include "RuntimeValue/Value.h"

#include "NativeFunctions/NativeFunction.h"
