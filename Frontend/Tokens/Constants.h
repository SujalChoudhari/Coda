#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include "TokenType.h"

namespace Coda {
    namespace Frontend {
        // Language Keywords for Source Code
        extern std::map<std::string, TokenType> KEYWORD;

        // Supported Digit Characters
        extern std::string DIGITS;
        
        // The digits which can be the first digit of a number
        extern std::string FIRST_VALID_DIGITS;
        
        // The extra chacters which can be used for better readability
        extern std::string DIGITS_EXTRA;
        
        // The extra chacters which are used to diffrentiate between a native types.
        extern const std::unordered_map<char, TokenType> DIGIT_EXTENSIONS;
    } // namespace Frontend
} // namespace Coda