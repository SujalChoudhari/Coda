#pragma once

#include <string>

namespace Coda {
	namespace Utils {
		namespace Colors {
			const std::string RESET = "\033[0m";
			const std::string ACCENT = "\033[1;36m";
			const std::string ERROR = "\033[0;31m";
			const std::string WARNING = "\033[0;33m";
			const std::string SUCCESS = "\033[0;32m";
		} // namespace Colors
	} // namespace Utils
} // namespace Coda