#include "cse/CseAssert.hpp"

#include <iostream>
#include <string>

namespace cse::_assert_internal {
void Assert(AssertArgs const &args, const char *args_text, const char *file,
            int line, const char *function) {
  std::string cond_string = std::string{args_text};
  std::cout << std::boolalpha;
  std::cout << "condition: " << args.condition << '\n';
  std::cout << "condition text: "
            << cond_string.substr(1, cond_string.size() - 2) << '\n';
  std::cout << "file: " << file << '\n';
  std::cout << "line: " << line << '\n';
  std::cout << "function: " << function << '\n';
  std::cout << "mesasge: " << args.message.value_or("none") << '\n';
}

void AssertNever(const char *file, int line, const char *function) {
  std::cout << "file: " << file << '\n';
  std::cout << "line: " << line << '\n';
  std::cout << "function: " << function << '\n';
  std::abort();
}
}  // namespace cse::_assert_internal
