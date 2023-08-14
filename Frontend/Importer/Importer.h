#pragma once

#include <string>
#include "../../Utils/FileReader.h"
#include <unordered_set>
namespace Coda {
	namespace Frontend {

		class Importer {
		public:
			std::string import(const std::string& filepath);

		private:
			std::unordered_set<std::string> mImportedFiles;
		private:
			size_t findNextImport(const std::string& sourceCode, size_t startIndex);
			size_t findImportEnd(const std::string& sourceCode, size_t importIndex);
			std::string extractImportString(const std::string& sourceCode, size_t importIndex, size_t importEndIndex);
			void replaceImport(std::string& sourceCode, size_t importIndex, size_t importEndIndex, const std::string& moduleSource);
			std::string getAbsImportPath(const std::string& filename, std::string& importString);
		};
	}
}
