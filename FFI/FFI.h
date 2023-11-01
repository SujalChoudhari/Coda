#pragma once

#include <string>
#include <type_traits>

#if defined(_MSC_VER)
//  Microsoft 
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  do nothing and hope for the best?
#define EXPORT
#define IMPORT
#pragma message ("Unknown dynamic link import/export semantics.")
#endif

#include "IEnvironment.h"
#include "INode.h"
#include "IValue.h"

template <typename T>
T getValue(const std::string& str)
{
	if (std::is_same<T, unsigned char>::value) {
		return static_cast<T>(str[0]);
	}
	else {
		#if defined(_MSC_VER)
		return (T)std::stod(str);
		#else
		return (T)std::stod(str.c_str());
		#endif
	}
}
