#include <iostream>
#include "Value.h"
#include "Type.h"

namespace Coda {
	namespace Runtime {
		namespace Native {
			void print(Value argument) {
				for (auto& it : argument.properties) {
					std::cout << it.first << std::endl;
				}
			}

		}
	}
}