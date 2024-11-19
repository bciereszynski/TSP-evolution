#include "RandomGenerator.h"

#include <random>

RandomGenerator::RandomGenerator() : engine(rd()) {}

size_t RandomGenerator::operator()() {
    std::uniform_int_distribution<size_t> dist(min(), max());
    return dist(engine);
}