/**
 * @file RichText.h
 * @author Grant, Kyle (Template stuff)
 *
 */

#pragma once

#include <algorithm>
#include <cstdint>
#include <format>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <ranges>

#include "CseAssert.hpp"
#include "IndexSet.hpp"

namespace cse {

using FormatID = std::string;

/**
 * A format in rich text.
 * The Format struct is specialized to represent various kinds of formats in
 * rich text. Can support simple formats like: bold, italic, strikethrough.
 * Can support more complex formats with metadata like: font size, web links.
 */
struct TextFormat {
  using FormatID = std::string;
  /**
   * The name/id of the format.
   */
  FormatID name;
  /**
   * The metadata associated with the format.
   * It may be an int to represent the size of the font, a string to represent
   * the target of a hyperlink, etc.
   */
  std::variant<std::string, int32_t, std::monostate> metadata;

  /**
   * Constructs a format with metadata
   * @param name The name of the format
   * @param metadata The metadata of the format
   */
  TextFormat(FormatID name,
             std::variant<std::string, int32_t, std::monostate> metadata)
      : name(std::move(name)), metadata(std::move(metadata)) {}

  /**
   * Constructs a simple format without metadata
   * @param name The name of the format
   */
  explicit(false) TextFormat(FormatID name)
      : name(std::move(name)), metadata(std::monostate()) {}

  /**
   * Compares the name first then the metadata
   * @param r right hand side
   * @return True if this is less than the right hand side
   */
  bool operator<(const TextFormat& r) const {
    if (name == r.name) return metadata < r.metadata;
    return name < r.name;
  }

  /**
   * Compares the name and the metadata
   * @param r right hand side
   * @return True if equivalent
   */
  bool operator==(const TextFormat& r) const {
    return name == r.name && metadata == r.metadata;
  }
};

/**
 * Stores Rich Text.
 * RichText stores and manipulates formatted rich text.
 * Provides tools to represent various formats.
 * Provides tools to serialize formatted text into various formats.
 */
template<
  typename CharT = char,
  typename Underlying = std::basic_string<CharT>
>
  requires std::derived_from<Underlying, std::basic_string<CharT>>
class RichText {
 public:

  /**
   * A rule for a format describing how it should be serialized.
   */
  struct SerializeRule {
    // No format name here, that is stored in the map as a key
    std::string start_token;
    std::string end_token;
    std::variant<std::string, std::function<std::string(const TextFormat&)>> format;
  };

  struct Serializer {
    const std::string name;
    const std::map<TextFormat::FormatID, SerializeRule> rules;
    explicit(false) Serializer(std::string name)
        : name(std::move(name)){};
  };

  struct SerializeResult {
    std::string name;
    std::string result;
    std::vector<TextFormat::FormatID> missed_formats;
  };

 private:
  Underlying m_text;
  std::map<TextFormat, cse::IndexSet> m_formatting;

 public:
  RichText() = default;
  RichText(const RichText&) = default;
  RichText(RichText&&) = default;
  RichText& operator=(const RichText&) = default;
  RichText& operator=(RichText&&) = default;
  ~RichText() = default;

  explicit RichText(Underlying text) : m_text(std::move(text)) {}
  explicit RichText(const CharT* text) : m_text(text) {}
  
  // Allow passing in any valid range to construct the RichText
  // object
  template<typename R>
    requires std::same_as<std::ranges::range_value_t<R>, CharT>
  RichText(const R& text) {
    m_text.insert_range(m_text.end(), text);
  }

  /**
   * @brief Assign a string into RichText
   * @param text The string to copy into RichText
   */
  RichText& operator=(const Underlying& text) {
    m_text = text;
    m_formatting.clear();
    return *this;
  }

  /**
   * @brief Assign and move a string into RichText
   * @param text The string to move into RichText
   */
  RichText& operator=(Underlying&& text) {
    m_text = std::move(text);
    m_formatting.clear();
    return *this;
  }

  RichText& operator=(const CharT* text) {
    m_text = Underlying(text);
    m_formatting.clear();
    return *this;
  }

  /**
   * @brief Append another RichText to RichText
   * @param str The other RichText instance to append
   */
  RichText& operator+=(const RichText& str) { return append(str); }

  /**
   * @brief Append a string to RichText
   * @param str The string to append
   * existing character to new characters
   */
  RichText& operator+=(const Underlying& str) { return append(str); }

  /**
   * @brief Get the size of the underlying string
   * @return The size of the text
   */
  [[nodiscard]] size_t size() const noexcept { return m_text.size(); }

