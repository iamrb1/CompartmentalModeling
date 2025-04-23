/**
 * @file SerializerHTML.hpp
 * @author Kyle
 */
#pragma once

#include <concepts>
#include <format>
#include <string>

#include "RichText.hpp"
#include "SerializerCommon.hpp"

namespace cse {

const std::string SERIALIZER_HTML_HEADER =
    "<!DOCTYPE html>\n<html>\n<head></head>\n<body>\n";
const std::string SERIALIZER_HTML_FOOTER = "\n</body>\n</html>\n";

// Generate an HTML serializer for the given character type and underlying
// string-like
template <typename CharT = char, typename Underlying = std::basic_string<CharT>>
  requires std::derived_from<Underlying, std::basic_string<CharT>>
auto SerializerHTML() {
  struct BasicRichText<CharT, Underlying>::Serializer out{"HTML"};
  out.header = str_to_underlying<CharT, Underlying>(SERIALIZER_HTML_HEADER);
  out.footer = str_to_underlying<CharT, Underlying>(SERIALIZER_HTML_FOOTER);

  // Add all formating rules
  out.AddRule("bold", str_to_underlying<CharT, Underlying>("<b>"),
              str_to_underlying<CharT, Underlying>("</b>"));

  out.AddRule("italic", str_to_underlying<CharT, Underlying>("<i>"),
              str_to_underlying<CharT, Underlying>("</i>"));

  out.AddRule("underline", str_to_underlying<CharT, Underlying>("<u>"),
              str_to_underlying<CharT, Underlying>("</u>"));

  out.AddRule("strikethrough", str_to_underlying<CharT, Underlying>("<s>"),
              str_to_underlying<CharT, Underlying>("</s>"));

  out.AddRule(
      "color",
      [](const TextFormat& f) -> Underlying {
        cse_assert(std::holds_alternative<std::string>(f.metadata));
        return str_to_underlying<CharT, Underlying>(std::format(
            "<span style=\"color: {};\">", std::get<std::string>(f.metadata)));
      },
      str_to_underlying<CharT, Underlying>("</span>"));

  out.AddRule(
      "link",
      [](const TextFormat& f) -> Underlying {
        cse_assert(std::holds_alternative<std::string>(f.metadata));
        return str_to_underlying<CharT, Underlying>(
            std::format("<a href=\"{}\">", std::get<std::string>(f.metadata)));
      },
      str_to_underlying<CharT, Underlying>("</a>"));

  return out;
}
};  // namespace cse
