// app/web/RichTextState.hpp
/**
 * @file RichTextState.hpp
 * @author Krish, Kyle
 * @brief Manages a RichText instance, editing operations, serialization, and
 *        provides a C++ API for tests.
 */
#pragma once

#include <unordered_map>
#include <optional>
#include <string>
#include <vector>

#include "CseString.hpp"
#include "RichText.hpp"
#include "SerializerHTML.hpp"
#include "SerializerLaTeX.hpp"
#include "SerializerMarkdown.hpp"
#include "SerializerRTF.hpp"

class RichTextState {
 public:
  using Text = cse::BasicRichText<char, cse::String>;

  RichTextState() {
    // HTML (no header/footer)
    m_output_map["html"] = {"HTML", cse::SerializerHTML<char, cse::String>()};
    m_output_map["html"].second.header = std::nullopt;
    m_output_map["html"].second.footer = std::nullopt;

    // RTF
    m_output_map["rtf"] = {"RTF", cse::SerializerRTF<char, cse::String>()};
    // Markdown
    m_output_map["markdown"] = {"Markdown",
                                cse::SerializerMarkdown<char, cse::String>()};
    // LaTeX
    m_output_map["latex"] = {"LaTeX",
                             cse::SerializerLaTeX<char, cse::String>()};
    // Raw HTML (full document)
    m_output_map["html_raw"] = {"Raw HTML",
                                cse::SerializerHTML<char, cse::String>()};

    // Demo: bold the first 5 characters
    m_text.apply_format(cse::TextFormat("bold", std::monostate()), 0, 5);
  }

  // — BASIC TEXT OPERATIONS —
  std::string edit_start_pos(size_t idx) {
    if (m_text.size() == 0) {
      m_edit = {0, 0};
      m_format_map.clear();
      for (auto& fmt : m_text.formats_at(idx))
        m_format_map[fmt.name] = fmt.metadata;
      return "";
    }
    if (idx >= m_text.size()) idx = m_text.size() - 1;

    m_format_map.clear();
    for (auto& fmt : m_text.formats_at(idx))
      m_format_map[fmt.name] = fmt.metadata;

    auto seg = m_text.segment_at(idx);
    m_edit = {seg.start, seg.end};
    return m_text.to_string().substr(m_edit.first, m_edit.second);
  }

  std::string edit_start_range(size_t start, size_t count) {
    m_format_map.clear();
    for (auto& fmt : m_text.formats_in_range(start, count + start))
      m_format_map[fmt.name] = fmt.metadata;

    if (start >= m_text.size() || start + count > m_text.size()) return "";
    m_edit = {start, start + count};
    return m_text.to_string().substr(start, count);
  }

  void edit_change(std::string replace) {
    // Save current state before making changes
    save_state();

    if (m_edit.first < m_text.size())
      m_text.erase(m_edit.first, m_edit.second - m_edit.first);

    m_edit.second = m_edit.first + replace.size();
    Text insert{replace};
    for (auto& [id, meta] : m_format_map)
      insert.apply_format(cse::TextFormat(id, meta));
    m_text.insert(m_edit.first, insert);
  }

  // — FORMAT OPERATIONS —
  void set_format(cse::TextFormat::FormatID id,
                  cse::TextFormat::FormatData data) {
    // Save current state before making changes
    save_state();

    cse::TextFormat fmt{id, data};
    if (m_edit.second > m_edit.first)
      m_text.apply_format(fmt, m_edit.first, m_edit.second);
    m_format_map[id] = data;
  }

  void set_monostate(cse::TextFormat::FormatID id) {
    set_format(id, std::monostate());
  }

  void set_string(cse::TextFormat::FormatID id, std::string value) {
    set_format(id, value);
  }

  void unset_format(cse::TextFormat::FormatID id) {
    if (m_format_map.contains(id)) {
      save_state();
      m_format_map.erase(id);
      m_text.clear_format(id, {{m_edit.first, m_edit.second}});
    }
  }

  void clear_all_formats() {
    if (!m_format_map.empty()) {
      save_state();
      for (const auto& [id, data] : m_format_map) {
        m_text.clear_format(id, {{m_edit.first, m_edit.second}});
      }
      m_format_map.clear();
    }
  }

