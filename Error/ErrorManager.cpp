#include "ErrorManager.h"


int ErrorManager::mErrorCount = 0;
int ErrorManager::mLexerErrorCount = 0;
int ErrorManager::mParserErrorCount = 0;

void ErrorManager::raiseLexerError()
{
	mLexerErrorCount++;
	raiseError();
}

void ErrorManager::raiseParserError()
{
	mParserErrorCount++;
	raiseError();
}

void ErrorManager::raiseError()
{
	mErrorCount++;
}

bool ErrorManager::isSafe()
{
	return (mErrorCount == 0);
}