#include "Random.hpp"

using std::invalid_argument;
using std::uniform_real_distribution;
using std::uniform_int_distribution;

Random::Random(unsigned int seed) : generator(seed) {}

double Random::GetDouble(double min, double max) {
    if (max < min) {
        throw invalid_argument("max cannot be less than min");
    }
    uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

int Random::GetInt(int min, int max) {
    if (max < min) {
        throw invalid_argument("max cannot be less than min");
    }
    uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

bool Random::P(double probability) {
    if (probability < 0.0 || probability > 1.0) {
        throw invalid_argument("probability isnt between 0.0 and 1.0");
    }
    uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator) < probability;
}

void Random::SetSeed(unsigned int seed) {
    generator.seed(seed);
}