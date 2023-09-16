

#include "../../FFI/FFI.h"
#include <iostream>
#include <cmath>

extern "C" __declspec(dllexport) void coda_sin(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double angle = std::stod(args->getProperties()["angle"]->getValue());

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(std::sin(angle)));
}

extern "C" __declspec(dllexport) void coda_cos(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double angle = std::stod(args->getProperties()["angle"]->getValue());

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(std::cos(angle)));
}

extern "C" __declspec(dllexport) void coda_tan(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double angle = std::stod(args->getProperties()["angle"]->getValue());

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(std::tan(angle)));
}

extern "C" __declspec(dllexport) void coda_asin(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double value = std::stod(args->getProperties()["value"]->getValue());

	if (value < -1.0 || value > 1.0) {
		throw "Invalid input for arcsin. Value must be in the range [-1, 1].";
	}
	else {
		double result = std::asin(value);
		res->setType(Coda::Runtime::Type::FLOAT);
		res->setValue(std::to_string(result));
	}
}

extern "C" __declspec(dllexport) void coda_acos(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double value = std::stod(args->getProperties()["value"]->getValue());

	if (value < -1.0 || value > 1.0) {
		throw "Invalid input for arccos. Value must be in the range [-1, 1].";
	}
	else {
		double result = std::acos(value);
		res->setType(Coda::Runtime::Type::FLOAT);
		res->setValue(std::to_string(result));
	}
}

extern "C" __declspec(dllexport) void coda_atan(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double value = std::stod(args->getProperties()["value"]->getValue());

	double result = std::atan(value);
	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(result));
}


extern "C" __declspec(dllexport) void coda_sec(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double angle = std::stod(args->getProperties()["angle"]->getValue());
	double secValue = 1.0 / std::cos(angle);

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(secValue));
}

extern "C" __declspec(dllexport) void coda_csc(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double angle = std::stod(args->getProperties()["angle"]->getValue());
	double cscValue = 1.0 / std::sin(angle);

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(cscValue));
}

extern "C" __declspec(dllexport) void coda_cot(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double angle = std::stod(args->getProperties()["angle"]->getValue());
	double cotValue = 1.0 / std::tan(angle);

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(cotValue));
}

extern "C" __declspec(dllexport) void coda_sinh(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double angle = std::stod(args->getProperties()["angle"]->getValue());
	double sinhValue = std::sinh(angle);

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(sinhValue));
}

extern "C" __declspec(dllexport) void coda_cosh(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double angle = std::stod(args->getProperties()["angle"]->getValue());
	double coshValue = std::cosh(angle);

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(coshValue));
}

extern "C" __declspec(dllexport) void coda_tanh(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double angle = std::stod(args->getProperties()["angle"]->getValue());
	double tanhValue = std::tanh(angle);

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(tanhValue));
}
