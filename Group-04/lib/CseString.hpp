/**
 * @file CseString.hpp
 * @author Krishna
 *
 */

#pragma once

#include <string>
#include "CseAssert.hpp"  

namespace cse {

#ifndef NDEBUG

class String : public std::string {
 public:
  // Inherit std::string's constructors
  String() : std::string() {}
  String(const std::string &other) : std::string(other) {}

  // Assignment operator
  String &operator=(const std::string &other) {
    std::string::operator=(other);
    return *this;
  }

  // Overloaded operator[] (non-const) with debug checks
  char &operator[](std::size_t pos) {
    dbg_assert(pos < size(), "cse::String index out of range");
    return std::string::operator[](pos);
  }

  // Overloaded operator[] (const) with debug checks
  const char &operator[](std::size_t pos) const {
    dbg_assert(pos < size(), "cse::String index out of range");
    return std::string::operator[](pos);
  }

  // NEW: at() with debug checks
  char &at(std::size_t pos) {
    dbg_assert(pos < size(), "cse::String at() index out of range");
    return std::string::at(pos);  // std::string::at() also throws std::out_of_range
  }

  // NEW: const at() with debug checks
  const char &at(std::size_t pos) const {
    dbg_assert(pos < size(), "cse::String at() index out of range");
    return std::string::at(pos);
  }

  // NEW: substr() with debug checks
  cse::String substr(std::size_t pos, std::size_t count = npos) const {
    dbg_assert(pos <= size(), "cse::String substr() start position out of range");
    // Use base-class std::string::substr, then wrap the result in cse::String
    return cse::String(std::string::substr(pos, count));
  }
};

#else

// In Release builds, String is just an alias to std::string
using String = std::string;

#endif  // NDEBUG

}  // namespace cse
