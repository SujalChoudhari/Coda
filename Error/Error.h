#pragma once
/*
	Error.h

	Error Sub package is used to raise, count and handle errors.
	
	Manager:
	Error::Manager is used to count and keep track of errors.
	When an error is raised, the Manager will increment the error count.
	When the error count is greater than 0, the Manager will return false
	when isSafe() is called. This is used to stop the execution of the program
	when an error is raised.

	Raiser:
	Error::Raiser is used to raise errors. 
	It is used in the following way:
		Error::Lexer::raise("Error message");
	
	The types of errors that can be raised are,
	- Lexer		[LEXER]
	- Parser	[PARSER]
	- Runtime	[RNTIME]

	Position:
	Error::Position is used to keep track of the position of the error.
	It is used in the following way:
		Error::Lexer::raise("Error message", Error::Position(1, 1));

*/
#include "Manager.h"
#include "Position.h"
#include "Raiser.h"

// Error Handling Macros
#define IF_ERROR_RETURN(x)  if (!Coda::Error::Manager::isSafe()) return x
#define IF_ERROR_CONTINUE if (!Coda::Error::Manager::isSafe()) continue
