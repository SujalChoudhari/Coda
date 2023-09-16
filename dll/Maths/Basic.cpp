#include "../../FFI/FFI.h"
#include <iostream>
#include <cmath> // Include the cmath library for mathematical functions
#include <string>


template <typename T>
T getValue(const std::string& str)
{
	if (std::is_same_v<T, unsigned char>) {
		return static_cast<T>(str[0]);
	}
	else {
		return (T)std::stod(str);
	}
}
template<typename T>
std::string handleArithmeticOperation(const std::string& left, const std::string& functor, const std::string& right)
{

	T typeLeft = getValue<T>(left);
	T typeRight = getValue<T>(right);

	std::string result;
	if (functor == "+") {
		result = std::to_string(typeLeft + typeRight);
	}
	else if (functor == "-") {
		result = std::to_string(typeLeft - typeRight);
	}
	else if (functor == "*") {
		result = std::to_string(typeLeft * typeRight);
	}
	else if (functor == "/") {
		if (typeRight == 0) {
			throw "Division by Zero";
			return "";
		}
		result = std::to_string(typeLeft / typeRight);
	}
	return result;
}


void evaluate(IValuePtr args, std::string functor, IValuePtr resultValue) {
	IValue* arg1 = args->getProperties()["first"].get();
	IValue* arg2 = args->getProperties()["second"].get();

	Coda::Runtime::Type suggestedType = arg1->getType();
	std::string result;
	if (arg2->getType() > suggestedType) {
		suggestedType = arg2->getType();
	}

	if (suggestedType == Coda::Runtime::Type::CHAR) {
		result = handleArithmeticOperation<unsigned char>(arg1->getValue(), functor, arg2->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::BYTE) {
		result = handleArithmeticOperation<unsigned char>(arg1->getValue(), functor, arg2->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::INT) {
		result = handleArithmeticOperation<int>(arg1->getValue(), functor, arg2->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::LONG) {
		result = handleArithmeticOperation<long>(arg1->getValue(), functor, arg2->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::FLOAT) {
		result = handleArithmeticOperation<float>(arg1->getValue(), functor, arg2->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::DOUBLE) {
		result = handleArithmeticOperation<double>(arg1->getValue(), functor, arg2->getValue());
	}

	resultValue->setType(suggestedType);
	resultValue->setValue(result);
}



extern "C" __declspec(dllexport) void coda_add(IValuePtr res, IValuePtr args, IEnvironment * env) {
	evaluate(args, "+", res);
}

extern "C" __declspec(dllexport) void coda_sub(IValuePtr res, IValuePtr args, IEnvironment * env) {
	evaluate(args, "-", res);
}


extern "C" __declspec(dllexport) void coda_mul(IValuePtr res, IValuePtr args, IEnvironment * env) {
	evaluate(args, "*", res);
}

extern "C" __declspec(dllexport) void coda_div(IValuePtr res, IValuePtr args, IEnvironment * env) {
	evaluate(args, "/", res);
}

extern "C" __declspec(dllexport) void mod(IValuePtr res, IValuePtr args, IEnvironment * env) {
	if (args->getProperties()["first"]->getType() == Coda::Runtime::Type::INT
		&& args->getProperties()["second"]->getType() == Coda::Runtime::Type::INT) {
		int value = std::stoi(args->getProperties()["first"]->getValue()) % std::stoi(args->getProperties()["second"]->getValue());

		res->setValue(std::to_string(value));
		res->setType(Coda::Runtime::Type::INT);

	}
	else {
		throw " Operands of the % is/are not of type <int>";
	}
}

extern "C" __declspec(dllexport) void coda_pow(IValuePtr res, IValuePtr args, IEnvironment * env) {
	double value = std::pow(std::stod(args->getProperties()["first"]->getValue()), std::stoi(args->getProperties()["second"]->getValue()));

	res->setValue(std::to_string(value));
	res->setType(Coda::Runtime::Type::DOUBLE);
}

