#pragma once


#include <fstream>
#include <string>
#include <vector>

namespace Coda {
	namespace Utils {
		/*
			A FileReader that reads a file to a string or a vector of lines.
		*/
		class FileReader {
		public:
			/*
				Create a new instance of FileReader.
				@param filename - The name of the file to read.
			*/
			FileReader(const std::string& filename);
			~FileReader();

			/*
				Read the file to a single string.
				@return The file as a string.
			*/
			std::string readToString();

			/*
				Read the file to a vector of lines.
				@return The file as a vector of lines.
			*/
			std::vector<std::string> readToLines();

		private:
			std::ifstream mFile;
		};

	}
}