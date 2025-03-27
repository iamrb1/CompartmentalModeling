#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "BitVector.hpp"
#include "CseAssert.hpp"

namespace cse {
/**
 * @brief A class that efficiently stores and recalls ranges of numbers in a
 * set.
 *
 * IndexSet provides an efficient way to store sets of integers by maintaining
 * them as ranges. For example, the sequence 1,2,3,5,6,7 would be stored as two
 * ranges: [1,4) and [5,8). This is particularly efficient when storing
 * consecutive numbers.
 *
 * The class automatically merges adjacent or overlapping ranges and splits
 * ranges when removing elements. All ranges are stored in the half-open
 * interval format [start, end) where end is exclusive.
 */
class IndexSet {
 public:
  // Define the maximum size_t value to indicate that no range ID exists.
  static constexpr std::size_t NO_RANGE = static_cast<std::size_t>(-1);
  
  // Define a custom range type for better readability
  struct IndexRange {
    std::size_t start;
    std::size_t end;
    
    // Public member aliases for backward compatibility
    std::size_t first;
    std::size_t second;
    
    // Constructor to initialize both sets of values
    IndexRange(std::size_t s = 0, std::size_t e = 0) 
      : start(s), end(e), first(s), second(e) {}
    
    // Copy constructor
    IndexRange(const IndexRange& other)
      : start(other.start), end(other.end), first(other.start), second(other.end) {}
    
    // Copy assignment operator
    IndexRange& operator=(const IndexRange& other) {
      if (this != &other) {
        start = other.start;
        end = other.end;
        first = other.start;
        second = other.end;
      }
      return *this;
    }
    
    // Move constructor
    IndexRange(IndexRange&& other) noexcept
      : start(other.start), end(other.end), first(other.start), second(other.end) {}
    
    // Move assignment operator
    IndexRange& operator=(IndexRange&& other) noexcept {
      if (this != &other) {
        start = other.start;
        end = other.end;
        first = other.start;
        second = other.end;
      }
      return *this;
    }
    
    bool operator<(const IndexRange& other) const {
      return start < other.start;
    }
    
    bool operator==(const IndexRange& other) const {
      return start == other.start && end == other.end;
    }
    
    bool operator!=(const IndexRange& other) const {
      return !(*this == other);
    }
  };
  
  // Iterator class definitions
  class iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = std::size_t*;
    using reference = std::size_t&;

    iterator(const std::vector<IndexRange>& ranges,
             std::size_t range_idx = 0, std::size_t offset = 0)
        : ranges_(&ranges), range_idx_(range_idx), offset_(offset) {
      // If this is an end iterator (range_idx >= size)
      if (range_idx_ >= ranges_->size()) {
        return;
      }
      
      // Set current_ to the actual value at the specified range and offset
      current_ = (*ranges_)[range_idx_].start + offset_;
      
      // Make sure the offset is valid for this range
      if (current_ >= (*ranges_)[range_idx_].end) {
        // If not, move to the next range if possible
        move_to_next_valid_position();
      }
    }

    value_type operator*() const { return current_; }

    iterator& operator++() {
      // If already at end, don't advance
      if (range_idx_ >= ranges_->size()) return *this;
      
      // Increment offset and current position
      offset_++;
      current_++;
      
      // If we've reached the end of the current range
      if (current_ >= (*ranges_)[range_idx_].end) {
        move_to_next_valid_position();
      }
      
      return *this;
    }

    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const iterator& other) const {
      // Check if both iterators are at the end
      bool this_at_end = range_idx_ >= ranges_->size();
      bool other_at_end = other.range_idx_ >= other.ranges_->size();
      
      if (this_at_end && other_at_end) return true;
      if (this_at_end != other_at_end) return false;
      
      // Otherwise, compare all fields
      return ranges_ == other.ranges_ && range_idx_ == other.range_idx_ &&
             current_ == other.current_;
    }

    bool operator!=(const iterator& other) const { return !(*this == other); }

