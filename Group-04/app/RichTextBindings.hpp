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
}
