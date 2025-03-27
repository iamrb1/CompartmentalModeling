/**
 * @file SerializerMarkdown.hpp
 * @author Kyle
 */
#pragma once

#include <concepts>
#include <format>
#include <string>

#include "RichText.hpp"
#include "SerializerCommon.hpp"

namespace cse {

// Generate an HTML serializer for the given character type and underlying
// string-like
template <typename CharT = char, typename Underlying = std::basic_string<CharT>>
  requires std::derived_from<Underlying, std::basic_string<CharT>>
auto SerializerMarkdown() {
  struct BasicRichText<CharT, Underlying>::Serializer out{"Markdown"};

  // Add all formating rules
  out.AddRule("bold", str_to_underlying<CharT, Underlying>("**"),
              str_to_underlying<CharT, Underlying>("**"));

  out.AddRule("italic", str_to_underlying<CharT, Underlying>("_"),
              str_to_underlying<CharT, Underlying>("_"));

  // Is there a universal color serialization model for Markdown?
  /*
  out.AddRule("color",
      [](const TextFormat& f) -> Underlying {
        cse_assert(std::holds_alternative<std::string>(f.metadata));
        return str_to_underlying<CharT, Underlying>(
          std::format("<span style=\"color: {};\">",
  std::get<std::string>(f.metadata))
        );
      },
      str_to_underlying<CharT, Underlying>("</span>"));
  */

  out.AddRule("link", str_to_underlying<CharT, Underlying>("["),
              [](const TextFormat& f) -> Underlying {
                cse_assert(std::holds_alternative<std::string>(f.metadata));
                return str_to_underlying<CharT, Underlying>(
                    std::format("]({})", std::get<std::string>(f.metadata)));
              });

  return out;
}
};  // namespace cse
