#include "../include/cse/index_set.hpp"
#include <algorithm>
#include <cassert>

namespace cse {

void IndexSet::insert(std::size_t index) {
    // First check if the index is already in a range to avoid duplicate work
    auto range_idx = find_range_index(index);
    if (range_idx < ranges_.size() && 
        ranges_[range_idx].first <= index && 
        ranges_[range_idx].second > index) {
        return; // Already in set
    }

    // Track old size before merging to determine if we need to increment total_size_
    std::size_t old_total = 0;
    for (const auto& range : ranges_) {
        old_total += range.second - range.first;
    }

    // Insert new range [index, index+1)
    ranges_.push_back({index, index + 1});
    
    // Merge any overlapping ranges - this will handle cases where the new index
    // is adjacent to or overlaps with existing ranges
    merge_overlapping_ranges();

    // Recalculate total size after merging
    total_size_ = 0;
    for (const auto& range : ranges_) {
        total_size_ += range.second - range.first;
    }

    // If total size didn't increase after merging, we must have merged with an existing range
    // In this case, we need to increment the size since we added a new index
    if (total_size_ == old_total) {
        total_size_++;
    }
}

void IndexSet::remove(std::size_t index) {
    // Find the range containing this index
    auto range_idx = find_range_index(index);
    if (range_idx >= ranges_.size() || 
        ranges_[range_idx].first > index || 
        ranges_[range_idx].second <= index) {
        return; // Index not in set
    }

    auto& range = ranges_[range_idx];
    if (range.first == index) {
        // Case 1: Removing from start of range
        range.first++;
        if (range.first == range.second) {
            // Range became empty, remove it
            ranges_.erase(ranges_.begin() + range_idx);
        }
    } else if (range.second == index + 1) {
        // Case 2: Removing from end of range
        range.second--;
    } else {
        // Case 3: Removing from middle of range - need to split into two ranges
        auto second_half = std::make_pair(index + 1, range.second);
        range.second = index;
        ranges_.insert(ranges_.begin() + range_idx + 1, second_half);
    }

    total_size_--;
}

bool IndexSet::contains(std::size_t index) const {
    // Find potential containing range and verify index is within its bounds
    auto range_idx = find_range_index(index);
    return range_idx < ranges_.size() && 
           ranges_[range_idx].first <= index && 
           ranges_[range_idx].second > index;
}

std::size_t IndexSet::size() const {
    return total_size_;
}

void IndexSet::merge_overlapping_ranges() {
    if (ranges_.empty()) return;

    // Sort ranges by start position to ensure we can merge in one pass
    std::sort(ranges_.begin(), ranges_.end());

    // Create new vector for merged ranges
    std::vector<std::pair<std::size_t, std::size_t>> merged;
    merged.reserve(ranges_.size());  // Reserve space to avoid reallocations
    merged.push_back(ranges_[0]);

    for (std::size_t i = 1; i < ranges_.size(); i++) {
        auto& back = merged.back();
        // Check if ranges are adjacent (back.second == ranges_[i].first - 1)
        // or overlapping (back.second >= ranges_[i].first)
        if (back.second >= ranges_[i].first - 1) {
            // Merge the ranges by extending the end of the previous range
            back.second = std::max(back.second, ranges_[i].second);
        } else {
            // Ranges are not adjacent or overlapping, add as new range
            merged.push_back(ranges_[i]);
        }
    }

    ranges_ = std::move(merged);
}

std::size_t IndexSet::find_range_index(std::size_t index) const {
    // Use binary search to find the range that could contain this index
    // The comparison function looks at range.second because we store ranges
    // as [start, end) intervals
    return std::lower_bound(ranges_.begin(), ranges_.end(), index,
        [](const auto& range, std::size_t val) {
            return range.second <= val;
        }) - ranges_.begin();
}

std::vector<std::size_t> IndexSet::getAllIndices() const {
    std::vector<std::size_t> result;
    result.reserve(total_size_);  // Preallocate to avoid reallocations
    
    // Expand each range into individual indices
    for (const auto& range : ranges_) {
        for (std::size_t i = range.first; i < range.second; ++i) {
            result.push_back(i);
        }
    }
    
    return result;
}

std::optional<std::pair<std::size_t, std::size_t>> IndexSet::getNextRange(std::size_t index) const {
    auto range_idx = find_range_index(index);
    
    // If index is in a range, we want the next range
    if (range_idx < ranges_.size() && ranges_[range_idx].first <= index && ranges_[range_idx].second > index) {
        range_idx++;
    }
    
    // Return next range if it exists
    if (range_idx < ranges_.size()) {
        return ranges_[range_idx];
    }
    return std::nullopt;
}

std::optional<std::pair<std::size_t, std::size_t>> IndexSet::getPrevRange(std::size_t index) const {
    auto range_idx = find_range_index(index);
    
    // Handle edge cases
    if (range_idx == 0 || ranges_.empty()) {
        return std::nullopt;
    }
    
    // If index is in current range, return previous range
    if (range_idx < ranges_.size() && ranges_[range_idx].first <= index && ranges_[range_idx].second > index) {
        if (range_idx == 0) return std::nullopt;
        return ranges_[range_idx - 1];
    }
    
    // Otherwise return the range before the found position
    return ranges_[range_idx - 1];
}

std::optional<std::pair<std::size_t, std::size_t>> IndexSet::getContainingRange(std::size_t index) const {
    auto range_idx = find_range_index(index);
    
    // Verify the range actually contains the index
    if (range_idx < ranges_.size() && 
        ranges_[range_idx].first <= index && 
        ranges_[range_idx].second > index) {
        return ranges_[range_idx];
    }
    return std::nullopt;
}

} // namespace cse 