/**
 * @file SerializerHTML.hpp
 * @author Kyle
 */
#pragma once

#include <string>
#include <concepts>
#include <format>

#include "RichText.hpp"
#include "SerializerCommon.hpp"

namespace cse {
  
// Generate an HTML serializer for the given character type and underlying string-like
template<typename CharT = char, typename Underlying = std::basic_string<CharT>>
  requires std::derived_from<Underlying, std::basic_string<CharT>>
auto SerializerHTML() {
  struct BasicRichText<CharT, Underlying>::Serializer out{"HTML"};
  out.header = str_to_underlying<CharT, Underlying>("<!DOCTYPE html>\n<html>\n<head></head>\n<body>\n");
  out.footer = str_to_underlying<CharT, Underlying>("\n</body>\n</html>\n");

  // Add all formating rules
  out.AddRule("bold",
      str_to_underlying<CharT, Underlying>("<b>"),
      str_to_underlying<CharT, Underlying>("</b>"));

  out.AddRule("italic",
      str_to_underlying<CharT, Underlying>("<i>"),
      str_to_underlying<CharT, Underlying>("</i>"));

  out.AddRule("color",
      [](const TextFormat& f) -> Underlying {
        cse_assert(std::holds_alternative<std::string>(f.metadata));
        return str_to_underlying<CharT, Underlying>(
          std::format("<span style=\"color: {};\">", std::get<std::string>(f.metadata))
        );
      },
      str_to_underlying<CharT, Underlying>("</span>"));

  out.AddRule("link",
      [](const TextFormat& f) -> Underlying {
        cse_assert(std::holds_alternative<std::string>(f.metadata));
        return str_to_underlying<CharT, Underlying>(
          std::format("<a href=\"{}\">", std::get<std::string>(f.metadata))
        );
      },
      str_to_underlying<CharT, Underlying>("</a>"));

  return out;
}
};

