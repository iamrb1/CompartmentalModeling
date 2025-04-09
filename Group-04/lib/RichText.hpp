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
#include <iterator>
#include <limits>
#include <map>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <type_traits>
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
template <typename CharT = char, typename Underlying = std::basic_string<CharT>>
  requires std::derived_from<Underlying, std::basic_string<CharT>>
class BasicRichText {
 public:
  /**
   * A rule describing how a format should be serialized.
   */
  class SerializeRule {
   private:
    // No format name here, that is stored in the map as a key
    using TokenLambda = std::function<Underlying(const TextFormat&)>;
    using TokenVariant = std::variant<Underlying, TokenLambda>;
    TokenVariant start_token;
    TokenVariant end_token;

    /**
     * @brief Get a token string from the variant, or generate a token with a
     * lambda
     * @param token The TokenVariant to use
     * @param format The text format to pass to the lambda (if applicable)
     */
    Underlying Token(TokenVariant const& token,
                     TextFormat const& format) const {
      switch (token.index()) {
        case 0:
          cse_assert(std::holds_alternative<Underlying>(token));
          return std::get<Underlying>(token);
        case 1:
          cse_assert(std::holds_alternative<TokenLambda>(token));
          return std::get<TokenLambda>(token)(format);
        default:
          cse_assert_never("Impossible variant access");
      }
    }

   public:
    template <typename T1, typename T2>
    SerializeRule(T1 start_token, T2 end_token)
        : start_token(std::move(start_token)),
          end_token(std::move(end_token)){};

    /**
     * @brief Generate the start token for this rule
     * @param format The text format to pass to the lambda (if applicable)
     */
    Underlying StartToken(TextFormat const& format) const {
      return Token(start_token, format);
    }

    /**
     * @brief Generate the start token for this rule
     * @param format The text format to pass to the lambda (if applicable)
     */
    Underlying EndToken(TextFormat const& format) const {
      return Token(end_token, format);
    }
  };

  struct Serializer {
    using Rules = std::map<TextFormat::FormatID, SerializeRule>;

    const std::string name;
    std::optional<Underlying> header = std::nullopt;
    std::optional<Underlying> footer = std::nullopt;
    Rules rules = Rules{};

    template <typename T1, typename T2>
    Serializer& AddRule(TextFormat::FormatID const& id, T1 start_token,
                        T2 end_token) {
      rules.emplace(
          id, SerializeRule{std::move(start_token), std::move(end_token)});
      return *this;
    };
  };

  struct SerializeResult {
    const std::string name;
    Underlying output{};
    std::vector<TextFormat> missed_formats{};
  };

 private:
  using IndexRange = IndexSet::IndexRange;

  Underlying m_text;
  std::map<TextFormat, cse::IndexSet> m_formatting;

 public:
  BasicRichText() = default;
  BasicRichText(const BasicRichText&) = default;
  BasicRichText(BasicRichText&&) = default;
  BasicRichText& operator=(const BasicRichText&) = default;
  BasicRichText& operator=(BasicRichText&&) = default;
  ~BasicRichText() = default;

  explicit BasicRichText(Underlying text) : m_text(std::move(text)) {}

  // Allow passing in any valid range to construct the RichText
  // object
  template <typename R>
    requires std::same_as<std::ranges::range_value_t<R>, CharT>
  BasicRichText(const R& text) {
    std::ranges::copy(text, std::back_inserter(m_text));
  }

  // Constructor for string literals, since we don't want the null terminator
  // (which is part of the range for some reason?)
  BasicRichText(const CharT* text) : m_text(text) {}

  /**
   * @brief Assign a string into RichText
   * @param text The string to copy into RichText
   */
  BasicRichText& operator=(const Underlying& text) {
    m_text = text;
    m_formatting.clear();
    return *this;
  }

  /**
   * @brief Assign and move a string into RichText
   * @param text The string to move into RichText
   */
  BasicRichText& operator=(Underlying&& text) {
    m_text = std::move(text);
    m_formatting.clear();
    return *this;
  }

