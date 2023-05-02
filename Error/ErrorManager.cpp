#include "ErrorManager.h"

using namespace Coda::Error;

int Manager::mErrorCount = 0;
int Manager::mLexerErrorCount = 0;
int Manager::mParserErrorCount = 0;

void Manager::raiseLexerError()
{
	mLexerErrorCount++;
	raiseError();
}

void Manager::raiseParserError()
{
	mParserErrorCount++;
	raiseError();
}

void Manager::raiseError()
{
	mErrorCount++;
}

bool Manager::isSafe()
{
	return (mErrorCount == 0);
}