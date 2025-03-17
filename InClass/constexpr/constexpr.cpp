#include <iostream>
#include <cmath>
#include <assert.h>
#include <ctime>         // For std::clock
#include <string>

static constexpr double LN2 = 0.6931471805599453;

constexpr double Log2(double in) {
  assert(in > 0.0);

  if (in == 1.0) return 0.0;
  if (in >= 2.0) return Log2(in/2) + 1.0;
  if (in < 1) return Log2(in*2) - 1.0;

  assert(in >= 1.0 && in < 2.0);

  double sum = 0.0;
  double term = (in - 1.0) / (in + 1.0);
  double term2 = term * term;

  for (int i = 1; i < 20; i += 2) {
    sum += (1.0 / i) * term;
    term *= term2;
  }

  return sum * 2 / LN2;
}

constexpr double Log(double value, double base=2.0) {
  return Log2(value) / Log2(base);
}

int main()
{
  constexpr double in = 0.13579;

  double total = 0.0;
  std::clock_t start_time = std::clock();
  for (size_t i = 0; i < 1'000'000'000; ++i) {
    double x = std::log2(in);
    total += x;
  }
  std::clock_t end_time = std::clock();

  std::cout << "std log time = "
            << (end_time - start_time) / static_cast<double>(CLOCKS_PER_SEC)
            << " sec" << std::endl;


  total = 0.0;
  start_time = std::clock();
  for (size_t i = 0; i < 1'000'000'000; ++i) {
    constexpr double y = Log2(in);
    total += y;
  }
  end_time = std::clock();

  std::cout << "constexpr Log time = "
            << (end_time - start_time) / CLOCKS_PER_SEC
            << " sec" << std::endl;

  // std::cout << "x = " << x << std::endl;
  // std::cout << "y = " << y << std::endl;
  std::cout << "Log10(10000) = " << Log(10000, 10) << std::endl;
}