

#include "../../FFI/FFI.h"
#include <iostream>
#include <cmath>

extern "C" __declspec(dllexport) void squareRoot(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double number = std::stod(args->getProperties()["number"]->getValue());

    if (number < 0) {
        throw "Square root of a negative number is not defined.";
    }
    else {
        double result = std::sqrt(number);
        res->setType(Coda::Runtime::Type::FLOAT);
        res->setValue(std::to_string(result));
    }
}

extern "C" __declspec(dllexport) void naturalLog(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double value = std::stod(args->getProperties()["value"]->getValue());

    if (value <= 0) {
        throw "Natural logarithm is not defined for non-positive numbers.";
    }
    else {
        double result = std::log(value);
        res->setType(Coda::Runtime::Type::FLOAT);
        res->setValue(std::to_string(result));
    }
}

extern "C" __declspec(dllexport) void base10Log(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double value = std::stod(args->getProperties()["value"]->getValue());

    if (value <= 0) {
        throw "Base-10 logarithm is not defined for non-positive numbers.";
    }
    else {
        double result = std::log10(value);
        res->setType(Coda::Runtime::Type::FLOAT);
        res->setValue(std::to_string(result));
    }
}

extern "C" __declspec(dllexport) void exponential(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double exponent = std::stod(args->getProperties()["exponent"]->getValue());

    double result = std::exp(exponent);
    res->setType(Coda::Runtime::Type::FLOAT);
    res->setValue(std::to_string(result));
}

extern "C" __declspec(dllexport) void absoluteValue(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double number = std::stod(args->getProperties()["number"]->getValue());

    double result = std::abs(number);
    res->setType(Coda::Runtime::Type::FLOAT);
    res->setValue(std::to_string(result));
}

extern "C" __declspec(dllexport) void roundToNearest(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double number = std::stod(args->getProperties()["number"]->getValue());

    double result = std::round(number);
    res->setType(Coda::Runtime::Type::FLOAT);
    res->setValue(std::to_string(result));
}
