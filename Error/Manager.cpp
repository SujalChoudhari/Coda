#include "Manager.h"

using namespace Coda::Error;

int Manager::mErrorCount = 0;



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
}
