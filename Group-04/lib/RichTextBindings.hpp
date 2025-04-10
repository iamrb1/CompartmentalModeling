/**
 * @file RichTextBindings.hpp
 * @author Krish
 */

#include <emscripten/bind.h>

#include "RichText.hpp"
#include "SerializerHTML.hpp"
#include "SerializerMarkdown.hpp"
#include "SerializerRTF.hpp"

using namespace emscripten;
using namespace cse;

/**
 * @brief Returns a full HTML document (doctype, <html>, <body>, etc.).
 */
std::string renderRichTextHTML() {
  RichText rt("Hello, colorful bold world!");

  // Apply some formatting
  rt.apply_format(TextFormat("bold"), 16, 20);             // "bold"
  rt.apply_format(TextFormat("color", std::string("red")), 7, 15); // "colorful"

  // Serialize using HTML serializer
  auto result = rt.serialize(SerializerHTML<>());
  return result.output;
}

/**
 * @brief Returns a Markdown string for the same example text.
 */
std::string renderRichTextMarkdown() {
  RichText rt("Hello, colorful bold world!");
  rt.apply_format(TextFormat("bold"), 16, 20);
  rt.apply_format(TextFormat("color", std::string("red")), 7, 15);

  // Serialize using Markdown serializer
  auto result = rt.serialize(SerializerMarkdown<>());
  return result.output;
}

/**
 * @brief Returns an RTF string for the same example text.
 */
std::string renderRichTextRTF() {
  RichText rt("Hello, colorful bold world!");
  rt.apply_format(TextFormat("bold"), 16, 20);
  rt.apply_format(TextFormat("color", std::string("red")), 7, 15);

  // Serialize using RTF serializer
  auto result = rt.serialize(SerializerRTF<>());
  return result.output;
}

/**
 * @brief Returns raw HTML *without* <html> / <body> wrappers (so we can show it literally).
 */
std::string renderRichTextRawHTML() {
  RichText rt("Hello, colorful bold world!");
  rt.apply_format(TextFormat("bold"), 16, 20);
  rt.apply_format(TextFormat("color", std::string("red")), 7, 15);

  // Build a minimal serializer that omits doctype/head/body
  // but still uses the same “bold”, “color”, etc. tokens.
  BasicRichText<char, std::string>::Serializer rawHtmlSer{"RawHTML"};

  // Bold
  rawHtmlSer.AddRule("bold", "<b>", "</b>");
  // Italic
  rawHtmlSer.AddRule("italic", "<i>", "</i>");
  // Color
  rawHtmlSer.AddRule(
    "color",
    [](const TextFormat& f) -> std::string {
      return "<span style=\"color: " + std::get<std::string>(f.metadata) + ";\">";
    },
    "</span>"
  );
  // Link
  rawHtmlSer.AddRule(
    "link",
    [](const TextFormat& f) -> std::string {
      return "<a href=\"" + std::get<std::string>(f.metadata) + "\">";
    },
    "</a>"
  );

  // Serialize with this minimal (no header/footer) serializer
  auto result = rt.serialize(rawHtmlSer);
  return result.output;
}

EMSCRIPTEN_BINDINGS(rich_text_module) {
  function("renderRichTextHTML", &renderRichTextHTML);
  function("renderRichTextMarkdown", &renderRichTextMarkdown);
  function("renderRichTextRTF", &renderRichTextRTF);
  function("renderRichTextRawHTML", &renderRichTextRawHTML);
}
