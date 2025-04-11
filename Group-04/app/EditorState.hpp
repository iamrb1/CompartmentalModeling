#pragma once

#include <RichText.hpp>
#include <SerializerHTML.hpp>
#include <SerializerMarkdown.hpp>
#include <SerializerRTF.hpp>
#include <map>
#include <vector>

class EditorState {
 private:
  // The underlying text content
  cse::RichText underlying = cse::RichText("Hello World!");
  // The curently selected formats
  std::map<cse::TextFormat::FormatID, cse::TextFormat::FormatData> format_map;
  // The possible output formats
  std::map<std::string, std::pair<std::string, cse::RichText::Serializer>> output_map;

  // Start and end of the current editing range.  If the values are negative
  // Then we are not changing anything
  int edit_start = -1, edit_end = -1;

 public:
  EditorState() {
    output_map["html"] = std::pair("HTML", cse::SerializerHTML());
    output_map["html"].second.header = std::nullopt;
    output_map["html"].second.footer = std::nullopt;
    output_map["rtf"] = std::pair("RTF", cse::SerializerRTF());
    output_map["markdown"] = std::pair("Markdown", cse::SerializerMarkdown());

    // Raw HTML serializer (document)
    output_map["html_raw"] = std::pair("Raw HTML", cse::SerializerHTML());

    underlying.apply_format(cse::TextFormat("bold", std::monostate()), 0, 5);
  }

  std::string edit_start_pos(size_t idx) {
    if (idx >= underlying.size())
      return "";

    cse::IndexSet segment = underlying.segment_at(idx);
    edit_start = segment.min_index().value_or(0);
    edit_end = segment.max_index().value_or(underlying.size());

    reset_format();
    for (const auto& format : underlying.formats_at(idx)) {
      update_bar(format);
    }

    return underlying.to_string().substr(edit_start, edit_end);
  }

  std::string edit_start_range(size_t start, size_t end) {
    if (start >= underlying.size() || end >= underlying.size())
      return "";
    edit_start = start;
    edit_end = end;
    return underlying.to_string().substr(edit_start, edit_end);
  }

  void edit_change(std::string replace) {}

  void update_format(cse::TextFormat::FormatID id,
                     cse::TextFormat::FormatData data) {
    cse::TextFormat format(id, data);
    if (edit_end - edit_start > 0) {
      // We are editing something
      underlying.apply_format(format, edit_start, edit_end);
    }

    update_bar(format);
  }

  // Reset all formats in the bar and in the format_map
  void reset_format() {
    for (const auto& [name, _] : format_map) {
      update_bar(cse::TextFormat(name, std::monostate()));
    }
    format_map.clear();
  }

  // Update the front end to show that we have set new formats
  // if we are clearing a format, set clear to true
  void update_bar(const cse::TextFormat& format, bool clear = false) {
    if (!clear)
      format_map[format.name] = format.metadata;
    else
      format_map.erase(format.name);
    // TODO: update graphics on front end
  }

  // Get all possible output formats (string names)
  std::vector<std::vector<std::string>> output_formats() const {
    std::vector<std::vector<std::string>> out;
    for (auto [name, p] : output_map) {
      out.push_back({name, p.first});
    }
    return out;
  }

  // Get the output as a string (by the output format name)
  std::string output(std::string output_format) {
    if (output_map.contains(output_format)) {
      auto result = underlying.serialize(output_map[output_format].second);
      return result.output;
    }

    return underlying.to_string();
  }
};
