#include <random>

class RandomGenerator {
public:
    typedef size_t result_type;
    static size_t min() { return 0; }
    static size_t max() { return std::numeric_limits<size_t>::max(); }

    RandomGenerator();

    size_t operator()();

private:
    std::random_device rd;
    std::default_random_engine engine;
};