#include "../../FFI/FFI.h"
#include <cstdlib> // For rand_r() and srand()
#include <ctime>   // For srand() seed

unsigned int seed;

// Seed the random number generator (usually done once at the start of your program)
void coda_setup()
{
    static bool seeded = false;
    if (!seeded)
    {
        unsigned int seed = static_cast<unsigned int>(time(nullptr))^ static_cast<unsigned int>(pthread_self());
        srand(seed);
        seeded = true;
    }
}

extern "C" EXPORT void coda_random_float(IValuePtr res, IValuePtr args, IEnvironment *env)
{
    // Generate a random floating-point number in [0, 1)
    double random_number = static_cast<double>(rand_r(&seed)) / RAND_MAX;

    res->setType(Coda::Runtime::Type::FLOAT);
    res->setValue(std::to_string(random_number));
}

extern "C" EXPORT void coda_random_int(IValuePtr res, IValuePtr args, IEnvironment *env)
{
    // Generate a random integer
    int random_integer = rand_r(&seed);

    res->setType(Coda::Runtime::Type::INT);
    res->setValue(std::to_string(random_integer));
}

extern "C" EXPORT void coda_random_long(IValuePtr res, IValuePtr args, IEnvironment *env)
{
    // Generate a random long integer
    long random_long = static_cast<long>(rand_r(&seed));

    res->setType(Coda::Runtime::Type::LONG);
    res->setValue(std::to_string(random_long));
}
