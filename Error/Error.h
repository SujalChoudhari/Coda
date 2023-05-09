#pragma once

#include "Error.h"
#include "Manager.h"
#include "Position.h"
#include "Raiser.h"

#define IF_ERROR_RETURN(x)  if (!Coda::Error::Manager::isSafe()) return x
#define IF_ERROR_CONTINUE if (!Coda::Error::Manager::isSafe()) continue
