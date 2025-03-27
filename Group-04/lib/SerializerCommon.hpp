/**
 * @file SerializerCommon.hpp
 * @author Kyle
 */
#pragma once

#include <concepts>
#include <string>

namespace cse {

namespace {
// Generate an underlying from a character string
template <typename CharT, typename Underlying>
  requires std::derived_from<Underlying, std::basic_string<CharT>>
Underlying str_to_underlying(std::string str) {
  Underlying out;
  for (const auto c : str) {
    out.insert(out.end(), static_cast<CharT>(c));
  }
  return out;
}
};  // namespace

};  // namespace cse
