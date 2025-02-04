#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <type_traits>
#include <stdexcept>

using std::mt19937;
using std::uniform_real_distribution;
using std::uniform_int_distribution;
using std::invalid_argument;
using std::is_arithmetic;
using std::is_floating_point;

class Random {
private:
    mt19937 generator;

public:
    Random(unsigned int seed);

    double GetDouble(double min, double max);

    int GetInt(int min, int max);

    bool P(double probability);

    void SetSeed(unsigned int seed);

    template<typename T>
    T Get(T min, T max) {
        static_assert(is_arithmetic<T>::value, "not a numeric type");
        if (max < min) {
            throw invalid_argument("max cannot be less than min");
        }
        
        if constexpr (is_floating_point<T>::value) {
            uniform_real_distribution<T> dist(min, max);
            return dist(generator);
        } else {
            uniform_int_distribution<T> dist(min, max);
            return dist(generator);
        }
    }
};

#endif // RANDOM_HPP