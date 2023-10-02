#include "../../FFI/FFI.h"
#include <cstdlib>
#include <ctime>
#include <random>

std::mt19937 generator(std::time(nullptr)); // Mersenne Twister PRNG


extern "C" EXPORT void coda_random_float(IValuePtr res, IValuePtr args, IEnvironment * env)
{
	// Generate a random floating-point number in [0, 1)
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	double random_number = distribution(generator);

	res->setType(Coda::Runtime::Type::FLOAT);
	res->setValue(std::to_string(random_number));
}

extern "C" EXPORT void coda_random_int(IValuePtr res, IValuePtr args, IEnvironment * env)
{
    int minLimit = std::stoi(args->getProperties()["min"]->getValue());
    int maxLimit = std::stoi(args->getProperties()["max"]->getValue());
	std::uniform_int_distribution<int> distribution(minLimit, maxLimit);
	int random_integer = distribution(generator);

	res->setType(Coda::Runtime::Type::INT);
	res->setValue(std::to_string(random_integer));
}

extern "C" EXPORT void coda_random_long(IValuePtr res, IValuePtr args, IEnvironment * env)
{
    long minLimit = std::stol(args->getProperties()["min"]->getValue());
    long maxLimit = std::stol(args->getProperties()["max"]->getValue());
	// Generate a random long integer
    std::uniform_int_distribution<long> distribution(minLimit, maxLimit);
    long random_long = distribution(generator);

	res->setType(Coda::Runtime::Type::LONG);
	res->setValue(std::to_string(random_long));
}

extern "C" EXPORT void coda_random_choice(IValuePtr res, IValuePtr args, IEnvironment * env)
{
    IValuePtr& iterable = args->getProperties()["iterable"];

    if (iterable->getType() == Coda::Runtime::Type::LIST) {
        std::map<std::string, IValuePtr> selectionMap = iterable->getProperties();
        if (selectionMap.empty()) {
            throw "Cannot select from an empty list";
        }

        // Generate a random index within the map size
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<std::size_t> distribution(0, selectionMap.size() - 1);
        std::size_t randomIndex = distribution(generator);

        // Use an iterator to select the random item in the map
        auto it = selectionMap.begin();
        std::advance(it, randomIndex);

        // Get the key and value of the randomly selected item
        const std::string& selectedKey = it->first;
        IValuePtr selectedValue = it->second;

        // Set the result to the selected value
        res->setType(selectedValue->getType());
        res->setValue(selectedValue->getValue());
    }
    else if (iterable->getType() == Coda::Runtime::Type::STRING) {
        std::string selectionCharset = iterable->getValue();
        if (selectionCharset.empty()) {
            throw "Cannot select from an empty string";
        }

        // Generate a random index within the string size
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<std::size_t> distribution(0, selectionCharset.size() - 1);
        std::size_t randomIndex = distribution(generator);

        // Get the character at the randomly selected index
        char selectedChar = selectionCharset[randomIndex];

        // Set the result to the selected character as a string
        res->setType(Coda::Runtime::Type::CHAR);
        res->setValue(std::string(1, selectedChar));
    }
    else {
        throw "Type '" + iterable->getTypeAsString(iterable->getType()) + "' is not iterable";
    }
}