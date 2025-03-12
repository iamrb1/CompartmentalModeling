/**
* @file Random.hpp
* @author George Faraj
*/
#pragma once

#include <cassert>
#include <iostream>
#include <random>
#include <stdexcept>
#include <type_traits>

using std::invalid_argument;
using std::is_arithmetic;
using std::is_floating_point;
using std::mt19937;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

namespace cse {
class Random {
 private:
  mt19937 generator;

 public:
  Random(unsigned int seed);
  double GetDouble(double min, double max) { return Get<double>(min,max); }
  int GetInt(int min, int max);
  bool P(double probability);
  void SetSeed(unsigned int seed);

  /**
   * Generate a random number of numeric type.
   *
   * @param min The minimum value 
   * @param max The maximum value 
   * @return A random value between min and max
   */
  template <typename T>
  T Get(T min, T max) {
    static_assert(is_arithmetic<T>::value, "not a numeric type");
    assert(max >= min);

    if constexpr (is_floating_point<T>::value) {
      uniform_real_distribution<T> dist(min, max);
      return dist(generator);
    } else {
      uniform_int_distribution<T> dist(min, max);
      return dist(generator);
    }
  }
};

/**
 * Constructs a Random object with a specified seed.
 *
 * @param seed The seed for the random number generator
 */
Random::Random(unsigned int seed) : generator(seed) {}

/**
 * Generates a random integer between a specified range.
 *
 * @param min The minimum value 
 * @param max The maximum value 
 * @return A random integer between min and max
 */
int Random::GetInt(int min, int max) {
  assert(max >= min);
  uniform_int_distribution<int> distribution(min, max);
  return distribution(generator);
}

/**
 * Checks based on a given probability.
 *
 * @param probability The probability 
 * @return true if occurs else false 
 */
bool Random::P(double probability) {
  assert(probability >= 0.0);
  assert(probability <= 1.0);
  uniform_real_distribution<double> distribution(0.0, 1.0);
  return distribution(generator) < probability;
}

/**
 * Sets a seed.
 *
 * @param seed The new seed value
 */
void Random::SetSeed(unsigned int seed) { generator.seed(seed); }

}  // namespace cse