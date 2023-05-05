#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include "TokenType.h"

namespace Coda {
    namespace Frontend {
        extern std::map<std::string, TokenType> KEYWORD;
        extern std::string DIGITS;
        extern std::string FIRST_VALID_DIGITS;
        extern std::string DIGITS_EXTRA;
        extern const std::unordered_map<char, TokenType> DIGIT_EXTENTIONS;
    }
}