#include <iostream>
#include <chrono>
#include <thread>
#include "../RuntimeValue/Value.h"
#include "../Runtime.h"
#include "NativeFunction.h"
#include "../../Error/Raiser.h"

namespace Coda {
	namespace Runtime {
		namespace Native {
			IValuePtr println(IValuePtr args, IEnvironment* env) {
				auto argv = print(args, env);
				std::cout << std::endl;
				return argv;
			}
			IValuePtr print(IValuePtr args, IEnvironment* env) {
				for (auto& it : args->getProperties()) {
					if (it.second->getType() == Type::BOOL) {
						if (it.second->getValue() == "0" || it.second->getValue() == "false")
							std::cout << "false ";
						else std::cout << "true ";
					}
					else if (it.second->getType() == Type::CHAR) {
						std::cout << "'" << (char)std::stoi(it.second->getValue()) << "' ";
					}
					else if (it.second->getType() == Type::STRING) {
						std::cout << "\"" << it.second->getValue() << "\" ";
					}
					else if (it.second->getType() == Type::LIST) {
						std::cout << "[";
						print(it.second, env);
						std::cout << "]\n";
					}
					else {
						std::cout << it.second->getValue();
						if (args->getType() == Type::LIST)
							std::cout << ", ";
						else if (args->getType() == Type::OBJECT)
							std::cout << ",\n";
						else
							std::cout << " ";

					}
				}
				return args;
			}

			IValuePtr input(IValuePtr args, IEnvironment* env) {
				print(args, env);
				std::string input_str;
				std::getline(std::cin, input_str);
				return std::make_shared<Value>(Type::INT, input_str);
			}


			IValuePtr sleep(IValuePtr args, IEnvironment* env) {
				if (!args->getProperties().empty()) {
					if (args->getProperties().begin()->second->getType() == Type::INT) {
						int seconds = std::stoi(args->getProperties().begin()->second->getValue());
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

			IValuePtr parseInt(IValuePtr args, IEnvironment* env) {
				std::string stringValuePtr = args->getProperties().begin()->second->getValue();
				return std::make_shared<Value>(Type::INT, stringValuePtr);
			}

			IValuePtr parseFloat(IValuePtr args, IEnvironment* env) {
				std::string stringValuePtr = args->getProperties().begin()->second->getValue();
				return std::make_shared<Value>(Type::FLOAT, stringValuePtr);
			}

			IValuePtr parseDouble(IValuePtr args, IEnvironment* env) {
				std::string stringValuePtr = args->getProperties().begin()->second->getValue();
				return std::make_shared<Value>(Type::DOUBLE, stringValuePtr);
			}

			IValuePtr parseBool(IValuePtr args, IEnvironment* env) {
				std::string stringValuePtr = args->getProperties().begin()->second->getValue();
				if (stringValuePtr == "1" || stringValuePtr == "0") {
					return std::make_shared<Value>(Type::BOOL, stringValuePtr);
				}
				else {
					Error::Runtime::raise("Cannot convert '" + stringValuePtr + "' to a bool.");
					return nullptr;
				}
			}

			IValuePtr parseByte(IValuePtr args, IEnvironment* env) {
				std::string stringValuePtr = args->getProperties().begin()->second->getValue();
				return std::make_shared<Value>(Type::BYTE, stringValuePtr);
			}


			IValuePtr quit(IValuePtr args, IEnvironment* env) {
				int exitCode = 0;
				if (!args->getProperties().empty()) {
					exitCode = std::stoi(args->getProperties().begin()->second->getValue());
					if (args->getProperties().begin()->second->getType() == Type::INT)
						Error::Runtime::raise("Invalid exit status code. Received '" + args->getProperties().begin()->second->getValue() + "' expected <int>.", args->getEndPosition());
				}
				std::exit(exitCode); // terminated by user
				return nullptr;
			}
		}
	}
}