  /**
   * @brief Get the character at a position
   * @param pos The position of the desired character
   * @return The character at position `pos`
   */
  [[nodiscard]] const CharT& char_at(size_t pos) const {
    // TODO replace with subscript operator
    return m_text.at(pos);
  }

  /**
   * @brief Get the underlying string of this RichText
   * @return The text without formatting
   */
  [[nodiscard]] Underlying to_string() {return m_text;}

  /**
   * @brief Get the formats at a position
   * @param pos The position to check for formats
   * @return A vector of `TextFormat`s at position `pos`
   */
  [[nodiscard]] std::vector<TextFormat> formats_at(size_t pos) const {
    dbg_assert(pos < m_text.size(),
               std::format("Out of bounds access, idx: {} size: {}", pos,
                           m_text.size()));
    std::vector<TextFormat> result;
    for (const auto& [format, indices] : m_formatting) {
      if (indices.contains(pos)) result.push_back(format);
    }
    return result;
  }

  /**
   * @brief Insert a string into RichText
   * @param index The index to insert the string at
   * @param str The string to insert
   * @param extend_formatting Whether to extend formatting applied to the
   * character before `index` to the new characters
   */
  template<typename Str>
    requires std::derived_from<Str, std::basic_string<CharT>>
  RichText& insert(size_t index, const Str& str,
                   bool extend_formatting = false) {
    cse_assert(!extend_formatting, "TODO Not implemented");
    m_text.insert(index, str);
    std::size_t len = str.length();
    for (auto& [_, indices] : m_formatting) {
      indices.shift_right_within(len, index, indices.size() + len);
    }
    return *this;
  }

  /**
   * @brief Insert a string into RichText
   * @param index The index to insert the string at
   * @param str The string to insert
   * @param extend_formatting Whether to extend formatting applied to the
   * character before `index` to the new characters
   */
  RichText& insert(size_t index, const CharT* str,
                   bool extend_formatting = false) {
    cse_assert(!extend_formatting, "TODO Not implemented");
    auto sstr = std::basic_string<CharT>(str);
    m_text.insert(index, sstr);
    std::size_t len = sstr.length();
    for (auto& [_, indices] : m_formatting) {
      indices.shift_right_within(len, index, indices.size() + len);
    }
    return *this;
  }

  /**
   * @brief Insert another RichText into RichText
   * @param index The index to insert the other RichText instance at
   * @param str The other RichText to insert
   */
  template<typename T>
  RichText& insert(size_t index, const RichText<CharT, T>& str) {
    std::size_t old_len = m_text.length();
    insert(index, str.m_text);
    for (auto [format, indices] : str.m_formatting) {
      indices.shift_right(old_len);
      apply_format(format, indices);
    }
    return *this;
  }

  /**
   * @brief Append a string to RichText
   * @param str The string to append
   * @param extend_formatting Whether to extend formatting applied to the last
   * existing character to new characters
   */
  template<typename Str>
    requires std::derived_from<Str, std::basic_string<CharT>>
  RichText& append(const Str& str, bool extend_formatting = false) {
    cse_assert(!extend_formatting, "TODO Not implemented");
    return insert(size(), str);
  }

  /**
   * @brief Append another RichText to RichText
   * @param str The other RichText instance to append
   */
  template<typename T>
  RichText& append(const RichText<CharT, T>& str) { return insert(size(), str); }

  /**
   * @brief Append a character to RichText
   * @param ch The character to append
   * @param extend_formatting Whether to extend formatting applied to the last
   * existing character to new character
   */
  RichText& push_back(CharT ch, bool extend_formatting = false) {
    cse_assert(!extend_formatting, "TODO Not implemented");
    m_text.push_back(ch);
    return *this;
  }

  /**
   * @brief Applies a format to the range [start, end)
   * @param format Format to apply
   * @param begin Beginning of range to apply format to
   * @param end End (exclusive) of range to apply format to
   */
  void apply_format(const TextFormat& format, const size_t begin,
                    const size_t end) {
    dbg_assert(
        end >= begin,
        std::format("Format range ends after beginning, begin: {}, end: {}",
                    begin, end));

    auto [item, inserted] =
        m_formatting.insert({format, IndexSet(std::pair{begin, end})});
    if (!inserted) {
      item->second.insert_range(begin, end);
    }
  }

  /**
   * @brief Applies a format to the specified indices
   * @param format Format to apply
   * @param begin Beginning of range to apply format to
   * @param end End (exclusive) of range to apply format to
   */
  void apply_format(const TextFormat& format, const IndexSet& indices) {
    auto [item, inserted] = m_formatting.insert({format, indices});
    if (!inserted) {
      item->second |= indices;
    }
  }

