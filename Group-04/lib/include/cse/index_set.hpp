#pragma once

#include <cstddef>
#include <vector>
#include <optional>
#include <utility>
#include <iterator>

namespace cse {

/**
 * @brief A class that efficiently stores and recalls ranges of numbers in a set.
 * 
 * IndexSet provides an efficient way to store sets of integers by maintaining them
 * as ranges. For example, the sequence 1,2,3,5,6,7 would be stored as two ranges:
 * [1,4) and [5,8). This is particularly efficient when storing consecutive numbers.
 * 
 * The class automatically merges adjacent or overlapping ranges and splits ranges
 * when removing elements. All ranges are stored in the half-open interval format
 * [start, end) where end is exclusive.
 */
class IndexSet {
public:
    // Forward declarations of iterator types for future implementation
    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;

    // Constructors
    IndexSet() = default;
    ~IndexSet() = default;

    /**
     * @brief Adds an index to the set
     * @param index The index to add
     * 
     * If the index is adjacent to or within an existing range, the ranges will
     * be automatically merged. For example, if [1,4) exists and we insert 4,
     * the range becomes [1,5).
     */
    void insert(std::size_t index);

    /**
     * @brief Removes an index from the set
     * @param index The index to remove
     * 
     * If the index is in the middle of a range, the range will be split.
     * For example, if [1,5) exists and we remove 3, we get [1,3) and [4,5).
     */
    void remove(std::size_t index);

    /**
     * @brief Checks if an index is in the set
     * @param index The index to check
     * @return true if the index is in the set, false otherwise
     */
    bool contains(std::size_t index) const;

    /**
     * @brief Returns the total number of indices in the set
     * @return The sum of the sizes of all ranges
     */
    std::size_t size() const;

    /**
     * @brief Returns a vector containing all indices in the set
     * @return Vector of indices in ascending order
     */
    std::vector<std::size_t> getAllIndices() const;

    /**
     * @brief Gets the next range after the given index
     * @param index The reference index
     * @return The next range if it exists, std::nullopt otherwise
     * 
     * If the index is within a range, returns the next range after the
     * containing range. If the index is between ranges, returns the next range.
     */
    std::optional<std::pair<std::size_t, std::size_t>> getNextRange(std::size_t index) const;

    /**
     * @brief Gets the previous range before the given index
     * @param index The reference index
     * @return The previous range if it exists, std::nullopt otherwise
     */
    std::optional<std::pair<std::size_t, std::size_t>> getPrevRange(std::size_t index) const;

    /**
     * @brief Gets the range containing the given index
     * @param index The index to find
     * @return The containing range if it exists, std::nullopt otherwise
     */
    std::optional<std::pair<std::size_t, std::size_t>> getContainingRange(std::size_t index) const;

    // Iterator support - to be implemented
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

private:
    // Internal representation - vector of ranges
    // Each range is represented as [start, end) pair where end is exclusive
    std::vector<std::pair<std::size_t, std::size_t>> ranges_;
    std::size_t total_size_ = 0;  // Cache of total number of indices

    /**
     * @brief Merges any overlapping or adjacent ranges
     * 
     * This function is called after insertions to maintain the invariant
     * that ranges are non-overlapping and non-adjacent. It first sorts
     * the ranges by start position, then merges any ranges that overlap
     * or are adjacent.
     */
    void merge_overlapping_ranges();

    /**
     * @brief Finds the index of the range that could contain the given index
     * @param index The index to find
     * @return The index of the potential containing range
     * 
     * Uses binary search to efficiently find the range that could contain
     * the given index. The returned range needs to be checked to confirm
     * it actually contains the index.
     */
    std::size_t find_range_index(std::size_t index) const;
};

} // namespace cse 