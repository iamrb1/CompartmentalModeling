/**
 * @file RichTextState.hpp
 * @author Krish, Kyle
 */

#pragma once

#include <RichText.hpp>
#include <SerializerHTML.hpp>
#include <SerializerMarkdown.hpp>
#include <SerializerRTF.hpp>
#include <map>
#include <string>
#include <vector>

class RichTextState {
 public:
  RichTextState() {
    m_outputMap["html"] = std::pair("HTML", cse::SerializerHTML());
    m_outputMap["html"].second.header = std::nullopt;
    m_outputMap["html"].second.footer = std::nullopt;
    m_outputMap["rtf"] = std::pair("RTF", cse::SerializerRTF());
    m_outputMap["markdown"] = std::pair("Markdown", cse::SerializerMarkdown());

    // Raw HTML serializer (document)
    m_outputMap["html_raw"] = std::pair("Raw HTML", cse::SerializerHTML());

    m_richText.apply_format(cse::TextFormat("bold", std::monostate()), 0, 5);
  }

  // -- BASIC TEXT OPERATIONS --

  // Begin editing the text from the given position
  //
  std::string edit_start_pos(size_t idx) {
    if (m_richText.size() == 0) {
      m_edit = {0, 0};
      return "";
    }
    if (idx >= m_richText.size()) idx = m_richText.size() - 1;

    // Reset formatting
    reset_format();
    // We want to update our formatting with that which is in the range
    for (const auto& format : m_richText.formats_at(idx)) {
      update_bar(format);
    }

    // Get the formatting segment at the cursor
    auto segment = m_richText.segment_at(idx);
    m_edit = std::pair(segment.start, segment.end);

    return m_richText.to_string().substr(m_edit.first, m_edit.second);
  }

  // Begin editing the text in the given range
  std::string edit_start_range(size_t start, size_t count) {
    if (start >= m_richText.size() || count + start > m_richText.size())
      return "";
    m_edit = std::pair(start, start + count);
    return m_richText.to_string().substr(start, count);
  }

  // Replace the text in the range with the given string
  void edit_change(std::string replace) {
    // saveState();
    if (m_edit.first < m_richText.size()) {
      m_richText.erase(m_edit.first, m_edit.second - m_edit.first);
    }
    m_edit.second = m_edit.first + replace.length();
    cse::RichText insert = cse::RichText(replace);
    for (const auto& [id, metadata] : m_formatMap) {
      insert.apply_format(cse::TextFormat(id, metadata));
    }
    m_richText.insert(m_edit.first, insert);
  }

  // -- FORMAT OPERATIONS --

  // Set a formatting option
  void update_format(cse::TextFormat::FormatID id,
                     cse::TextFormat::FormatData data) {
    cse::TextFormat format(id, data);
    if (m_edit.second - m_edit.first > 0) {
      // We are editing something
      m_richText.apply_format(format, m_edit.first, m_edit.second);
    }

    update_bar(format);
  }

  // Reset all formats in the bar and in the format_map
  void reset_format() {
    for (const auto& [name, _] : m_formatMap) {
      update_bar(cse::TextFormat(name, std::monostate()), true);
    }
    m_formatMap.clear();
  }

  // Update the front end to show that we have set new formats
  // if we are clearing a format, set clear to true
  void update_bar(const cse::TextFormat& format, bool clear = false) {
    if (!clear) m_formatMap[format.name] = format.metadata;
    // TODO: update graphics on front end
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

  // -- OUTPUT RELATED FUNCTIONS --

  // Get all possible output formats (string names)
  std::vector<std::vector<std::string>> output_formats() const {
    std::vector<std::vector<std::string>> out;
    for (auto [name, p] : m_outputMap) {
      out.push_back({name, p.first});
    }
    return out;
  }

  // Get the output as a string (by the output format name)
  std::string output(std::string output_format) {
    if (m_outputMap.contains(output_format)) {
      auto result = m_richText.serialize(m_outputMap[output_format].second);
      return result.output;
    }

    return m_richText.to_string();
  }

 private:
  // "Primary" RichText instance.
  cse::RichText m_richText = cse::RichText("Hello World!");

  // The curently selected formats
  std::map<cse::TextFormat::FormatID, cse::TextFormat::FormatData> m_formatMap;
  // The possible output formats
  // the first element in the pair is the name of the format which should appear
  // on the UI the second element is the serializer to use when exporting
  std::map<std::string, std::pair<std::string, cse::RichText::Serializer>>
      m_outputMap;

  // The text range we are currently editing
  std::pair<std::size_t, std::size_t> m_edit = std::pair(-1, -1);

  // Undo/Redo stacks.
  std::vector<cse::RichText> undoStack;
  std::vector<cse::RichText> redoStack;

  // Save a copy of the current state and clear the redo stack.
  void saveState() {
    undoStack.push_back(m_richText);
    redoStack.clear();
  }
};
