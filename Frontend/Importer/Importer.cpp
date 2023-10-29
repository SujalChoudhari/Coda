#include "Importer.h"
#include <algorithm>
#include <iostream>

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

namespace Coda
{
	namespace Frontend
	{
		std::string Importer::import(const std::string &filepath)
		{
			std::string sourceCode;
			if (fileExists(filepath))
			{
				Coda::Utils::FileReader fileReader(filepath);
				sourceCode = fileReader.readToString();
				if (!Error::Manager::isSafe())
				{
					Error::Importer::raise("Import failed, either file does not exist or circular import detected.");
				}
				sourceCode += "\n";
			}
			else
			{
				sourceCode = filepath;
			}

			size_t importIndex = findNextImport(sourceCode, 0);
			while (importIndex != std::string::npos)
			{
				size_t importEndIndex = findImportEnd(sourceCode, importIndex);
				std::string importString = extractImportString(sourceCode, importIndex, importEndIndex);
				std::string modulePath = getAbsImportPath(filepath, importString);

				if (mImportedFiles.find(modulePath) == mImportedFiles.end())
				{
					std::string moduleSource = import(modulePath);
					replaceImport(sourceCode, importIndex, importEndIndex, moduleSource);
					mImportedFiles.insert(modulePath); // Add the module path to the set of imported files
				}
				else
				{
					// If the module path is already in the set, replace the import with an empty string
					replaceImport(sourceCode, importIndex, importEndIndex, "");
				}
				importIndex = findNextImport(sourceCode, importIndex);
			}
			return sourceCode;
		}

		size_t Importer::findNextImport(const std::string &sourceCode, size_t startIndex)
		{
			return sourceCode.find("import", startIndex);
		}

		size_t Importer::findImportEnd(const std::string &sourceCode, size_t importIndex)
		{
			return sourceCode.find_first_of("\n;", importIndex);
		}

		std::string Importer::extractImportString(const std::string &sourceCode, size_t importIndex, size_t importEndIndex)
		{
			return sourceCode.substr(importIndex + 7, importEndIndex - importIndex - 7);
		}

		void Importer::replaceImport(std::string &sourceCode, size_t importIndex, size_t importEndIndex, const std::string &moduleSource)
		{
			sourceCode.replace(importIndex, importEndIndex - importIndex + 1, moduleSource);
		}

		std::string Importer::getAbsImportPath(const std::string &filename, std::string &importString)
		{
			size_t lastSlashIndex = filename.rfind("/");

			for (int i = 0; i < importString.size(); i++)
			{
				if (importString[i] == '.')
				{
					importString[i] = '/';
				}
			}

			std::string currentWorkingDirModulePath = filename.substr(0, lastSlashIndex) + "/" + importString + ".coda";
			if (fileExists(currentWorkingDirModulePath))
			{
				return currentWorkingDirModulePath;
			}

			std::string executablePath = getExecutablePath();
			std::string exeModulePath = executablePath + importString + ".coda";

			if (fileExists(exeModulePath))
			{
				return exeModulePath;
			}

			Error::Importer::raise("Imported file not found: " + importString);
		}

#ifdef _WIN32
		std::string Importer::getExecutablePath()
		{
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			PathRemoveFileSpecA(buffer);
			PathAddBackslashA(buffer);
			return std::string(buffer);
		}

		bool Importer::fileExists(const std::string &filePath)
		{
			DWORD fileAttributes = GetFileAttributesA(filePath.c_str());
			return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
		}
#else
		// Implementation for Unix-based platforms
		std::string Importer::getExecutablePath()
		{
			char buffer[MAX_PATH];
			ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
			if (len != -1)
			{
				buffer[len] = '\0';
				std::string path(buffer);
				size_t lastSlashIndex = path.rfind("/");
				if (lastSlashIndex != std::string::npos)
				{
					return path.substr(0, lastSlashIndex + 1);
				}
			}
			return "";
		}

		bool Importer::fileExists(const std::string &filePath)
		{
			struct stat buffer;
			return (stat(filePath.c_str(), &buffer) == 0);
		}
#endif
	}
}
