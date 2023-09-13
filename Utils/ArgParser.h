#pragma once

#include <string>
#include <unordered_map>


namespace Coda {
	namespace Utils {
		/*
			A Command Line Argument Parser.
			Stores the arguments as Flags (e.g. -flag), 
			Values (e.g. -flag=value), 
			Standalone Values (e.g. value) 
		*/
		class ArgParser {
		public:

			/*
				Create a new ArgParser.
			*/
			ArgParser();

			/*
				Parse the arguments passed to the program.
				Updates the current state of ArgParser. 
				Old data is lost.
				@param argc - The number of arguments passed to the program.
				@param argv - The arguments passed to the program.
			*/
			void parse(int argc, char** argv);

			/*
				Get the value of a flag.
				These flags are set by using the syntax: -flag.
				@param flag - The flag to get the value of.
				@return The value of the flag.
			*/
			bool getFlag(std::string flag) const;

			/*
				Get the string value of a flag.
				These flags are set by using the syntax: -flag=value.
				@param flag - The flag to get the value of.
				@return The string value of the flag.

			*/
			std::string getValue(std::string flag) const;

			/*
				Get the standalone value at the specified index.
				These values are set by using the syntax: value.
				@param index - The index of the standalone value to get.
				@return The standalone value at the specified index.

			*/
			std::string getStandaloneValueAt(int index);
			
			/*
				Get the sub command from the cli input.
				@return The string value of the command.
			*/
			std::string getSubCommand();

		private:
			std::string mSubCommand;
			std::vector<std::string> mStandaloneValues;
			std::unordered_map<std::string, std::string> mValues;
			std::unordered_map<std::string, bool> mFlags;
		};
	} // namespace Utils
} // namespace Coda