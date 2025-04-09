/**
 * @file RichTextBindings.hpp
 * @author Krish
 */

#include <emscripten/bind.h>

#include "RichText.hpp"
#include "SerializerHTML.hpp"

using namespace emscripten;
using namespace cse;

std::string renderRichTextHTML() {
  RichText rt("Hello, colorful bold world!");

  // Apply some formatting
  rt.apply_format(TextFormat("bold"), 16, 20);  // "bold"
  rt.apply_format(TextFormat("color", std::string("red")), 7,
                  15);  // "colorful"

  // Serialize using HTML serializer
  auto result = rt.serialize(SerializerHTML<>());
  return result.output;
}

EMSCRIPTEN_BINDINGS(rich_text_module) {
  function("renderRichTextHTML", &renderRichTextHTML);
}
