#pragma once

#include <string>
#include "../../Utils/FileReader.h"
#include <unordered_set>
namespace Coda {
	namespace Frontend {

		class Importer {
		public:
			/*
				Imports the file from the given path, 
				recursively handles the import declaration
				and returns the final source code.
			*/
			std::string import(const std::string& filepath);

		private:

			// Stores all the Imported files
			std::unordered_set<std::string> mImportedFiles;
		private:
			/*
				find the poistion of next import statement
			*/
			size_t findNextImport(const std::string& sourceCode, size_t startIndex);
			
			/*
				find the position of end of import statement
			*/
			size_t findImportEnd(const std::string& sourceCode, size_t importIndex);

			/*
				get the import string
			*/
			std::string extractImportString(const std::string& sourceCode, size_t importIndex, size_t importEndIndex);
			
			/*
				replaces the import statement with the contents of the given file
			*/
			void replaceImport(std::string& sourceCode, size_t importIndex, size_t importEndIndex, const std::string& moduleSource);

			/*
				convert an import statement into an actual, absolute path
			*/
			std::string getAbsImportPath(const std::string& filename, std::string& importString);

			/*
				get the path to the executable
			*/
			std::string getExecutablePath();
			/*
				check if file exists or nots
			*/
			bool fileExists(const std::string& filePath);
		};
	} // namespace Frontend
} // namespace Coda
