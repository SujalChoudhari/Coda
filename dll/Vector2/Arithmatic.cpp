#include "../../FFI/FFI.h"

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
	auto arg1Props = args->getProperties()["first"]->getProperties();
	auto arg2Props = args->getProperties()["second"]->getProperties();

	Coda::Runtime::Type suggestedType = arg2Props["x"]->getType();
	std::string x;
	std::string y;
	if (arg1Props["x"]->getType() > suggestedType) {
		suggestedType = arg1Props["x"]->getType();
	}

	if (suggestedType == Coda::Runtime::Type::CHAR) {
		x = handleArithmeticOperation<unsigned char>(arg1Props["x"]->getValue(), functor, arg2Props["x"]->getValue());
		y = handleArithmeticOperation<unsigned char>(arg1Props["y"]->getValue(), functor, arg2Props["y"]->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::BYTE) {
		x = handleArithmeticOperation<unsigned char>(arg1Props["x"]->getValue(), functor, arg2Props["x"]->getValue());
		y = handleArithmeticOperation<unsigned char>(arg1Props["y"]->getValue(), functor, arg2Props["y"]->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::INT) {
		x = handleArithmeticOperation<int>(arg1Props["x"]->getValue(), functor, arg2Props["x"]->getValue());
		y = handleArithmeticOperation<int>(arg1Props["y"]->getValue(), functor, arg2Props["y"]->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::LONG) {
		x = handleArithmeticOperation<long>(arg1Props["x"]->getValue(), functor, arg2Props["x"]->getValue());
		y = handleArithmeticOperation<long>(arg1Props["y"]->getValue(), functor, arg2Props["y"]->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::FLOAT) {
		x = handleArithmeticOperation<float>(arg1Props["x"]->getValue(), functor, arg2Props["x"]->getValue());
		y = handleArithmeticOperation<float>(arg1Props["y"]->getValue(), functor, arg2Props["y"]->getValue());
	}
	else if (suggestedType == Coda::Runtime::Type::DOUBLE) {
		x = handleArithmeticOperation<double>(arg1Props["x"]->getValue(), functor, arg2Props["x"]->getValue());
		y = handleArithmeticOperation<double>(arg1Props["y"]->getValue(), functor, arg2Props["y"]->getValue());
	}

	auto resProps = resultValue->getProperties();

	resProps.insert({ "x" , args->getProperties()["first"] });
	resProps.insert({ "y" , args->getProperties()["second"] });
	resProps["x"]->setType(suggestedType);
	resProps["x"]->setValue(x);
	resProps["y"]->setType(suggestedType);
	resProps["y"]->setValue(y);
	resultValue->setType(Coda::Runtime::Type::OBJECT);
	resultValue->setProperties(resProps);
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