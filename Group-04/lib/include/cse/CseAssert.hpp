#pragma once

// use of __VA_ARGS__ prevents accidentally splitting on commas
// inspired by c++ proposal P2264R7
#define cse_assert(...) _cse_assert_internal((__VA_ARGS__))
#define _cse_assert_internal(condition, ...)                                   \
  ::cse::_assert_internal::Assert(condition, #condition, __FILE__, __LINE__,   \
                                  __func__);

#define cse_assert_never()                                                     \
  ::cse::_assert_internal::AssertNever(__FILE__, __LINE__, __func__);

namespace cse {
namespace _assert_internal {
void Assert(bool condition, const char *condition_text, const char *file,
            int line, const char *function);

[[noreturn]] void AssertNever(const char *file, int line, const char *function);
} // namespace _assert_internal
} // namespace cse
