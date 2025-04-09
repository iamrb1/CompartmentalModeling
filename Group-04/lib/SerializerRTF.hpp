/**
 * @file SerializerRTF.hpp
 * @author Group-04
 */
#pragma once

#include <concepts>
#include <format>
#include <string>

#include "RichText.hpp"
#include "SerializerCommon.hpp"

namespace cse {

// RTF serializer template for RichText
template <typename CharT = char, typename Underlying = std::basic_string<CharT>>
  requires std::derived_from<Underlying, std::basic_string<CharT>>
auto SerializerRTF() {
  struct BasicRichText<CharT, Underlying>::Serializer out{"RTF"};
  
  // RTF document structure
  out.header = str_to_underlying<CharT, Underlying>(
      "{\\rtf1\\ansi\\deff0\\deflang1033\n"
      "{\\fonttbl{\\f0\\fswiss\\fcharset0 Arial;}}\n"
      "{\\colortbl;\\red0\\green0\\blue0;\\red255\\green0\\blue0;\\red0\\green0\\blue255;}\n");
  
  out.footer = str_to_underlying<CharT, Underlying>("}\n");

  // Format rules
  out.AddRule("bold", str_to_underlying<CharT, Underlying>("{\\b "),
              str_to_underlying<CharT, Underlying>(" }"));

  out.AddRule("italic", str_to_underlying<CharT, Underlying>("{\\i "),
              str_to_underlying<CharT, Underlying>(" }"));

  out.AddRule(
      "color",
      [](const TextFormat& f) -> Underlying {
        cse_assert(std::holds_alternative<std::string>(f.metadata));
        return str_to_underlying<CharT, Underlying>("{\\cf2 ");
      },
      str_to_underlying<CharT, Underlying>(" }"));

  out.AddRule(
      "link",
      [](const TextFormat& f) -> Underlying {
        cse_assert(std::holds_alternative<std::string>(f.metadata));
        std::string url = std::get<std::string>(f.metadata);
        std::string field = "{\\field{\\*\\fldinst HYPERLINK \"" + url + "\"}{\\fldrslt ";
        return str_to_underlying<CharT, Underlying>(field);
      },
      str_to_underlying<CharT, Underlying>("}}"));

  return out;
}
};  // namespace cse 