#pragma once

#include "IEnvironment.h"
#include "INode.h"
#include "IValue.h"

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
    #pragma warning Unknown dynamic link import/export semantics.
#endif

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