#pragma once

#include <string>
#include <unordered_map>

namespace Coda {
	namespace Utils {

		class ArgParser {
		public:
			ArgParser();
			void parse(int argc, char** argv);
			bool getFlag(std::string flag) const;
			std::string getValue(std::string flag) const;
			std::string getStandaloneValueAt(int i);
		
		private:
			std::vector<std::string> mStandaloneValues;
			std::unordered_map<std::string, std::string> mValues;
			std::unordered_map<std::string, bool> mFlags;
		
		};

	}
}