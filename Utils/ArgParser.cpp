#include "ArgParser.h"
#include <stdexcept>
#include <vector>

Coda::Utils::ArgParser::ArgParser()
{
	mFlags.insert_or_assign("-w", false);
}

void Coda::Utils::ArgParser::parse(int argc, char** argv)
{
	while (argc > 1)
	{
		std::string arg = argv[1];

		if (arg[0] != '-')
		{
			if (mSubCommand.empty())
				mSubCommand = arg;
			else
				mStandaloneValues.push_back(arg);
		}
		else
		{
			if (arg.find('=') != std::string::npos)
			{
				std::string flag = arg.substr(1, arg.find('=') - 1);
				std::string value = arg.substr(arg.find('=') + 1);

				mValues[flag] = value;
			}
			else
			{
				mFlags[arg] = true;
			}
		}

		argc--;
		argv++;
	}
}

bool Coda::Utils::ArgParser::getFlag(std::string flag) const
{
	return mFlags.at(flag);
}

std::string Coda::Utils::ArgParser::getValue(std::string flag) const
{
	return mValues.at(flag);
}

std::string Coda::Utils::ArgParser::getStandaloneValueAt(int i)
{
	if (i >= mStandaloneValues.size())
		return "";
	return mStandaloneValues.at(i);
}

std::vector<std::string>* Coda::Utils::ArgParser::getAllStandaloneValues()
{
	return &mStandaloneValues;
}

std::string Coda::Utils::ArgParser::getSubCommand()
{
	return mSubCommand.empty() ? "none" : mSubCommand;
}
