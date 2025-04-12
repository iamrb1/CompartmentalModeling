/**
 * @file RichTextBindings.hpp
 * @author Krish
 */

#pragma once

#include <emscripten/bind.h>

#include "RichTextState.hpp"

using namespace emscripten;
using namespace cse;

EMSCRIPTEN_BINDINGS(rich_text_module) {
  // Bind the RichTextState class so that JS can create and interact with it.
  class_<RichTextState>("RichTextState")
      .constructor<>()
      // Text editing
      .function("edit_start_pos", &RichTextState::edit_start_pos)
      .function("edit_start_range", &RichTextState::edit_start_range)
      .function("edit_change", &RichTextState::edit_start_pos)

      // Output mapping
      .function("output_formats", &RichTextState::output_formats)
      .function("output", &RichTextState::output)

      // Undo/Redo functions.
      .function("undo", &RichTextState::undo)
      .function("redo", &RichTextState::redo);

  register_vector<std::vector<std::string>>("VVString");
  register_vector<std::string>("VString");
}