  /**
   * @brief Get the range corresponding to a `TextFormat`
   * @param format The format to get the range for
   * @return The IndexSet corresponding to the format, if it exists
   */
  [[nodiscard]] std::optional<IndexSet> get_format_range(
      const TextFormat& format) const {
    const auto iter = m_formatting.find(format);
    if (iter == m_formatting.end()) return {};
    return {iter->second};
  }

  struct FormatSerializeTracker {
    TextFormat format;
    SerializeRule rule;
    IndexSet::const_pair_iterator iter;
    IndexSet::const_pair_iterator end;
    FormatSerializeTracker(TextFormat format, SerializeRule rule,
                           IndexSet::const_pair_iterator begin,
                           IndexSet::const_pair_iterator end)
        : format(format), rule(rule), iter(begin), end(end) {}

    bool operator<(const FormatSerializeTracker& rhs) const {
      dbg_assert(iter != end,
                 "FormatSerializeTracker compare failed: invalid iterator.");
      return (*iter).first < (*rhs.iter).first ||
             ((*iter).first == (*rhs.iter).first &&
              (*iter).second < (*rhs.iter).second);
    }
  };

  [[nodiscard]] SerializeResult serialize(const Serializer& serializer) const {
    SerializeResult result;
    result.name = serializer.name;

    std::set<FormatSerializeTracker> trackers;

    auto format_iter = m_formatting.begin();
    auto const format_iter_end = m_formatting.end();
    auto serializer_format_iter = serializer.serialize_rules.begin();
    auto const serializer_format_iter_end = serializer.serialize_rules.end();

    while (format_iter != format_iter_end &&
           serializer_format_iter != serializer_format_iter_end) {
      if (format_iter->first < serializer_format_iter->first) {
        result.missed_formats.push_back(format_iter->first);
        ++format_iter;
      } else if (format_iter->first == serializer_format_iter->first) {
        trackers.emplace(format_iter->first,                 // Format
                         serializer_format_iter->second,     // Serialize Rule
                         format_iter->second.cbegin_pair(),  // Format Iterators
                         format_iter->second.cend_pair());
        ++format_iter;
        ++serializer_format_iter;
      } else {
        ++serializer_format_iter;
      }
    }

    while (format_iter != format_iter_end) {
      result.missed_formats.push_back(format_iter->first);
      ++format_iter;
    }

    size_t current = 0;
    auto tracker_iter = trackers.begin();
    auto const tracker_begin = trackers.begin();
    auto const tracker_end = trackers.end();

    // Process the text
    while (current < m_text.size() && !trackers.empty()) {
      size_t next = SIZE_MAX;  // Track the next format deactivation

      // Apply formats
      while (tracker_iter != tracker_end &&
             (*tracker_iter->iter).first <= current) {
        // The format begins, add the token
        if ((*tracker_iter->iter).first <= current) {
          result.result += tracker_iter->rule.start_token;
        }

        // Keep track of when the closest formatting change is
        if ((*tracker_iter->iter).second < next)
          next == (*tracker_iter->iter).second;
        ++tracker_iter;
      }

      // Process all the upcoming activating formats
      while (tracker_iter != tracker_end &&
             (*tracker_iter->iter).first < next) {
        result.result +=
            m_text.substr(current, (*tracker_iter->iter).first - current);
        result.result += tracker_iter->rule.start_token;
        current = (*tracker_iter->iter).first;

        // Keep track of when the closest formatting deactivation is
        if ((*tracker_iter->iter).second < next)
          next == (*tracker_iter->iter).second;
        ++tracker_iter;
      }

      // Jump to next formatting deactivation
      result.result += m_text.substr(current, next - current);

      // Unwind formatting

      // Find the rule to end
      auto rule_to_end_iter = trackers.begin();
      while (rule_to_end_iter != tracker_end &&
             (*rule_to_end_iter->iter).second != next)
        ++rule_to_end_iter;
      dbg_assert(
          rule_to_end_iter != tracker_end,
          "rule_to_end_iter failed to find the rule that needed ending.");

      while (tracker_iter != rule_to_end_iter) {
        result.result += tracker_iter->rule.end_token;
        // TODO: Check to see if rule needs to be reapplied after unraveling
        // TODO: If it doesn't pull the tracker out and update it
        --tracker_iter;
      }
      result.result += rule_to_end_iter->rule.end_token;
      // TODO: Pull the tracker out and modify it
      // TODO: Reapply the unraveled formats.

      dbg_assert_never(
          "Loop not guaranteed to end while TODOs are present in code.");
    }

    return result;
  }
};
}  // namespace cse
