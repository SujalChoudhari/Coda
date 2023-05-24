#include <iostream>
#include <chrono>
#include <thread>
#include "../Runtime.h"
#include "../../Error/Raiser.h"

namespace Coda {
	namespace Runtime {
		namespace Native {
			Value  println(Value args, Environment env) {
				for (auto& it : args.properties) {
					if (it.second->type == Type::BOOL) {
						if (it.second->value == "0")
							std::cout << "false ";
						else std::cout << "true ";
					}
					else {
						std::cout << it.second->value << " ";
					}
				}
				std::cout << std::endl;
				return args;
			}
			Value print(Value args, Environment env) {
				for (auto& it : args.properties) {
					if (it.second->type == Type::BOOL) {
						if (it.second->value == "0")
							std::cout << "false ";
						else std::cout << "true ";
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
						// Invalid argument type
						Error::Runtime::raise("Invalid Argument type, type should be <int>.");
					}
				}
				else {
					// No argument provided, default sleep duration
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}

				return Value(Type::NONE, "");
			}

		}

	}
}
