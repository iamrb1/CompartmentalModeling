// app/web/Bindings.hpp
/**
 * @file Bindings.hpp
 * @brief Emscripten bindings for RichTextState, disambiguating overloads.
 */
#pragma once

#include <emscripten/bind.h>

#include <cstddef>
#include <string>
#include <vector>

#include "RichTextState.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(rich_text_module) {
  class_<RichTextState>("RichTextState")
      .constructor<>()

      // Text editing
      .function("edit_start_pos", &RichTextState::edit_start_pos)
      .function("edit_start_range", &RichTextState::edit_start_range)
      .function("edit_change", &RichTextState::edit_change)
      .function("setText", &RichTextState::set_text)
      .function("getText", &RichTextState::get_text)

      // Single‑arg formatting
      .function("applyBold", static_cast<void (RichTextState::*)()>(
                                 &RichTextState::apply_bold))
      .function("applyItalic", static_cast<void (RichTextState::*)()>(
                                   &RichTextState::apply_italic))
      .function("applyUnderline", static_cast<void (RichTextState::*)()>(
                                      &RichTextState::apply_underline))
      .function("applyStrikethrough", static_cast<void (RichTextState::*)()>(
                                          &RichTextState::apply_strikethrough))
      .function("applyColor",
                static_cast<void (RichTextState::*)(const std::string&)>(
                    &RichTextState::apply_color))
      .function("applyLink",
                static_cast<void (RichTextState::*)(const std::string&)>(
                    &RichTextState::apply_link))
      .function("applyHeader", static_cast<void (RichTextState::*)(int)>(
                                   &RichTextState::apply_header))

      // Generic format functions
      .function("set_monostate",
                static_cast<void (RichTextState::*)(cse::TextFormat::FormatID)>(
                    &RichTextState::set_monostate))

      .function("set_string", static_cast<void (RichTextState::*)(
                                  cse::TextFormat::FormatID, std::string)>(
                                  &RichTextState::set_string))

      // Clear formats
      .function("unset_format", &RichTextState::unset_format)

      .function("clear_all_formats", &RichTextState::clear_all_formats)

      // Get format data (for display on top bar)
      .function("format_ids", &RichTextState::format_ids)
      .function("format_data", &RichTextState::format_data)
      .function("has_format", &RichTextState::has_format)

      // Range‑based formatting (distinct JS names)
      .function("applyBoldRange",
                static_cast<void (RichTextState::*)(std::size_t, std::size_t)>(
                    &RichTextState::apply_bold))
      .function("applyItalicRange",
                static_cast<void (RichTextState::*)(std::size_t, std::size_t)>(
                    &RichTextState::apply_italic))
      .function("applyUnderlineRange",
                static_cast<void (RichTextState::*)(std::size_t, std::size_t)>(
                    &RichTextState::apply_underline))
      .function("applyStrikeRange",
                static_cast<void (RichTextState::*)(std::size_t, std::size_t)>(
                    &RichTextState::apply_strikethrough))
      .function("applyColorRange",
                static_cast<void (RichTextState::*)(const std::string&,
                                                    std::size_t, std::size_t)>(
                    &RichTextState::apply_color))
      .function("applyLinkRange",
                static_cast<void (RichTextState::*)(const std::string&,
                                                    std::size_t, std::size_t)>(
                    &RichTextState::apply_link))
      .function(
          "applyHeaderRange",
          static_cast<void (RichTextState::*)(int, std::size_t, std::size_t)>(
              &RichTextState::apply_header))

      // Output serialization
      .function("output_formats", &RichTextState::output_formats)
      .function("output", &RichTextState::output)
      .function("getHTML", &RichTextState::get_html)
      .function("getRawHTML", &RichTextState::get_raw_html)
      .function("getMarkdown", &RichTextState::get_markdown)
      .function("getRTF", &RichTextState::get_rtf)
      .function("getLaTeX", &RichTextState::get_LaTeX)

      // Undo / Redo
      .function("undo", &RichTextState::undo)
      .function("redo", &RichTextState::redo);

  // Expose vectors
  register_vector<std::vector<std::string>>("VVString");
  register_vector<std::string>("VString");
}
