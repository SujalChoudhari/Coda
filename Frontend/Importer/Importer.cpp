#include "Importer.h"
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#define PATH_SEPARATOR "\\"
#else
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#define MAX_PATH PATH_MAX
#define PATH_SEPARATOR "/"
#endif

#include "../../Error/Error.h"


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

			for (int i = 0; i < importString.size(); i++) {
				if (importString[i] == '.') {
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
		}

		#ifdef _WIN32
	// Implementation for Windows platform
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
		DWORD fileAttributes = GetFileAttributesW(wideFilePath.c_str());
		return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
	}
#else
	// Implementation for Unix-based platforms
	std::string Importer::getExecutablePath() {
		char buffer[MAX_PATH];
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
		if (len != -1) {
			buffer[len] = '\0';
			return std::string(buffer);
		}
		return "";
	}

	bool Importer::fileExists(const std::string& filePath) {
		struct stat buffer;
		return (stat(filePath.c_str(), &buffer) == 0);
	}
#endif
	}
}
