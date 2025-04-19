// app/web/RichTextState.hpp
/**
 * @file RichTextState.hpp
 * @author Krish, Kyle
 * @brief Manages a RichText instance, editing operations, serialization, and
 *        provides a C++ API for tests.
 */
#pragma once

#include "RichText.hpp"
#include "SerializerHTML.hpp"
#include "SerializerMarkdown.hpp"
#include "SerializerRTF.hpp"
#include "SerializerLaTeX.hpp"

#include <map>
#include <optional>
#include <string>
#include <vector>

class RichTextState {
public:
  RichTextState() {
    // HTML (no header/footer)
    m_outputMap["html"]     = { "HTML",      cse::SerializerHTML()   };
    m_outputMap["html"].second.header = std::nullopt;
    m_outputMap["html"].second.footer = std::nullopt;

    // RTF
    m_outputMap["rtf"]      = { "RTF",       cse::SerializerRTF()    };
    // Markdown
    m_outputMap["markdown"] = { "Markdown",  cse::SerializerMarkdown() };
    // LaTeX
    m_outputMap["latex"]    = { "LaTeX",     cse::SerializerLaTeX()  };
    // Raw HTML (full document)
    m_outputMap["html_raw"] = { "Raw HTML",  cse::SerializerHTML()   };

    // Demo: bold the first 5 characters
    m_richText.apply_format(cse::TextFormat("bold", std::monostate()), 0, 5);
  }

  // — BASIC TEXT OPERATIONS —
  std::string edit_start_pos(size_t idx) {
    if (m_richText.size() == 0) {
      m_edit = {0,0};
      return "";
    }
    if (idx >= m_richText.size()) idx = m_richText.size() - 1;

    reset_format();
    for (auto& fmt : m_richText.formats_at(idx))
      update_bar(fmt);

    auto seg = m_richText.segment_at(idx);
    m_edit = { seg.start, seg.end };
    return m_richText.to_string().substr(m_edit.first, m_edit.second);
  }

  std::string edit_start_range(size_t start, size_t count) {
    if (start >= m_richText.size() || start + count > m_richText.size())
      return "";
    m_edit = { start, start + count };
    return m_richText.to_string().substr(start, count);
  }

  void edit_change(std::string replace) {
    // Save current state before making changes
    saveState();
    
    if (m_edit.first < m_richText.size())
      m_richText.erase(m_edit.first, m_edit.second - m_edit.first);

    m_edit.second = m_edit.first + replace.size();
    cse::RichText insert{ replace };
    for (auto& [id, meta] : m_formatMap)
      insert.apply_format(cse::TextFormat(id, meta));
    m_richText.insert(m_edit.first, insert);
  }

  // — FORMAT OPERATIONS —
  void update_format(cse::TextFormat::FormatID id,
                     cse::TextFormat::FormatData data) {
    // Save current state before making changes
    saveState();
    
    cse::TextFormat fmt{id, data};
    if (m_edit.second > m_edit.first)
      m_richText.apply_format(fmt, m_edit.first, m_edit.second);
    update_bar(fmt);
  }

  void reset_format() {
    for (auto& [name,_] : m_formatMap)
      update_bar(cse::TextFormat(name, std::monostate()), true);
    m_formatMap.clear();
  }

  void update_bar(const cse::TextFormat& fmt, bool clear = false) {
    if (!clear) m_formatMap[fmt.name] = fmt.metadata;
    // TODO: reflect toolbar UI
  }

  // — UNDO / REDO —
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

  // — OUTPUT —
  std::vector<std::vector<std::string>> output_formats() const {
    std::vector<std::vector<std::string>> out;
    for (auto& [key,p] : m_outputMap)
      out.push_back({ key, p.first });
    return out;
  }

  std::string output(std::string fmt) {
    if (m_outputMap.contains(fmt)) {
      auto res = m_richText.serialize(m_outputMap[fmt].second);
      return res.output;
    }
    return m_richText.to_string();
  }

  // — TEST‑COMPATIBILITY API —

  // Raw text
  std::string getText() const {
    return m_richText.to_string();
  }
  void setText(const std::string& txt) {
    m_richText = cse::RichText(txt);
  }

  // Serialization shortcuts
  std::string getHTML()     { return output("html"); }
  std::string getRawHTML()  { return output("html_raw"); }
  std::string getMarkdown() { return output("markdown"); }
  std::string getRTF()      { return output("rtf"); }
  std::string getLaTeX()    { return output("latex"); }

  // Single‑arg convenience formatting
  void applyBold()          { update_format("bold", std::monostate{}); }
  void applyItalic()        { update_format("italic", std::monostate{}); }
  void applyUnderline()     { update_format("underline", std::monostate{}); }
  void applyStrikethrough() { update_format("strikethrough", std::monostate{}); }
  void applyColor(const std::string& c) { update_format("color", c); }
  void applyLink(const std::string& u)  { update_format("link", u); }
  void applyHeader(int lvl)             { update_format("header", lvl); }

  // Range‑based overloads (the tests call these with start,end)
  void applyBold(size_t start, size_t end) {
    m_richText.apply_format(cse::TextFormat("bold", std::monostate()), start, end);
  }
  void applyItalic(size_t start, size_t end) {
    m_richText.apply_format(cse::TextFormat("italic", std::monostate()), start, end);
  }
  void applyUnderline(size_t start, size_t end) {
    m_richText.apply_format(cse::TextFormat("underline", std::monostate()), start, end);
  }
  void applyStrikethrough(size_t start, size_t end) {
    m_richText.apply_format(cse::TextFormat("strikethrough", std::monostate()), start, end);
  }
  void applyColor(const std::string& c, size_t start, size_t end) {
    m_richText.apply_format(cse::TextFormat("color", c), start, end);
  }
  void applyLink(const std::string& u, size_t start, size_t end) {
    m_richText.apply_format(cse::TextFormat("link", u), start, end);
  }
  void applyHeader(int lvl, size_t start, size_t end) {
    m_richText.apply_format(cse::TextFormat("header", lvl), start, end);
  }

private:
  cse::RichText m_richText{"Hello World!"};
  std::map<cse::TextFormat::FormatID, cse::TextFormat::FormatData> m_formatMap;
  std::map<std::string, std::pair<std::string, cse::RichText::Serializer>> m_outputMap;
  std::pair<size_t,size_t> m_edit{0,0};
  std::vector<cse::RichText> undoStack, redoStack;

  void saveState() {
    undoStack.push_back(m_richText);
    redoStack.clear();
  }
};
