#pragma once

#include "IEnvironment.h"
#include "INode.h"
#include "IValue.h"

template <typename T>
T getValue(const std::string& str)
{
	if (std::is_same_v<T, unsigned char>) {
		return static_cast<T>(str[0]);
	}
	else {
		return (T)std::stod(str);
	}
}
