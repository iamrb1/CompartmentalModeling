/**
 * @file RichTextBindings.hpp
 * @author Krish
 */

#pragma once

#include <emscripten/bind.h>

#include "RichTextState.hpp"
#include "EditorState.hpp"

using namespace emscripten;
using namespace cse;

EMSCRIPTEN_BINDINGS(rich_text_module) {
  // Bind the RichTextState class so that JS can create and interact with it.
  class_<RichTextState>("RichTextState")
      .constructor<>()

      // Basic text manipulation.
      .function("setText", &RichTextState::setText)
      .function("getText", &RichTextState::getText)
      .function("clearFormatting", &RichTextState::clearFormatting)
      .function("applyBold", &RichTextState::applyBold)
      .function("applyColor", &RichTextState::applyColor)

      // Undo/Redo functions.
      .function("undo", &RichTextState::undo)
      .function("redo", &RichTextState::redo)

      // Serialization methods.
      .function("getHTML", &RichTextState::getHTML)
      .function("getMarkdown", &RichTextState::getMarkdown)
      .function("getRTF", &RichTextState::getRTF)
      .function("getRawHTML", &RichTextState::getRawHTML);

  class_<EditorState>("EditorState")
    .constructor<>()
    .function("edit_start_pos", &EditorState::edit_start_pos)
    .function("edit_start_range", &EditorState::edit_start_range)
    .function("edit_change", &EditorState::edit_start_pos)
    
    .function("output_formats", &EditorState::output_formats)
    .function("output", &EditorState::output);

  register_vector<std::vector<std::string>>("VVString");
  register_vector<std::string>("VString");
}