  std::vector<std::string> format_ids() const {
    std::vector<std::string> out;

    for (const auto& [id, _] : m_format_map) {
      out.push_back(id);
    }

    return out;
  }

  std::string format_data(cse::TextFormat::FormatID id) const {
    if (m_format_map.contains(id)) {
      auto data = m_format_map.at(id);
      if (std::holds_alternative<std::monostate>(data))
        return "1";
      else if (std::holds_alternative<int32_t>(data))
        return std::to_string(std::get<int32_t>(data));
      else if (std::holds_alternative<std::string>(data))
        return std::get<std::string>(data);
    }
    return "";
  }

  bool has_format(cse::TextFormat::FormatID id) const {
    return m_format_map.contains(id);
  }

  // — UNDO / REDO —
  void undo() {
    if (!undo_stack.empty()) {
      redo_stack.push_back(m_text);
      m_text = undo_stack.back();
      undo_stack.pop_back();
    }
  }

  void redo() {
    if (!redo_stack.empty()) {
      undo_stack.push_back(m_text);
      m_text = redo_stack.back();
      redo_stack.pop_back();
    }
  }

  // — OUTPUT —
  std::vector<std::vector<std::string>> output_formats() const {
    std::vector<std::vector<std::string>> out;
    for (auto& [key, p] : m_output_map) out.push_back({key, p.first});
    return out;
  }

  std::string output(std::string fmt) {
    if (m_output_map.contains(fmt)) {
      auto res = m_text.serialize(m_output_map[fmt].second);
      return res.output;
    }
    return m_text.to_string();
  }

  // Raw text
  std::string get_text() const { return m_text.to_string(); }
  void set_text(const std::string& txt) {
    save_state();
    m_text = Text(txt);
  }

  // Serialization shortcuts
  std::string get_html() { return output("html"); }
  std::string get_raw_html() { return output("html_raw"); }
  std::string get_markdown() { return output("markdown"); }
  std::string get_rtf() { return output("rtf"); }
  std::string get_LaTeX() { return output("latex"); }

  // Single‑arg convenience formatting
  void apply_bold() { set_format("bold", std::monostate{}); }
  void apply_italic() { set_format("italic", std::monostate{}); }
  void apply_underline() { set_format("underline", std::monostate{}); }
  void apply_strikethrough() { set_format("strikethrough", std::monostate{}); }
  void apply_color(const std::string& c) { set_format("color", c); }
  void apply_link(const std::string& u) { set_format("link", u); }
  void apply_header(int lvl) { set_format("header", lvl); }

  // Range‑based overloads (the tests call these with start,end)
  void apply_bold(size_t start, size_t end) {
    m_text.apply_format(cse::TextFormat("bold", std::monostate()), start, end);
  }
  void apply_italic(size_t start, size_t end) {
    m_text.apply_format(cse::TextFormat("italic", std::monostate()), start,
                        end);
  }
  void apply_underline(size_t start, size_t end) {
    m_text.apply_format(cse::TextFormat("underline", std::monostate()), start,
                        end);
  }
  void apply_strikethrough(size_t start, size_t end) {
    m_text.apply_format(cse::TextFormat("strikethrough", std::monostate()),
                        start, end);
  }
  void apply_color(const std::string& c, size_t start, size_t end) {
    m_text.apply_format(cse::TextFormat("color", c), start, end);
  }
  void apply_link(const std::string& u, size_t start, size_t end) {
    m_text.apply_format(cse::TextFormat("link", u), start, end);
  }
  void apply_header(int lvl, size_t start, size_t end) {
    m_text.apply_format(cse::TextFormat("header", lvl), start, end);
  }

 private:
  Text m_text{"Hello World!"};
  std::unordered_map<cse::TextFormat::FormatID, cse::TextFormat::FormatData> m_format_map;
  std::unordered_map<std::string, std::pair<std::string, Text::Serializer>> m_output_map;
  std::pair<size_t, size_t> m_edit{0, 0};
  std::vector<Text> undo_stack, redo_stack;

  void save_state() {
    undo_stack.push_back(m_text);
    redo_stack.clear();
  }
};
