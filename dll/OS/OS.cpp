#include "../../FFI/FFI.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cstdlib>
#include <cstdio>

extern "C" EXPORT void coda_run_command(IValuePtr res, IValuePtr args, IEnvironment * env) {
	const std::string command = args->getProperties()["command"]->getValue();

	// Use std::system to run the command
	int result = std::system(command.c_str());

	res->setType(Coda::Runtime::Type::INT);
	res->setValue(std::to_string(result));
}


extern "C" EXPORT void coda_get_env(IValuePtr res, IValuePtr args, IEnvironment * env) {
	std::string key = args->getProperties()["key"]->getValue();
	char* value;
	size_t len;
	_dupenv_s(&value, &len, key.c_str());
	res->setType(Coda::Runtime::Type::STRING);
	res->setValue(value);
}

extern "C" EXPORT void coda_set_env(IValuePtr res, IValuePtr args, IEnvironment * env) {
	std::string key = args->getProperties()["key"]->getValue();
	std::string value = args->getProperties()["value"]->getValue();

	_putenv_s(key.c_str(), value.c_str());
	res->setType(Coda::Runtime::Type::BOOL);
	res->setValue("1");
}

// Functions for pointer handling
void* getPointer(IValuePtr value) {
	void* ptr = reinterpret_cast<void*>(std::stoull(value->getValue()));
	return ptr;
}

void createValueOfTypePointer(IValuePtr res, void* pointer) {
	res->setType(Coda::Runtime::Type::POINTER);
	res->setValue(std::to_string(reinterpret_cast<unsigned long long>(pointer)));
}

void freePointer(IValuePtr value) {
	void* ptr = reinterpret_cast<void*>(std::stoull(value->getValue()));
	delete ptr;
}