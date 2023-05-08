#pragma once


#include <fstream>
#include <string>
#include <vector>

namespace Coda {
	namespace Utils {

		class FileReader {
		public:
			FileReader(const std::string& filename);
			~FileReader();

			std::string readToString();
			std::vector<std::string> readToLines();

		private:
			std::ifstream mFile;
		};

	}
}