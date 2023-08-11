#include <iostream>
#include <chrono>
#include <thread>
#include "../RuntimeValue/Value.h"
#include "../Runtime.h"
#include "../../Error/Raiser.h"

namespace Coda {
	namespace Runtime {
		namespace Native {
			ValuePtr println(ValuePtr args, Environment env) {
				auto argv = print(args, env);
				std::cout << std::endl;
				return argv;
			}
			ValuePtr print(ValuePtr args, Environment env) {
				for (auto& it : args->properties) {
					if (it.second->type == Type::BOOL) {
						if (it.second->value == "0" || it.second->value == "false")
							std::cout << "false ";
						else std::cout << "true ";
					}
					else if (it.second->type == Type::CHAR) {
						std::cout << (char)std::stoi(it.second->value) << " ";
					}
					else {
						std::cout << it.second->value << " ";
					}
				}
				return args;
			}

			ValuePtr input(ValuePtr args, Environment env) {
				print(args, env);
				std::string input_str;
				std::getline(std::cin, input_str);
				return std::make_shared<Value>(Type::INT, input_str);
			}


			ValuePtr sleep(ValuePtr args, Environment env) {
				if (!args->properties.empty()) {
					if (args->properties.begin()->second->type == Type::INT) {
						int seconds = std::stoi(args->properties.begin()->second->value);
						std::this_thread::sleep_for(std::chrono::seconds(seconds));
					}
					else {
						Error::Runtime::raise("Invalid Argument type, type should be <int>.");
					}
				}
				else {
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}

				return nullptr;
			}

			ValuePtr parseInt(ValuePtr args, Environment env) {

				std::string stringValuePtr = args->properties.begin()->second->value;
				return std::make_shared<Value>(Type::INT, stringValuePtr);
			}

			ValuePtr parseFloat(ValuePtr args, Environment env) {

				std::string stringValuePtr = args->properties.begin()->second->value;
				return std::make_shared<Value>(Type::FLOAT, stringValuePtr);
			}

			ValuePtr parseDouble(ValuePtr args, Environment env) {

				std::string stringValuePtr = args->properties.begin()->second->value;
				return std::make_shared<Value>(Type::DOUBLE, stringValuePtr);
			}

			ValuePtr parseBool(ValuePtr args, Environment env) {

				std::string stringValuePtr = args->properties.begin()->second->value;
				if (stringValuePtr == "1" || stringValuePtr == "0") {
					return std::make_shared<Value>(Type::BOOL, stringValuePtr);
				}
				else {
					Error::Runtime::raise("Cannot convert '" + stringValuePtr + "' to a bool.");
					return nullptr;
				}
			}

			ValuePtr parseByte(ValuePtr args, Environment env) {

				std::string stringValuePtr = args->properties.begin()->second->value;
				return std::make_shared<Value>(Type::BYTE, stringValuePtr);
			}


			ValuePtr quit(ValuePtr args, Environment env) {
				int exitCode = 0;
				if (!args->properties.empty() && args->properties.begin()->second->type == Type::INT)
					exitCode = std::stoi(args->properties.begin()->second->value);
				else
					Error::Runtime::raise("Invalid exit status code. Received '" + args->properties.begin()->second->value + "' expected <int>.",args->endPosition);
				std::exit(exitCode); // terminated by user
				return nullptr;
			}
		}

	}
}
