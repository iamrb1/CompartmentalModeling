/**
 * @file RichTextState.hpp
 * @author Krish
 */

#pragma once

#include <string>
#include <vector>

#include "RichText.hpp"
#include "SerializerHTML.hpp"
#include "SerializerMarkdown.hpp"
#include "SerializerRTF.hpp"

class RichTextState {
 public:
  RichTextState() {
    // Initialize with some default text.
    m_richText = cse::RichText("Hello, colorful bold world!");

    // Set up each serializer once.
    m_html = cse::SerializerHTML<>();
    m_md = cse::SerializerMarkdown<>();
    m_rtf = cse::SerializerRTF<>();

    // Minimal “raw HTML” serializer—omits <html> / <body> wrappers.
    m_rawHtml = cse::SerializerHTML();
    m_rawHtml.header = "";
    m_rawHtml.footer = "";
  }

  // -- BASIC TEXT OPERATIONS --
  void setText(const std::string &text) {
    saveState();
    m_richText = text;
    m_richText.clear_format();
  }

  std::string getText() const { return m_richText.to_string(); }

  // -- SAMPLE FORMAT APPLIERS --
  void applyBold(size_t start, size_t end) {
    saveState();
    m_richText.apply_format(cse::TextFormat("bold"), start, end);
  }

  void applyColor(const std::string &color, size_t start, size_t end) {
    saveState();
    m_richText.apply_format(cse::TextFormat("color", color), start, end);
  }

  void clearFormatting() {
    saveState();
    m_richText.clear_format();
  }

  // -- UNDO/REDO METHODS --
  void undo() {
    if (!undoStack.empty()) {
      redoStack.push_back(m_richText);
      m_richText = undoStack.back();
      undoStack.pop_back();
    }
  }

  void redo() {
    if (!redoStack.empty()) {
      undoStack.push_back(m_richText);
      m_richText = redoStack.back();
      redoStack.pop_back();
    }
  }

  // -- SERIALIZE METHODS --
  std::string getHTML() const {
    auto result = m_richText.serialize(m_html);
    return result.output;
  }

  std::string getMarkdown() const {
    auto result = m_richText.serialize(m_md);
    return result.output;
  }

  std::string getRTF() const {
    auto result = m_richText.serialize(m_rtf);
    return result.output;
  }

  std::string getRawHTML() const {
    auto result = m_richText.serialize(m_rawHtml);
    return result.output;
  }

 private:
  // "Primary" RichText instance.
  cse::RichText m_richText;

  // Serializers.
  cse::RichText::Serializer m_html;
  cse::RichText::Serializer m_md;
  cse::RichText::Serializer m_rtf;
  cse::RichText::Serializer m_rawHtml;

  // Undo/Redo stacks.
  std::vector<cse::RichText> undoStack;
  std::vector<cse::RichText> redoStack;

  // Save a copy of the current state and clear the redo stack.
  void saveState() {
    undoStack.push_back(m_richText);
    redoStack.clear();
  }
};
