#include <iostream>
#include <string>

#include "cse/CseAssert.hpp"

namespace cse {
namespace _assert_internal {
void Assert(bool condition, const char *condition_text, const char *file,
            int line, const char *function) {
  std::string cond_string = std::string{condition_text};
  std::cout << std::boolalpha;
  std::cout << "condition: " << condition << std::endl;
  std::cout << "condition text: "
            << cond_string.substr(1, cond_string.size() - 2) << std::endl;
  std::cout << "file: " << file << std::endl;
  std::cout << "line: " << line << std::endl;
  std::cout << "function: " << function << std::endl;
}

void AssertNever(const char *file, int line, const char *function) {
  std::cout << "file: " << file << std::endl;
  std::cout << "line: " << line << std::endl;
  std::cout << "function: " << function << std::endl;
  std::abort();
}
} // namespace _assert_internal
} // namespace cse
