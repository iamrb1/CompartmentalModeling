#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <format>
#include <functional>
#include <map>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <utility>
#include <variant>
#include <vector>

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
template <typename CharT = char, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT> >
class RichText {
 public:
  struct SerializeLambdaInput {
    TextFormat format;
  };

  struct SerializeLambdaOutput {
    std::string token;
  };

  /**
   * A rule for a format describing how it should be serialized.
   */
  struct SerializeRule {
    // No format name here, that is stored in the map as a key
    std::string start_token;
    std::string end_token;
    std::function<SerializeLambdaOutput&()> format;
  };

  struct Serializer {
    const std::string name;
    const std::map<FormatID, SerializeRule> serialize_rules;
    explicit(false) Serializer(std::string name) : name(std::move(name)){};
  };

  struct SerializeResult {
    std::string name;
    std::string result;
    std::vector<FormatID> missed_formats;
  };

 private:
  using string = std::basic_string<CharT, Traits, Allocator>;
  string m_text;
  std::map<TextFormat, cse::IndexSet> m_formatting;

 public:
  RichText() = default;
  RichText(const RichText&) = default;
  RichText(RichText&&) = default;
  RichText& operator=(const RichText&) = default;
  RichText& operator=(RichText&&) = default;
  ~RichText() = default;

  explicit RichText(string text) : m_text(std::move(text)) {}
  explicit RichText(const CharT* text) : m_text(text) {}

  // Assign a string to RichText
  RichText& operator=(const string& text) {
    m_text = text;
    m_formatting.clear();
    return *this;
  }

  // Assign and move a string into RichText
  RichText& operator=(string&& text) {
    m_text = std::move(text);
    m_formatting.clear();
    return *this;
  }

  [[nodiscard]] size_t size() const noexcept { return m_text.size(); }

  [[nodiscard]] const CharT& char_at(size_t pos) const {
    return m_text.at(pos);
  }

  [[nodiscard]] string to_string() const { return m_text; }

  [[nodiscard]] std::vector<TextFormat> formats_at(size_t pos) const {
    dbg_assert(pos < m_text.size(),
               std::format("Out of bounds access, idx: {} size: {}", pos,
                           m_text.size()));
    std::vector<TextFormat> result;
    for (const auto& [format, index] : m_formatting) {
      if (index.contains(pos)) result.push_back(format);
    }
    return result;
  }

  RichText& append(const RichText& str) {
    auto left = m_formatting.begin();
    auto const left_end = m_formatting.end();
    auto right = str.m_formatting.begin();
    auto const right_end = str.m_formatting.end();
    while (left != left_end && right != right_end) {
      if (left->first == right->first) {
        left->second.append_at(right->second, m_text.size());
        ++left;
        ++right;
        continue;
      }
      if (left->first < right->first) {
        ++left;
        continue;
      }

      m_formatting.insert({right->first, right->second})
          .first->second.offset(m_text.size());
      ++right;
    }
    while (right != right_end) {
      m_formatting.insert({right->first, right->second})
          .first->second.offset(m_text.size());
      ++right;
    }
    m_text += str.m_text;
    return *this;
  }

  void apply_format_to_range(const TextFormat& format, const size_t begin,
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

  RichText& operator+=(const RichText& str) { return append(str); }

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

  RichText& insert(size_t index, const string& str) {
    m_text.insert(index, str);
    // TODO: Shift over formatting
    return *this;
  }

  RichText& insert(size_t index, const RichText<CharT>& str) {
    m_text.insert(index, str.m_text);
    // TODO: Shift over formatting and insert formatting from str
    return *this;
  }

  // TODO change std::invocable argument to T& once RichText is templated
  template <std::invocable<string&> Callable>
  void update(Callable const& callable) {
    size_t const old_size = m_text.size();
    std::optional<IndexSet> const indices_opt = callable(m_text);
    size_t const new_size = m_text.size();

    if (new_size > old_size) {
      // insertion
      // extend formatting options following each indicated index
      cse_assert(indices_opt.has_value(),
                 "size increased during update, but no indices returned");
      IndexSet indices{*indices_opt};

      // TODO: how to handle multiple discontinuous insertions?

      // TODO
    } else if (new_size < old_size) {
      // deletion
      // reduce formatting at each indicated index
      cse_assert(indices_opt.has_value(),
                 "size decreased during update, but no indices returned");
      IndexSet indices{*indices_opt};

      for (auto& [_, fmt_indices] : m_formatting) {
        // move this into IndexSet?
        size_t offset = 0;
        IndexSet new_indices{};

        auto max = std::max_element(fmt_indices.cbegin(), fmt_indices.cend());
        if (max == fmt_indices.cend()) continue;

        for (size_t idx = 0; idx <= *max; idx++) {
          if (indices.contains(idx)) {
            // index was deleted, advance offset
            offset++;
            continue;
          }
          if (fmt_indices.contains(idx)) {
            new_indices.insert(idx - offset);
          }
        }
        fmt_indices = new_indices;
      }

    } else {
      // substitution
      // no formatting changes necessary
      cse_assert(
          !indices_opt.has_value(),
          "size did not change during update, but some indices returned");
    }
  }
};
}  // namespace cse
