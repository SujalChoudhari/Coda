#pragma once
#include <vector>
#include <string>
#include "../RuntimeValue/Value.h"
namespace Coda {
	namespace Runtime {
		class Pool {

		public:
			static std::vector<int> intPool;
			static std::vector<long> longPool;
			static std::vector<float> floatPool;
			static std::vector<double> doublePool;
			static std::vector<std::string> stringPool;
			static std::vector<Value> objectPool;

		public:
			template <typename tValueType>
			void addValueOfType(tValueType value) {
				if (typeid(tValueType) == typeid(int)) {
					intPool.push_back(value);
				}
				else if (typeid(tValueType) == typeid(long)) {
					longPool.push_back(value);
				}
				else if (typeid(tValueType) == typeid(float)) {
					floatPool.push_back(value);
				}
				else if (typeid(tValueType) == typeid(double)) {
					doublePool.push_back(value);
				}
				else if (typeid(tValueType) == typeid(std::string)) {
					stringPool.push_back(value);
				}
				else if (typeid(tValueType) == typeid(Value)) {
					objectPool.push_back(value);
				}
			}
		};
	} // namespace Runtime
} // namespace Coda