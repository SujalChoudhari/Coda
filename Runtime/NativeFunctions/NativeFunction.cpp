#include <iostream>
#include <chrono>
#include <thread>
#include "../Runtime.h"
#include "../../Error/Raiser.h"

namespace Coda {
	namespace Runtime {
		namespace Native {
			Value println(Value args, Environment env) {
				auto argv = print(args, env);
				std::cout << std::endl;
				return argv;
			}
			Value print(Value args, Environment env) {
				for (auto& it : args.properties) {
					if (it.second->type == Type::BOOL) {
						if (it.second->value == "0")
							std::cout << "false ";
						else std::cout << "true ";
					}
					else if (it.second->type == Type::CHAR) {
						std::cout << (char) std::stoi(it.second->value) << " ";
					}
					else {
						std::cout << it.second->value << " ";
					}
				}
				return args;
			}

			Value input(Value args, Environment env) {
				print(args, env);
				std::string input_str;
				std::getline(std::cin, input_str);
				return Value(Type::INT, input_str);
			}


			Value sleep(Value args, Environment env) {
				if (!args.properties.empty()) {
					if (args.properties.begin()->second->type == Type::INT) {
						int seconds = std::stoi(args.properties.begin()->second->value);
						std::this_thread::sleep_for(std::chrono::seconds(seconds));
					}
					else {
						Error::Runtime::raise("Invalid Argument type, type should be <int>.");
					}
				}
				else {
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}

				return Value(Type::NONE, "");
			}

			Value parseInt(Value args, Environment env) {

				std::string stringValue = args.properties.begin()->second->value;
				return Value(Type::INT, stringValue);
			}

			Value parseFloat(Value args, Environment env) {

				std::string stringValue = args.properties.begin()->second->value;
				return Value(Type::FLOAT, stringValue);
			}

			Value parseDouble(Value args, Environment env) {

				std::string stringValue = args.properties.begin()->second->value;
				return Value(Type::DOUBLE, stringValue);
			}

			Value parseBool(Value args, Environment env) {

				std::string stringValue = args.properties.begin()->second->value;
				if (stringValue == "1" || stringValue == "0") {
					return Value(Type::BOOL, stringValue);
				}
				else {
					Error::Runtime::raise("Cannot convert '" + stringValue + "' to a bool.");
					return Value(Type::NONE);
				}
			}

			Value parseByte(Value args, Environment env) {

				std::string stringValue = args.properties.begin()->second->value;
				return Value(Type::BYTE, stringValue);
			}
		}

	}
}
