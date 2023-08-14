#include "Importer.h"
#include <algorithm>
#include "../../Error/Error.h"

namespace Coda {
	namespace Frontend {
		std::string Importer::import(const std::string& filepath) {
			Coda::Utils::FileReader fileReader(filepath);
			std::string sourceCode = fileReader.readToString();
			sourceCode += "\n";

			size_t importIndex = findNextImport(sourceCode, 0);
			while (importIndex != std::string::npos) {
				size_t importEndIndex = findImportEnd(sourceCode, importIndex);
				std::string importString = extractImportString(sourceCode, importIndex, importEndIndex);
				std::string modulePath = getAbsImportPath(filepath, importString);
				
				std::string moduleSource = import(modulePath);
				if (mImportedFiles.find(modulePath) == mImportedFiles.end()) {
					replaceImport(sourceCode, importIndex, importEndIndex, moduleSource);
					mImportedFiles.insert(modulePath);
				}
				else {
					replaceImport(sourceCode, importIndex, importEndIndex, "");
				}
				importIndex = findNextImport(sourceCode, importIndex);
			}

			return sourceCode;
		}

		size_t Importer::findNextImport(const std::string& sourceCode, size_t startIndex) {
			return sourceCode.find("import", startIndex);
		}

		size_t Importer::findImportEnd(const std::string& sourceCode, size_t importIndex) {
			return sourceCode.find_first_of("\n;", importIndex);
		}

		std::string Importer::extractImportString(const std::string& sourceCode, size_t importIndex, size_t importEndIndex) {
			return sourceCode.substr(importIndex + 7, importEndIndex - importIndex - 7);
		}

		void Importer::replaceImport(std::string& sourceCode, size_t importIndex, size_t importEndIndex, const std::string& moduleSource) {
			sourceCode.replace(importIndex, importEndIndex - importIndex + 1, moduleSource);
		}

		std::string Importer::getAbsImportPath(const std::string& filename, std::string& importString) {
			size_t lastSlashIndex = filename.rfind("/");
			if (lastSlashIndex == std::string::npos) {
				Error::Importer::raise("Invalid file path: " + filename);
			}

			for(int i = 0; i < importString.size(); i++) {
				if (importString[i] == '.') {
					importString[i] = '/';
				}
			}

			std::string modulePath = filename.substr(0, lastSlashIndex + 1) + importString + ".coda";
			return modulePath;
		}
	}
}