  BasicRichText& operator=(const CharT* text) {
    m_text = Underlying(text);
    m_formatting.clear();
    return *this;
  }

  /**
   * @brief Append another RichText to RichText
   * @param str The other RichText instance to append
   */
  BasicRichText& operator+=(const BasicRichText& str) { return append(str); }

  /**
   * @brief Append a string to RichText
   * @param str The string to append
   * existing character to new characters
   */
  BasicRichText& operator+=(const Underlying& str) { return append(str); }

  /**
   * @brief Get the character at a position (const reference)
   * @param pos The position of the desired character
   * @return The character at position `pos`
   */
  [[nodiscard]] CharT& operator[](std::size_t pos) const {
    return m_text.at(pos);
  }

  /**
   * @brief Get the character at a position (non-const reference)
   * @param pos The position of the desired character
   * @return The character at position `pos`
   */
  [[nodiscard]] CharT& operator[](std::size_t pos) { return m_text.at(pos); }

  /**
   * @brief Get the size of the underlying string
   * @return The size of the text
   */
  [[nodiscard]] std::size_t size() const noexcept { return m_text.size(); }

  /**
   * @brief Get the underlying string of this RichText
   * @return The text without formatting
   */
  [[nodiscard]] Underlying to_string() { return m_text; }

  /**
   * @brief Get the formats at a position
   * @param pos The position to check for formats
   * @return A vector of `TextFormat`s at position `pos`
   */
  [[nodiscard]] std::vector<TextFormat> formats_at(std::size_t pos) const {
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
   * @brief Get the largest range containing `pos` with no formatting changes
   * @param pos The position to check for range changes
   */
  IndexRange segment_at(std::size_t pos) const {
    dbg_assert(pos < m_text.size(),
               std::format("Out of bounds access, idx: {} size: {}", pos,
                           m_text.size()));
    // index range containing entire richtext
    IndexSet text_range{std::pair{0, m_text.size()}};
    // get containing ranges of all formats (or lack thereof) at pos
    auto ranges =
        std::views::elements<1>(m_formatting) |
        filter_transform([pos, &text_range](IndexSet const& fmt_indices) {
          auto range = fmt_indices.get_containing_range(pos);
          // if a range encompasses pos, return it
          if (range.has_value()) {
            return range;
          }
          // otherwise, return the unformatted range encompassing pos.
          // if the range is nullopt then the text must be empty, so just
          // filter out the range
          auto unfmt_indices = text_range - fmt_indices;
          return unfmt_indices.get_containing_range(pos);
        });

    return std::ranges::fold_left(
        ranges, IndexRange{0, m_text.size()},
        [](IndexRange const& acc, IndexRange const& it) {
          std::size_t start = std::max(acc.start, it.start);
          std::size_t end = std::min(acc.end, it.end);
          return IndexRange{start, end};
        });
  }

  /**
   * @brief Insert a string into RichText
   * @param index The index to insert the string at
   * @param str The string to insert
   * character before `index` to the new characters
   */
  template <typename Str>
    requires std::derived_from<Str, std::basic_string<CharT>>
  BasicRichText& insert(std::size_t index, const Str& str) {
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
   * character before `index` to the new characters
   */
  BasicRichText& insert(std::size_t index, const CharT* str) {
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
  template <typename T>
  BasicRichText& insert(std::size_t index, const BasicRichText<CharT, T>& str) {
    insert(index, str.m_text);
    for (auto [format, indices] : str.m_formatting) {
      indices.shift_right(index);
      apply_format(format, indices);
    }
    return *this;
  }

  /**
   * @brief Append a string to RichText
   * @param str The string to append
   * existing character to new characters
   */
  template <typename Str>
    requires std::derived_from<Str, std::basic_string<CharT>>
  BasicRichText& append(const Str& str) {
    return insert(size(), str);
  }

  /**
   * @brief Append another RichText to RichText
   * @param str The other RichText instance to append
   */
  template <typename T>
  BasicRichText& append(const BasicRichText<CharT, T>& str) {
    return insert(size(), str);
  }

  /**
   * @brief Append a character to RichText
   * @param ch The character to append
   * existing character to new character
   */
  BasicRichText& push_back(CharT ch) {
    m_text.push_back(ch);
    return *this;
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
      item->second.clamp(0, m_text.size());
    }
  }

  /**
   * @brief Applies a format to the range [start, end)
   * @param format Format to apply
   * @param begin Beginning of range to apply format to
   * @param end End (exclusive) of range to apply format to
   */
  void apply_format(const TextFormat& format, const std::size_t begin,
                    const std::size_t end) {
    dbg_assert(
        end >= begin,
        std::format("Format range ends after beginning, begin: {}, end: {}",
                    begin, end));
    apply_format(format, std::pair{begin, end});
  }

  /**
   * @brief Applies a format to the entire text
   * @param format Format to apply
   */
  void apply_format(const TextFormat& format) {
    apply_format(format, 0, m_text.size());
  }

  /**
   * @brief Clear all formatting
   */
  void clear_format() { m_formatting.clear(); }

  /**
   * @brief Clear all formatting over indices
   * @param indices Indices to clear formatting for
   */
  void clear_format(const IndexSet& indices) {
    for (auto& [format, fmt_indices] : m_formatting) {
      fmt_indices -= indices;
    }
  }

  /**
   * @brief Clear all formatting of a specific type
   * @param format Format to clear
   */
  void clear_format(const TextFormat& format) {
    auto result = m_formatting.find(format);
    if (result != m_formatting.end()) {
      m_formatting.erase(result);
    }
  }

  /**
   * @brief Clear formatting of a specific type over indices
   * @param format Format to clear
   */
  void clear_format(const TextFormat& format, const IndexSet& indices) {
    auto result = m_formatting.find(format);
    if (result != m_formatting.end()) {
      result->second -= indices;
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
    SerializeResult result{serializer.name};

    if (serializer.header) {
      result.output += *serializer.header;
    }

    std::set<FormatSerializeTracker> trackers;

    // Populate the format trackers
    for (auto& [format, indices] : m_formatting) {
      auto serializer_format = serializer.rules.find(format.name);
      if (serializer_format != serializer.rules.end()) {
        trackers.emplace(format, serializer_format->second,
                         indices.cbegin_pair(), indices.cend_pair());
      } else {
        result.missed_formats.push_back(format.name);
      }
    }

    std::size_t current = 0;

    // Process the text
    while (current < m_text.size() && !trackers.empty()) {
      // Restart trackers while there's still text to process
      auto tracker_iter = trackers.begin();

      // Track the next format deactivation
      std::size_t next = std::numeric_limits<std::size_t>::max();

      // Apply formats
      while (tracker_iter != trackers.end() &&
             (*tracker_iter->iter).first <= current) {
        // The format begins, add the token
        if ((*tracker_iter->iter).first == current) {
          result.output += tracker_iter->rule.StartToken(tracker_iter->format);
        }

        // Keep track of when the closest formatting deactivation is
        if ((*tracker_iter->iter).second < next)
          next = (*tracker_iter->iter).second;
        ++tracker_iter;
      }

      // Process all the upcoming activating format activations
      while (tracker_iter != trackers.end() &&
             (*tracker_iter->iter).first < next) {
        result.output +=
            m_text.substr(current, (*tracker_iter->iter).first - current);
        result.output += tracker_iter->rule.StartToken(tracker_iter->format);
        current = (*tracker_iter->iter).first;

        // Keep track of when the closest formatting deactivation is
        if ((*tracker_iter->iter).second < next)
          next = (*tracker_iter->iter).second;
        ++tracker_iter;
      }

      // Jump to next formatting deactivation
      result.output += m_text.substr(current, next - current);
      current = next;

      // Unwind formatting

      // Find the rule to end
      auto rule_to_end_iter = trackers.begin();
      while (rule_to_end_iter != trackers.end() &&
             (*rule_to_end_iter->iter).second != next)
        ++rule_to_end_iter;
      dbg_assert(
          rule_to_end_iter != trackers.end(),
          "rule_to_end_iter failed to find the rule that needed ending.");

      while (tracker_iter != rule_to_end_iter) {
        --tracker_iter;
        result.output += tracker_iter->rule.EndToken(tracker_iter->format);
      }

      bool all_deactivated = tracker_iter == trackers.begin();
      if (!all_deactivated) --tracker_iter;
      // tracker_iter now points to the rule before the rule that needs to be
      // deactivated (or all_deactivated is true, then it points to the first
      // rule)

      typename decltype(trackers)::const_iterator reapply_rule_iter;
      do {
        if (all_deactivated) {
          reapply_rule_iter = trackers.begin();
        } else {
          reapply_rule_iter = tracker_iter;
          ++reapply_rule_iter;
        }
        if (reapply_rule_iter == trackers.end()) break;
        if ((*reapply_rule_iter->iter).first > current) break;
        if ((*reapply_rule_iter->iter).second <= current) {
          auto tracker_to_update = trackers.extract(reapply_rule_iter);
          ++tracker_to_update.value().iter;
          if (tracker_to_update.value().iter != tracker_to_update.value().end)
            trackers.insert(std::move(tracker_to_update));
          continue;
        }
        result.output +=
            reapply_rule_iter->rule.StartToken(tracker_iter->format);
        if (all_deactivated)
          all_deactivated = false;
        else
          ++tracker_iter;
      } while (tracker_iter != trackers.end());
    }

    if (current < m_text.size())
      result.output += m_text.substr(current, m_text.size());

    if (serializer.footer) {
      result.output += *serializer.footer;
    }

    return result;
  }

  /**
   * @brief Erases `count` characters starting at `index`.
   *        Removes substring [index..index+count) from m_text and
   *        updates each format's IndexSet accordingly, removing empty sets.
   */
  void erase(std::size_t index, std::size_t count) {
    // 1) Physically remove the substring from the underlying text
    std::size_t const old_size = m_text.size();
    m_text.erase(index, count);

    // 2) For each format's IndexSet, shift erased indices out
    for (auto& [format, idxSet] : m_formatting) {
      idxSet.shift_left_within(count, index, old_size);
    }

    // 3) Remove formats whose sets became empty
    for (auto it = m_formatting.begin(); it != m_formatting.end();) {
      if (it->second.size() == 0) {
        it = m_formatting.erase(it);
      } else {
        ++it;
      }
    }
  }

  /**
   * @brief Erases each [start,end) in `to_erase` by calling the
   *        (index,count) overload in descending order of start.
   */
  void erase(const cse::IndexSet& to_erase) {
    // Gather all IndexRanges from to_erase
    std::vector<IndexRange> ranges{to_erase.cbegin_pair(),
                                   to_erase.cend_pair()};

    // Reverse them so we erase from the highest start index first
    std::reverse(ranges.begin(), ranges.end());

    // For each range, call the 2-param erase
    for (auto const& r : ranges) {
      // Overload ambiguity fix: qualify with this-> to ensure we call
      // erase(std::size_t, std::size_t) instead of erase(const IndexSet&)
      this->erase(r.start, r.end - r.start);
    }
  }
};

using RichText = BasicRichText<>;

/**
 * A composition of filter and transform.
 *
 * Transformed values should be enclosed in an std::optional. To filter out an
 * element, return std::nullopt.
 *
 * Known as filter map in other languages
 */
auto filter_transform(auto const& func) {
  return std::views::transform(func) |
         std::views::filter(
             [](auto const& option) { return option.has_value(); }) |
         std::views::transform([](auto const& option) { return *option; });
}

}  // namespace cse
