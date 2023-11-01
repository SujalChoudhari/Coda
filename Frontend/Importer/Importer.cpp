#include "Importer.h"
#include <algorithm>
#include <Windows.h>


#include "../../Error/Error.h"

namespace Coda {
	namespace Frontend {
		std::string Importer::import(const std::string& filepath) {
			std::string sourceCode;
			if (fileExists(filepath)) {
				Coda::Utils::FileReader fileReader(filepath);
				sourceCode = fileReader.readToString();
				if (!Error::Manager::isSafe()) {
					Error::Importer::raise("Import failed, either file does not exist or circular import detected.");
				}
				sourceCode += "\n";
			}
			else {
				sourceCode = filepath;
			}


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

			for (unsigned int i = 0; i < importString.size(); i++)
			{
				if (importString[i] == '.')
				{
					importString[i] = '/';
				}
			}

			// Check if the file exists in the base directory
			std::string baseModulePath = filename.substr(0, lastSlashIndex + 1) + importString + ".coda";
			if (fileExists(baseModulePath)) {
				return baseModulePath;
			}

			// If the file doesn't exist in the base directory, check the directory where the executable is located
			std::string executablePath = getExecutablePath(); // Implement this function to get the executable's directory
			std::string exeModulePath = executablePath + importString + ".coda";

			if (fileExists(exeModulePath)) {
				return exeModulePath;
			}

			// If the file doesn't exist in either location, raise an error
			Error::Importer::raise("Imported file not found: " + importString);
			return "";
		}

		std::string Importer::getExecutablePath() {
			wchar_t buffer[MAX_PATH];
			GetModuleFileNameW(NULL, buffer, MAX_PATH);
			std::wstring executablePath(buffer);
			size_t lastSlashIndex = executablePath.rfind(L"\\");
			if (lastSlashIndex != std::wstring::npos) {
				executablePath = executablePath.substr(0, lastSlashIndex + 1);
			}

			// Convert the wide character string to a narrow character string (UTF-8)
			std::string narrowExecutablePath(executablePath.begin(), executablePath.end());

			return narrowExecutablePath;
		}

		bool Importer::fileExists(const std::string& filePath) {
			std::wstring wideFilePath(filePath.begin(), filePath.end());
			DWORD fileAttributes = GetFileAttributes(wideFilePath.c_str());
			return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
		}
	}
}
