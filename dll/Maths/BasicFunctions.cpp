

#include "../../FFI/FFI.h"
#include <iostream>
#include <cmath>

extern "C" EXPORT void coda_sqrt(IValuePtr res, IValuePtr args, IEnvironment * env) {
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

extern "C" EXPORT void coda_ln(IValuePtr res, IValuePtr args, IEnvironment * env) {
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

extern "C" EXPORT void coda_log10(IValuePtr res, IValuePtr args, IEnvironment * env) {
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

extern "C" EXPORT void coda_exp(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double exponent = std::stod(args->getProperties()["exponent"]->getValue());

    double result = std::exp(exponent);
    res->setType(Coda::Runtime::Type::FLOAT);
    res->setValue(std::to_string(result));
}

extern "C" EXPORT void coda_abs(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double number = std::stod(args->getProperties()["number"]->getValue());

    double result = std::abs(number);
    res->setType(Coda::Runtime::Type::FLOAT);
    res->setValue(std::to_string(result));
}

extern "C" EXPORT void coda_round(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double number = std::stod(args->getProperties()["number"]->getValue());

    double result = std::round(number);
    res->setType(Coda::Runtime::Type::FLOAT);
    res->setValue(std::to_string(result));
}

extern "C" EXPORT void coda_mod(IValuePtr res, IValuePtr args, IEnvironment * env) {
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

extern "C" EXPORT void coda_pow(IValuePtr res, IValuePtr args, IEnvironment * env) {
    double value = std::pow(std::stod(args->getProperties()["first"]->getValue()), std::stoi(args->getProperties()["second"]->getValue()));

    res->setValue(std::to_string(value));
    res->setType(Coda::Runtime::Type::DOUBLE);
}

