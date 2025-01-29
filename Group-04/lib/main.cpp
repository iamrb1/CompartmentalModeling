#include "cse/CseAssert.hpp"
#include <iostream>
#include <string>

int with_assert_false(std::string x) {
  if (x == "hello") {
    return 123;
  }
  cse_assert(false);
}

int with_assert_never(std::string x) {
  if (x == "hello") {
    return 123;
  }
  cse_assert_never();
}

int main() {
  std::cout << "normal assert" << std::endl;
  cse_assert(1 + 1 == 2);
  std::cout << std::endl << "assert never" << std::endl;
  with_assert_never("goodbye");
  with_assert_false("goodbye");
  return 0;
}
