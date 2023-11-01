#include "FileReader.h"
#include "../Error/Error.h"
namespace Coda {
	namespace Utils {

		FileReader::FileReader(const std::string& filename)
			: mFile(filename)
		{
			if (!mFile)
				Error::Importer::raise("Failed to open file: " + filename);
		}

		FileReader::~FileReader()
		{
			mFile.close();
		}

		std::string FileReader::readToString()
		{
			if (!Error::Manager::isSafe())
				return "";
			if (!mFile)
				Error::Importer::raise("File not open.");
			std::string content;
			mFile.seekg(0, std::ios::end);
			content.reserve((const unsigned int)mFile.tellg());
			mFile.seekg(0, std::ios::beg);
			content.assign((std::istreambuf_iterator<char>(mFile)),
				std::istreambuf_iterator<char>());

			content.append("\0");
			return content;
		}

		std::vector<std::string> FileReader::readToLines()
		{
			if (!mFile)
				Error::Importer::raise("File not open.");
			std::vector<std::string> lines;
			std::string line;
			while (std::getline(mFile, line))
				lines.push_back(line);
			return lines;
		}

	}
}