   private:
    const std::vector<IndexRange>* ranges_;
    std::size_t range_idx_;
    std::size_t offset_ = 0;  // Offset within the current range
    std::size_t current_ = 0; // The actual value being pointed to
    
    // Helper method to move to the next valid position when at the end of a range
    void move_to_next_valid_position() {
      // Reset offset since we're moving to a new range
      offset_ = 0;
      // Move to the next range
      range_idx_++;
      
      // If there are more ranges, set current to the first element of that range
      if (range_idx_ < ranges_->size()) {
        current_ = (*ranges_)[range_idx_].start;
      }
    }
  };

  class const_iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const std::size_t*;
    using reference = const std::size_t&;

    const_iterator(
        const std::vector<IndexRange>& ranges,
        std::size_t range_idx = 0, std::size_t offset = 0)
        : ranges_(&ranges), range_idx_(range_idx), offset_(offset) {
      // If this is an end iterator (range_idx >= size)
      if (range_idx_ >= ranges_->size()) {
        return;
      }
      
      // Set current_ to the actual value at the specified range and offset
      current_ = (*ranges_)[range_idx_].start + offset_;
      
      // Make sure the offset is valid for this range
      if (current_ >= (*ranges_)[range_idx_].end) {
        // If not, move to the next range if possible
        move_to_next_valid_position();
      }
    }

    std::remove_const_t<value_type> operator*() const { return current_; }

    const_iterator& operator++() {
      // If already at end, don't advance
      if (range_idx_ >= ranges_->size()) return *this;
      
      // Increment offset and current position
      offset_++;
      current_++;
      
      // If we've reached the end of the current range
      if (current_ >= (*ranges_)[range_idx_].end) {
        move_to_next_valid_position();
      }
      
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const const_iterator& other) const {
      // Check if both iterators are at the end
      bool this_at_end = range_idx_ >= ranges_->size();
      bool other_at_end = other.range_idx_ >= other.ranges_->size();
      
      if (this_at_end && other_at_end) return true;
      if (this_at_end != other_at_end) return false;
      
      // Otherwise, compare all fields
      return ranges_ == other.ranges_ && range_idx_ == other.range_idx_ &&
             current_ == other.current_;
    }

    bool operator!=(const const_iterator& other) const {
      return !(*this == other);
    }

   private:
    const std::vector<IndexRange>* ranges_;
    std::size_t range_idx_;
    std::size_t offset_ = 0;  // Offset within the current range
    std::size_t current_ = 0; // The actual value being pointed to
    
    // Helper method to move to the next valid position when at the end of a range
    void move_to_next_valid_position() {
      // Reset offset since we're moving to a new range
      offset_ = 0;
      // Move to the next range
      range_idx_++;
      
      // If there are more ranges, set current to the first element of that range
      if (range_idx_ < ranges_->size()) {
        current_ = (*ranges_)[range_idx_].start;
      }
    }
  };

  class const_pair_iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = IndexRange;
    using difference_type = std::ptrdiff_t;
    using pointer = const std::size_t*;
    using reference = const std::size_t&;

    explicit const_pair_iterator(
        const std::vector<IndexRange>& ranges,
        std::size_t current = 0)
        : ranges_(&ranges), current_(current) {}

    value_type operator*() const {
      dbg_assert(current_ < ranges_->size(), "Iterator dereferenced out of range.");
      return (*ranges_)[current_];
    }

    const_pair_iterator& operator++() {
      dbg_assert(current_ < ranges_->size(), "Iterator moved out of range.");
      current_ += 1;
      return *this;
    }

    const_pair_iterator operator++(int) {
      const_pair_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const const_pair_iterator& other) const {
      return ranges_ == other.ranges_ && current_ == other.current_;
    }

    bool operator!=(const const_pair_iterator& other) const {
      return !(*this == other);
    }

  private:
    const std::vector<IndexRange>* ranges_;
    std::size_t current_;
  };

  // Constructors
  IndexSet() = default;
  ~IndexSet() = default;

  IndexSet(const IndexRange& range) {
    ranges_.push_back(range);
    total_size_ = range.end - range.start;
  }

  // Add constructor that takes std::pair for backward compatibility
  IndexSet(const std::pair<std::size_t, std::size_t>& range) {
    ranges_.push_back({range.first, range.second});
    total_size_ = range.second - range.first;
  }

  template <typename... T>
    requires std::convertible_to<std::common_type_t<T...>, std::size_t>
  IndexSet(T... sizes) {
    // initializer list range-for trick from
    // https://stackoverflow.com/a/50892567/4678913
    for (size_t idx : {sizes...}) {
      insert(idx);
    }
  }

  // Copy operations
  IndexSet(const IndexSet&) = default;
  IndexSet& operator=(const IndexSet&) = default;

  // Move operations
  IndexSet(IndexSet&& other) noexcept
      : ranges_(std::move(other.ranges_)), total_size_(other.total_size_) {
    // Clear the moved-from object
    other.ranges_.clear();
    other.total_size_ = 0;
  }

  IndexSet& operator=(IndexSet&& other) noexcept {
    if (this != &other) {
      ranges_ = std::move(other.ranges_);
      total_size_ = other.total_size_;
      // Clear the moved-from object
      other.ranges_.clear();
      other.total_size_ = 0;
    }
    return *this;
  }

  /**
   * @brief Adds an index to the set
   * @param index The index to add
   *
   * If the index is adjacent to or within an existing range, the ranges will
   * be automatically merged. For example, if [1,4) exists and we insert 4,
   * the range becomes [1,5).
   */
  void insert(std::size_t index) {
    // First check if the index is already in a range to avoid duplicate work
    auto range_idx = find_range_index(index);
    if (range_idx != NO_RANGE) {
      return;  // Already in set
    }

    // Track old size before merging to determine if we need to increment
    // total_size_
    std::size_t old_total = 0;
    for (const auto& range : ranges_) {
      old_total += range.end - range.start;
    }

    // Insert new range [index, index+1)
    ranges_.push_back({index, index + 1});

    // Merge any overlapping ranges - this will handle cases where the new index
    // is adjacent to or overlaps with existing ranges
    merge_overlapping_ranges();

    // Recalculate total size after merging
    total_size_ = 0;
    for (const auto& range : ranges_) {
      total_size_ += range.end - range.start;
    }

    // If total size didn't increase after merging, we must have merged with an
    // existing range In this case, we need to increment the size since we added
    // a new index
    if (total_size_ == old_total) {
      total_size_++;
    }
  }

  void insert_range(const std::size_t start, const std::size_t end) {
    if (start >= end) return;  // Invalid range

    // Insert new range directly
    ranges_.push_back({start, end});

    // Merge overlapping ranges
    merge_overlapping_ranges();

    // Recalculate total size
    total_size_ = 0;
    for (const auto& range : ranges_) {
      total_size_ += range.end - range.start;
    }
  }

  /**
   * @brief Removes an index from the set
   * @param index The index to remove
   *
   * If the index is in the middle of a range, the range will be split.
   * For example, if [1,5) exists and we remove 3, we get [1,3) and [4,5).
   */
  void remove(std::size_t index) {
    // Find the range containing this index
    auto range_idx = find_range_index(index);
    if (range_idx == NO_RANGE) {
      return;  // Index not in set
    }

    auto& range = ranges_[range_idx];
    if (range.start == index) {
      // Case 1: Removing from start of range
      range.start++;
      range.first++;  // Update first as well
      if (range.start == range.end) {
        // Range became empty, remove it
        ranges_.erase(ranges_.begin() + range_idx);
      }
    } else if (range.end == index + 1) {
      // Case 2: Removing from end of range
      range.end--;
      range.second--;  // Update second as well
    } else {
      // Case 3: Removing from middle of range - need to split into two ranges
      auto second_half = IndexRange{index + 1, range.end};
      range.end = index;
      range.second = index;  // Update second as well
      ranges_.insert(ranges_.begin() + range_idx + 1, second_half);
    }

    total_size_--;
  }

  /**
   * @brief Checks if an index is in the set
   * @param index The index to check
   * @return true if the index is in the set, false otherwise
   */
  bool contains(std::size_t index) const {
    return find_range_index(index) != NO_RANGE;
  }

  /**
   * @brief Returns the total number of indices in the set
   * @return The sum of the sizes of all ranges
   */
  std::size_t size() const { return total_size_; }

  /**
   * @brief Returns the maximum index in the set
   */
  std::optional<std::size_t> min_index() const {
    std::vector<size_t> indices = get_all_indices();
    auto result = std::min_element(indices.begin(), indices.end());
    // the set is empty
    if (result == indices.end()) return std::nullopt;
    return *result;
  }

  /**
   * @brief Returns the maximum index in the set
   */
  std::optional<std::size_t> max_index() const {
    std::vector<size_t> indices = get_all_indices();
    auto result = std::max_element(indices.begin(), indices.end());
    // the set is empty
    if (result == indices.end()) return std::nullopt;
    return *result;
  }

  /**
   * @brief Returns a vector containing all indices in the set
   * @return Vector of indices in ascending order
   */
  std::vector<std::size_t> get_all_indices() const {
    std::vector<std::size_t> result;
    
    // First check if total_size_ is unexpectedly large
    // Set a reasonable limit to avoid bad_alloc errors
    // Cap at 10 million elements to prevent excessive memory use
    const std::size_t max_size = 10'000'000;
    if (total_size_ > max_size) {
      // If it's unreasonably large, log an error
      // Here we just return an empty result to prevent crash
      return result;
    }
    
    result.reserve(total_size_);  // Preallocate to avoid reallocations

    // Expand each range into individual indices
    for (const auto& range : ranges_) {
      if (range.end <= range.start) continue; // Skip invalid ranges
      
      for (std::size_t i = range.start; i < range.end; ++i) {
        result.push_back(i);
      }
    }

    return result;
  }

  /**
   * @brief Gets the next range after the given index
   * @param index The reference index
   * @return The next range if it exists, std::nullopt otherwise
   *
   * If the index is within a range, returns the next range after the
   * containing range. If the index is between ranges, returns the next range.
   */
  std::optional<IndexRange> get_next_range(
      std::size_t index) const {
    auto range_idx = find_range_index(index);

    // If index is in a range, we want the next range
    if (range_idx != NO_RANGE) {
      range_idx = range_idx + 1;
    } else {
      // Find the next range after this index
      range_idx = 0;
      while (range_idx < ranges_.size() && ranges_[range_idx].end <= index) {
        range_idx++;
      }
    }

    // Return next range if it exists
    if (range_idx < ranges_.size()) {
      return ranges_[range_idx];
    }
    return std::nullopt;
  }

  /**
   * @brief Gets the previous range before the given index
   * @param index The reference index
   * @return The previous range if it exists, std::nullopt otherwise
   */
  std::optional<IndexRange> get_prev_range(
      std::size_t index) const {
    auto range_idx = find_range_index(index);

    // Handle edge cases
    if (range_idx == 0 || ranges_.empty()) {
      return std::nullopt;
    }

    // If index is in current range, return previous range
    if (range_idx != NO_RANGE) {
      if (range_idx == 0) return std::nullopt;
      return ranges_[range_idx - 1];
    }

    // Find the range immediately before this index
    range_idx = 0;
    std::size_t prev_idx = NO_RANGE;
    while (range_idx < ranges_.size() && ranges_[range_idx].end <= index) {
      prev_idx = range_idx;
      range_idx++;
    }

    if (prev_idx != NO_RANGE) {
      return ranges_[prev_idx];
    }
    return std::nullopt;
  }

  /**
   * @brief Gets the range containing the given index
   * @param index The index to find
   * @return The containing range if it exists, std::nullopt otherwise
   */
  std::optional<IndexRange> get_containing_range(
      std::size_t index) const {
    auto range_idx = find_range_index(index);

    // If range contains the index, return it
    if (range_idx != NO_RANGE) {
      return ranges_[range_idx];
    }
    return std::nullopt;
  }

  void offset(const std::size_t offset) {
    for (auto& range : ranges_) {
      range.start += offset;
      range.end += offset;
      range.first += offset;   // Update first as well
      range.second += offset;  // Update second as well
    }
  }

  /**
   * @brief Shift all indices within [start, end) left by `shift_by` places
   *
   * @param shift_by The number of places to shift by
   * @param start Start of the range to shift within.
   * @param end End (exclusive) of the range to shift within.
   */
  void shift_left_within(const std::size_t shift_by, const std::size_t start,
                         const std::size_t end) {
    dbg_assert(end > start, "Shift range end is not past start");

    // load range into BitVector
    BitVector indices{end - start};
    for (size_t i = start; i < end; i++) {
      indices[i - start] = contains(i);
    }

    // shifting indices left actually means shifting BitVector right
    indices >>= shift_by;

    // restore shifted range from BitVector
    for (size_t i = start; i < end; i++) {
      if (indices[i - start]) {
        insert(i);
      } else {
        remove(i);
      }
    }
  }

  void shift_left(const std::size_t shift_by) {
    if (auto max = max_index()) shift_left_within(shift_by, 0, *max + 1);
  }

  /**
   * @brief Adds another index set to the end of this one, starting at the
   * index
   * @param indexSet The IndexSet to append
   * @param index The index to start appending
   */
  void append_at(const IndexSet& indexSet, const std::size_t index) {
    // Create a copy of the input IndexSet and offset it by the starting index
    IndexSet offsetSet = indexSet;
    offsetSet.offset(index);

    // Add all ranges from the offset set to this set
    for (const auto& range : offsetSet.ranges_) {
      insert_range(range.start, range.end);
    }
  }

  /**
   * @brief Computes the union of this set with another
   * @param other The other set to union with
   * @return A new set containing all elements that are in either set
   */
  IndexSet operator|(const IndexSet& other) const {
    IndexSet result = *this;  // Start with a copy of this set
    // Add all ranges from the other set
    for (const auto& range : other.ranges_) {
      result.insert_range(range.start, range.end);
    }
    return result;
  }

  /**
   * @brief Computes the intersection of this set with another
   * @param other The other set to intersect with
   * @return A new set containing all elements that are in both sets
   */
  IndexSet operator&(const IndexSet& other) const {
    IndexSet result;

    // For each range in this set
    for (const auto& range1 : ranges_) {
      // For each range in the other set
      for (const auto& range2 : other.ranges_) {
        // Find intersection of ranges
        auto start = std::max(range1.start, range2.start);
        auto end = std::min(range1.end, range2.end);

        // If ranges overlap, add their intersection
        if (start < end) {
          result.insert_range(start, end);
        }
      }
    }
    return result;
  }

  /**
   * @brief Computes the difference of this set with another
   * @param other The set to subtract
   * @return A new set containing elements that are in this set but not in other
   */
  IndexSet operator-(const IndexSet& other) const {
    IndexSet result = *this;  // Start with a copy of this set

    // Remove each range from the other set
    for (const auto& range : other.ranges_) {
      for (std::size_t i = range.start; i < range.end; ++i) {
        result.remove(i);
      }
    }
    return result;
  }

  /**
   * @brief Computes the symmetric difference of this set with another
   * @param other The other set
   * @return A new set containing elements that are in either set but not both
   */
  IndexSet operator^(const IndexSet& other) const {
    IndexSet result;

    // Get the union and intersection
    IndexSet union_set = *this | other;
    IndexSet intersect_set = *this & other;

    // Symmetric difference is union minus intersection
    result = union_set - intersect_set;

    return result;
  }

  /**
   * @brief Checks if this set is a subset of another
   * @param other The potential superset
   * @return true if this set is a subset of other
   */
  bool operator<=(const IndexSet& other) const {
    // For each index in this set, check if it's in other
    for (const auto& range : ranges_) {
      for (std::size_t i = range.start; i < range.end; ++i) {
        if (!other.contains(i)) return false;
      }
    }
    return true;
  }

  /**
   * @brief Checks if this set is a superset of another
   * @param other The potential subset
   * @return true if this set is a superset of other
   */
  bool operator>=(const IndexSet& other) const { return other <= *this; }

  /**
   * @brief Checks if this set is the same as another
   * @param other The set to check against
   * @return true if this set is the same as other
   */
  bool operator==(const IndexSet& other) const {
    // If sizes are different, they can't be equal
    if (total_size_ != other.total_size_) return false;
    
    // If number of ranges is different, they can't be equal
    if (ranges_.size() != other.ranges_.size()) return false;
    
    // Check that all ranges match (assuming both are sorted)
    for (size_t i = 0; i < ranges_.size(); i++) {
      if (ranges_[i] != other.ranges_[i]) return false;
    }
    
    return true;
  }

  // Iterator methods
  iterator begin() { return iterator(ranges_); }
  iterator end() { return iterator(ranges_, ranges_.size(), 0); }
  const_iterator begin() const { return const_iterator(ranges_); }
  const_iterator end() const {
    return const_iterator(ranges_, ranges_.size(), 0);
  }
  const_iterator cbegin() const { return const_iterator(ranges_); }
  const_iterator cend() const {
    return const_iterator(ranges_, ranges_.size(), 0);
  }
  [[nodiscard]] const_pair_iterator cbegin_pair() const { return const_pair_iterator(ranges_); }
  [[nodiscard]] const_pair_iterator cend_pair() const { return const_pair_iterator(ranges_, ranges_.size()); }

 private:
  // Internal representation - vector of ranges
  // Each range is represented as [start, end) pair where end is exclusive
  std::vector<IndexRange> ranges_;
  std::size_t total_size_ = 0;  // Cache of total number of indices

  /**
   * @brief Merges any overlapping or adjacent ranges
   *
   * This function is called after insertions to maintain the invariant
   * that ranges are non-overlapping and non-adjacent. It first sorts
   * the ranges by start position, then merges any ranges that overlap
   * or are adjacent.
   */
  void merge_overlapping_ranges() {
    if (ranges_.empty()) return;

    // Sort ranges by start position to ensure we can merge in one pass
    std::sort(ranges_.begin(), ranges_.end());

    // Create new vector for merged ranges
    std::vector<IndexRange> merged;
    merged.reserve(ranges_.size());  // Reserve space to avoid reallocations
    merged.push_back(ranges_[0]);

    for (std::size_t i = 1; i < ranges_.size(); i++) {
      auto& back = merged.back();
      // Check if ranges are adjacent (back.end == ranges_[i].start - 1)
      // or overlapping (back.end >= ranges_[i].start)
      if (back.end >= ranges_[i].start - 1) {
        // Merge the ranges by extending the end of the previous range
        back.end = std::max(back.end, ranges_[i].end);
        back.second = back.end;  // Update second as well
      } else {
        // Ranges are not adjacent or overlapping, add as new range
        merged.push_back(ranges_[i]);
      }
    }

    ranges_ = std::move(merged);
  }

  /**
   * @brief Finds the range that contains the given index, or returns NO_RANGE if not found
   * @param index The index to find
   * @return The index of the containing range, or NO_RANGE if not found
   */
  std::size_t find_range_index(std::size_t index) const {
    // Use binary search to find the range that could contain this index
    auto it = std::lower_bound(ranges_.begin(), ranges_.end(), index,
                            [](const auto& range, std::size_t val) {
                              return range.end <= val;
                            });
    
    // Check if we found a valid range that contains the index
    if (it != ranges_.end() && it->start <= index && index < it->end) {
      return std::distance(ranges_.begin(), it);
    }
    
    return NO_RANGE;
  }
};

}  // namespace cse
