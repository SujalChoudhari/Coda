#include "../../FFI/FFI.h"
#include <iostream>

extern "C" __declspec(dllexport)  void add(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double arg1 = std::stod(args->getProperties()["1"]->getValue());
	double arg2 = std::stod(args->getProperties()["2"]->getValue());

	res->setType(Coda::Runtime::Type::INT);
	res->setValue(std::to_string(arg1 + arg2));

}