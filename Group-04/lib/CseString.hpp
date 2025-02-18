/**
 * @file CseString.h
 * @author Krishna (Krish)
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

  String &operator=(const std::string &other) {
    std::string::operator=(other);
    return *this;
  }

  char &operator[](std::size_t pos) {
    // Range check only in debug mode
    dbg_assert(pos < size(), "cse::String index out of range");
    return std::string::operator[](pos);
  }

  const char &operator[](std::size_t pos) const {
    // Range check only in debug mode
    dbg_assert(pos < size(), "cse::String index out of range");
    return std::string::operator[](pos);
  }
};

#else

// In Release builds, String is just an alias to std::string
using String = std::string;

#endif  // NDEBUG

}  // namespace cse
