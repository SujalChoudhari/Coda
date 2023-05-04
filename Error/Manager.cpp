#include "Manager.h"

using namespace Coda::Error;

int Manager::mErrorCount = 0;
int Manager::mLexerErrorCount = 0;
int Manager::mParserErrorCount = 0;
int Manager::mRuntimeErrorCount = 0;

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

void Coda::Error::Manager::raiseRuntimeError()
{
	mRuntimeErrorCount++;
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

void Coda::Error::Manager::reset()
{
	mErrorCount = 0;
	mRuntimeErrorCount = 0;
	mParserErrorCount = 0;
	mLexerErrorCount = 0;
}
