#pragma once

#include <concepts>
#include <iostream>
#include <optional>
#include <string>

// Entrypoints for assert macros.
//
// These use variadic macro to prevent accidentally splitting on commas within
// condition. Inspired by c++ proposal P2264R7.
//
// Curly braces are used (instead of parenthesis as in P2264R7) as these
// arguments are actually passed to the constructor of `AssertArgs`.
#define cse_assert(condition_msg...) \
  _cse_assert_internal(_cse_assert_arg_1(condition_msg), {condition_msg})
#define cse_assert_never(...)                   \
  ::cse::_assert_internal::AssertNeverNoReturn( \
      __FILE__, __LINE__, __func__ __VA_OPT__(, __VA_ARGS__))
#define cse_assert_eq(args...)                                              \
  _cse_assert_eq_internal(_cse_assert_arg_1(args), _cse_assert_arg_2(args), \
                          cse::_assert_internal::BinaryAssertArgs{args})

// These macros perform a best-effort recovery of the text of the original
// arguments for display output on assertion failure.
//
// This will incorrectly split arguments in the same way that std's assert macro
// does, but in this case only affects the display and not the logic. We could
// potentially implement a parser to determine more accurately how the arguments
// are split, but this is already an edge case.
#define _cse_assert_arg_1(arg1, ...) #arg1
#define _cse_assert_arg_2(arg1, arg2, ...) #arg2

// Internal macros which fill in the file, line number, etc. and call into the
// actual C++ functions.
#define _cse_assert_internal(condition_text, condition_msg...)             \
  ::cse::_assert_internal::Assert(condition_msg, condition_text, __FILE__, \
                                  __LINE__, __func__)
#define _cse_assert_eq_internal(lhs_text, rhs_text, rest...)            \
  ::cse::_assert_internal::AssertEq(rest, lhs_text, rhs_text, __FILE__, \
                                    __LINE__, __func__)

// Debug macros
#ifdef NDEBUG
// Adding (void)0 here ensures callers still
// needs to end the line with a semi-colon.
// Source: https://stackoverflow.com/a/1306618/4678913
#define dbg_assert(...) (void)0
#define dbg_assert_never(...) (void)0
#else
#define dbg_assert(...) cse_assert(__VA_ARGS__)
#define dbg_assert_never(...)                 \
  ::cse::_assert_internal::AssertNeverReturn( \
      __FILE__, __LINE__, __func__ __VA_OPT__(, __VA_ARGS__))
#endif

// Do *not* access anything inside this namespace directly
namespace cse::_assert_internal {
// underscore followed by capital letter is
// reserved
#ifdef _cse_ASSERT_TEST
class AssertTestException : std::exception {};
void Fail() { throw AssertTestException(); }
#else
[[noreturn]] void Fail() { std::abort(); }
#endif

// This class allows us to accept both a condition and an optional message.
//
// The message can't be a macro argument, since if it's the first argument then
// it would have to be mandatory. It can't be the last argument since there's no
// way to extract just the last argument, and even if we could we wouldn't know
// if it's part of the condition or if it's the message.
//
// Using a class provides a constructor which does the argument splitting at the
// language level, rather than the pre-processor level.
class AssertArgs {
 public:
  bool condition;
  std::optional<const char *> message = std::nullopt;
  AssertArgs(bool condition) : condition(condition) {};
  AssertArgs(bool condition, const char *message)
      : condition(condition), message(message) {};
};

template <typename T>
class BinaryAssertArgs {
 public:
  T lhs;
  T rhs;
  std::optional<const char *> message = std::nullopt;

  BinaryAssertArgs(T lhs, T rhs) : lhs(lhs), rhs(rhs) {};
  BinaryAssertArgs(T lhs, T rhs, const char *message)
      : lhs(lhs), rhs(rhs), message(message) {};
};

void PrintAssertMessage(
    std::optional<const char *> message_opt = std::nullopt) {
  std::cout << "Assertion ";
  if (auto message = message_opt) {
    std::cout << "'" << message.value() << "' ";
  }
  std::cout << "failed!";
}

void PrintLocation(const char *file, int line, const char *function) {
  std::cout << "\nLocated at: " << file << ":" << line << " (in function "
            << function << ")\n";
}

void Assert(AssertArgs const &args, const char *args_text, const char *file,
            int line, const char *function) {
  if (args.condition) {
    return;
  }
  std::string cond_string = std::string{args_text};

  PrintAssertMessage(args.message);
  std::cout << "\n  Condition evaluated to "
               "false: "
            << cond_string << "\n";
  PrintLocation(file, line, function);
  Fail();
}

template <typename T>
concept printable = requires(T a) { std::cout << a; };

template <typename T>
  requires std::equality_comparable<T>
void AssertEq(BinaryAssertArgs<T> const &args, const char *lhs_text,
              const char *rhs_text, const char *file, int line,
              const char *function) {
  if (args.lhs == args.rhs) {
    return;
  }
  std::string lhs_string = std::string{lhs_text};
  std::string rhs_string = std::string{rhs_text};

  PrintAssertMessage(args.message);
  if constexpr (printable<T>) {
    std::cout << " These values are different:\n"
              << "   left: " << args.lhs << "\n  right: " << args.rhs << "\n";
    PrintLocation(file, line, function);
  } else {
    std::cout << "  These expressions evaluated to different values:\n"
              << "   left: " << lhs_string << "\n  right: " << rhs_string
              << "\n";
    PrintLocation(file, line, function);
    std::cout << "hint: implement operator<< to see the underlying values\n";
  }

  Fail();
}

// technically does return when in test mode
#ifndef _cse_ASSERT_TEST
[[noreturn]]
#endif
void AssertNeverNoReturn(
    const char *file, int line, const char *function,
    std::optional<const char *> message_opt = std::nullopt) {
  PrintAssertMessage(message_opt);
  PrintLocation(file, line, function);
  Fail();
}

// dbg_assert_never doesn't uphold noreturn in release mode (since it is a
// no-op), so this function acts like AssertNeverNoReturn but without the
// [[noreturn]] marker
void AssertNeverReturn(const char *file, int line, const char *function,
                       std::optional<const char *> message_opt = std::nullopt) {
  AssertNeverNoReturn(file, line, function, message_opt);
}

}  // namespace cse::_assert_internal
