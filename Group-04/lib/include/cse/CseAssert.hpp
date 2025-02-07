#pragma once

// use of __VA_ARGS__ prevents accidentally splitting on commas
// inspired by c++ proposal P2264R7
#include <optional>
#include <string>
#define cse_assert(condition_msg...) _cse_assert_internal({condition_msg})
#define _cse_assert_internal(condition_msg...)                             \
  ::cse::_assert_internal::Assert(condition_msg, #condition_msg, __FILE__, \
                                  __LINE__, __func__)

#define cse_assert_never() \
  ::cse::_assert_internal::AssertNever(__FILE__, __LINE__, __func__)

#ifdef NDEBUG
// (void)0 trick from here https://stackoverflow.com/a/1306618/4678913
#define dbg_assert(...) (void)0
#else
#define dbg_assert(...) cse_assert(__VA_ARGS__)
#endif

namespace cse {
namespace _assert_internal {
class AssertArgs {
 public:
  bool condition;
  std::optional<const char *> message = std::nullopt;

  AssertArgs(bool condition) : condition(condition) {};
  AssertArgs(bool condition, const char *message)
      : condition(condition), message(message) {};
};

void Assert(AssertArgs const &args, const char *args_text, const char *file,
            int line, const char *function);

[[noreturn]] void AssertNever(AssertArgs const &args, const char *file,
                              int line, const char *function);
}  // namespace _assert_internal
}  // namespace cse
