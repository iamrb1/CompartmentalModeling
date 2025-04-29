/* --- ComboManager.hpp --- */

/* ------------------------------------------
Author: Amery Johnson, Devon FoxElster, Ephraim Bennett
Last Changed Date: 03/26/2025
------------------------------------------ */

#ifndef COMBOMANAGER_H
#define COMBOMANAGER_H

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <vector>

template <typename Container>
concept supportedContainer = requires(Container& testContainer) {
  std::begin(testContainer);
  std::end(testContainer);
};

namespace cse {

/**
 * @brief ComboManager class template.
 *
 * This class manages combinations of items from a container. It allows stepping
 * through combinations in lexicographical order via NextCombo() and
 * PrevCombo(). The initial combination is available immediately after
 * construction or Reset().
 *
 * Template parameter:
 *   Container - the type of the container holding the items.
 */
template <supportedContainer Container>
class ComboManager {
 public:
  // The type of elements stored in the container.
  using ValueType = typename Container::value_type;

  /**
   * @brief Constructs a ComboManager without a required element.
   *
   * @param container The container of items.
   * @param combinationSize The number of items per combination.
   * @param allowRepeats If true, combinations with repeated elements are
   * allowed.
   * @param indexRequired The index of the element that is required to appear in
   * every combination.
   * @throws assertion failure: if combinationSize is greater than the number
   * of items.
   * @throws std::invalid_argument: if the required element index is out of
   * range
   */
  ComboManager(const Container& container, std::size_t combinationSize,
               bool allowRepeats = false,
               std::optional<std::size_t> indexRequired = std::nullopt)
      : items_(std::begin(container),
               std::end(container)), /* Copy items from the container into a
                                        vector for random access.*/
        numItems_(items_.size()),
        comboSize_(combinationSize),
        allowRepeats_(allowRepeats),
        isRequired_((indexRequired != std::nullopt)),
        totalCombinations_(0) {

    if (comboSize_ > numItems_) {
      totalCombinations_ = 0;
      return;
    }

    if (isRequired_) {
      if (*indexRequired >= items_.size()) {
        throw std::invalid_argument(
            "Required Element Index is out of container limits");
      }
      requiredValue_ = items_.at(*indexRequired);

      if (!allowRepeats_) {
        auto foundRequiredValue = std::ranges::find(items_, requiredValue_);
        items_.erase(foundRequiredValue);
      }

      --comboSize_;
      numItems_ = items_.size();
    }

    totalCombinations_ =
        (allowRepeats_)
            ? BinomialCoefficient_(numItems_ + comboSize_ - 1, comboSize_)
            : BinomialCoefficient_(numItems_, comboSize_);
    indices_.resize(comboSize_);
    Reset();
  }

  // https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
  // for the setup and structure of the custom iterator
  /**
   * @brief Const Bidirectional Iterator for the ComboManager Class
   *
   * @details Iterator that can traverse the available combos in ComboManager
   * both forward and backward, but cannot edit the individual combinations
   */
  struct Iterator {
    using iterator_category = std::bidirectional_iterator_tag;

    /**
     * @brief Iterator constructor
     *
     * @param comboManager Reference to the current ComboManager instance
     * @param begin Bool indicating whether this points before the first combo
     * @param end Bool indicating whether this points after the last combo
     */
    Iterator(ComboManager& comboManager, bool begin = false, bool end = false)
        : manager_(comboManager), pastBegin_(begin), pastEnd_(end) {
      combination_ = manager_.GetCurrentCombo();
    }

    /**
     * @brief Prefix Increment Operator
     *
     * @return returns itself after incrementation
     */
    Iterator& operator++() {
      // Increment normally
      // If no further combos, mark as past end
      if (manager_.NextCombo()) {
        combination_ = manager_.GetCurrentCombo();
      } else {
        pastEnd_ = true;
      }
      // It should never point before begin after this
      pastBegin_ = false;
      return *this;
    }

    /**
     * @brief Postfix Increment Operator
     *
     * @return returns the previous iterator version after incrementing
     */
    Iterator operator++(int) {
      auto temporaryReturn = *this;
      // Implementation between the two is identical save the temp return
      ++(*this);
      return temporaryReturn;
    }

    /**
     * @brief Prefix Decrement Operator
     *
     * @return returns itself after decrementation
     */
    Iterator& operator--() {
      // If marked as past end, decrement it to the last combo
      // Otherwise use PrevCombo
      // If it is the first combo then mark it as past the begin
      if (pastEnd_) {
        AdjustToEnd_();
      } else if (manager_.PrevCombo()) {
        combination_ = manager_.GetCurrentCombo();
      } else {
        pastBegin_ = true;
      }
      // It should never be past end after this function
      pastEnd_ = false;
      return *this;
    }

    /**
     * @brief Postfix Decrement Operator
     *
     * @return returns the previous iterator version after decrementing
     */
    Iterator operator--(int) {
      auto temporaryReturn = *this;
      // Implementation between the two is identical save the temp return
      --(*this);
      return temporaryReturn;
    }

    /**
     * @brief Dereference Operator for the ComboManager Iterator
     *
     * @return Reference to the current combination
     */
    const std::vector<ValueType>& operator*() const {
      // No dereferencing iterators that do not point at appropriate
      // combinations
      if (pastBegin_) {
        throw std::out_of_range(
            "This iterator points before the combination range");
      } else if (pastEnd_) {
        throw std::out_of_range(
            "This iterator points after the combination range");
      }
      return combination_;
    }

    /**
     * @brief == Operator for two Iterators
     */
    friend bool operator==(const Iterator& left, const Iterator& right) {
      /*
        Boolean Logic: both Begin and End booleans should line up
                      (otherwise one is at the begin/end and the other isn't)
        If they line up, we confirm that either they are both Begin, both are
        End, or both have the same combination

        In that order, as they can have the 'same combination' but one points
        beyond the limits of the ComboManager so we rule out those first
      */
      return (left.pastBegin_ == right.pastBegin_ &&
              left.pastEnd_ == right.pastEnd_ &&
              (left.pastBegin_ || left.pastEnd_ ||
               left.combination_ == right.combination_));
    };

    /**
     * @brief != Operator for two Iterators
     */
    friend bool operator!=(const Iterator& left, const Iterator& right) {
      // Simple inverse of the == operator
      return (!(left.pastBegin_ == right.pastBegin_ &&
                left.pastEnd_ == right.pastEnd_ &&
                (left.pastBegin_ || left.pastEnd_ ||
                 left.combination_ == right.combination_)));
    };

   private:
    /// @brief Reference to the ComboManager
    ComboManager& manager_;
    /// @brief Current combination
    std::vector<ValueType> combination_;
    /// @brief Indicates whether the Iterator is before the first combination
    /// (or == rbegin())
    bool pastBegin_ = false;
    /// @brief Indicates whether the Iterator is after scope (or == end())
    bool pastEnd_ = false;

    /**
     * @brief Helper function to increment the iterator to the last combination
     */
    void AdjustToEnd_() {
      pastEnd_ = false;
      while (!pastEnd_) {
        ++(*this);
      }
    }
  };

  /**
   * @brief Iterator for the first combination
   */
  Iterator begin() {
    Reset();
    return Iterator(*this);
  }

  Iterator rbegin() { return Iterator(*this, true); }

  /**
   * @brief Iterator signifying the end of available combinations
   */
  Iterator end() { return Iterator(*this, false, true); }

  Iterator rend() { return --Iterator(*this, false, true); }

  /**
   * @brief Resets the combination to the first combination.
   *
   * If repeating elements are allowed, the first combination is [0,0,...,0].
   * Otherwise, it is [0,1,2,...,comboSize_-1].
   */
  constexpr void Reset() {
    if (allowRepeats_) {
      std::ranges::fill(indices_, 0);
    } else {
      std::iota(indices_.begin(), indices_.end(), 0);
    }
  }

  /**
   * @brief Returns the current combination as a vector of items.
   *
   * @return A vector containing the current combination.
   */
  std::vector<ValueType> GetCurrentCombo() const {
    std::vector<ValueType> combo(indices_.size());
    // Transform the indices into actual items from the container.
    std::ranges::transform(indices_, combo.begin(),
                           [this](std::size_t index) { return items_[index]; });

    if (isRequired_) {
      combo.push_back(requiredValue_);
    }
    return combo;
  }

  /**
   * @brief Advances to the next combination.
   *
   * @return true if the next combination was generated, false if no further
   * combination exists.
   */
  bool NextCombo() {
    if (totalCombinations_ == 0) {
      return false; // No combinations to generate
    }
    if (allowRepeats_) {
      // For repeating combinations, the elements must be non-decreasing.
      // Start from the rightmost index for repeating combinations.
      for (int i = static_cast<int>(comboSize_) - 1; i >= 0; --i) {
        // If the current index is less than numItems_ - 1, it can be
        // incremented.
        if (indices_[i] < numItems_ - 1) {
          ++indices_[i];
          // Set all indices to the right of i equal to the new value to
          // maintain non-decreasing order.
          for (std::size_t j = i + 1; j < comboSize_; ++j) {
            indices_[j] = indices_[i];
          }
          return true;
        }
      }
      // If no index can be incremented, we've reached the final repeating
      // combination.
      return false;
    } else {
      int i = static_cast<int>(comboSize_) - 1;
      // Find the first index from the right that can be incremented.
      // The condition indices_[i] == i + numItems_ - comboSize_ indicates that
      // the current index is at its maximum allowable value.
      while (i >= 0 && indices_[i] == i + numItems_ - comboSize_) {
        --i;  // Move to the left if the current index is "maxed out".
      }
      // If no index can be incremented, we've reached the final combination.
      if (i < 0) {
        return false;
      }
      ++indices_[i];

      UpdateIndicesFrom_(i);
      return true;
    }
  }

  /**
   * @brief Moves to the previous combination.
   *
   * @return true if the previous combination was generated, false if already at
   * the first combination.
   */
  bool PrevCombo() {
    if (totalCombinations_ == 0) {
      return false; // No combinations to generate
    }
    // If the current combination is the first one, return false.
    if (IsFirstCombo_()) {
      return false;
    }
    if (allowRepeats_) {
      for (int i = static_cast<int>(comboSize_) - 1; i >= 0; --i) {
        // Determine the minimum allowed value:
        // For index 0 it is 0, for subsequent indices it is the value of the
        // previous index.
        std::size_t minValue = (i == 0 ? 0 : indices_[i - 1]);
        if (indices_[i] > minValue) {
          --indices_[i];
          // Set all indices to the right to the maximum allowed value
          // (numItems_ - 1) to form the previous combination.
          for (std::size_t j = i + 1; j < comboSize_; ++j) {
            indices_[j] = numItems_ - 1;
          }
          return true;
        }
      }
      // If no index can be decremented, we've reached the first repeating
      // combination.
      return false;
    } else {
      int i = static_cast<int>(comboSize_) - 1;
      // Find the first index from the right that can be decremented.
      // For each index, the minimum allowed value is:
      //   - 0 for the first index, or
      //   - one more than the previous index for subsequent indices.
      while (i >= 0) {
        std::size_t minValue = (i == 0) ? 0 : (indices_[i - 1] + 1);
        if (indices_[i] > minValue) {  // This index can be decremented.
          break;
        }
        --i;  // Move left if the current index is at its minimum allowable
              // value.
      }
      // Should not happen if IsFirstCombo_() is false, but check nonetheless.
      if (i < 0) {
        return false;
      }
      --indices_[i];

      UpdateIndicesFrom_(i, true);
      return true;
    }
  }

  /**
   * @brief Returns the total number of possible combinations.
   *
   * @return The total number of combinations (n choose k).
   */
  std::size_t TotalCombinations() const noexcept { return totalCombinations_; }

  /**
   * @brief Returns the current combination indices.
   *
   * @return A vector of indices representing the current combination.
   */
  constexpr const std::vector<std::size_t>& GetCurrentIndices() const noexcept {
    return indices_;
  }

 private:
  /**
   * @brief Computes the binomial coefficient "n choose k".
   *
   * @param n Total number of items.
   * @param k Number of items per combination.
   * @return The binomial coefficient.
   */
  static constexpr std::size_t BinomialCoefficient_(std::size_t n,
                                                    std::size_t k) noexcept {
    if (k > n) {
      return 0;
    }
    if (k == 0 || k == n) {
      return 1;
    }
    // Use symmetry: C(n, k) == C(n, n - k). This reduces the number of
    // iterations.
    if (k > n - k) {
      k = n - k;
    }
    std::size_t result = 1;
    // Iteratively compute the binomial coefficient.
    // The loop multiplies by (n - k + i) and divides by i at each step.
    // This method avoids computing large factorials directly.
    for (std::size_t i = 1; i <= k; ++i) {
      result *= n - k + i;  // Multiply by the next term in the numerator.
      result /= i;  // Divide by the corresponding term in the denominator.
    }
    return result;
  }

  /**
   * @brief Helper function to update indices starting from a given position.
   *
   * @details After an index is changed, this function resets all subsequent
   * indices to maintain the correct order depending on if it is generating
   * forward or in reverse
   *
   * If Forward: lexicographical order (i.e. indices[j] = indices[j - 1] + 1 for
   * j > i). Will result in each subsequent index being 1 higher than the last
   *
   * If Reverse: n - k + j. Will result in indices in descending order from the
   * maximum possible index, stopping when it reaches the i index
   *
   * @param i The position after which indices should be updated.
   * @param reverse Defines whether indices should be calculated for reverse
   * combinations or for forward combinations
   */
  void UpdateIndicesFrom_(std::size_t i, bool reverse = false) noexcept {
    for (std::size_t j = i + 1; j < comboSize_; ++j) {
      // If not reverse, use lexicographical order
      // Else calculate the reverse of lexicographical
      indices_[j] =
          (!reverse) ? indices_[j - 1] + 1 : numItems_ - comboSize_ + j;
    }
  }

  /**
   * @brief Helper function to check if the current combination is the first
   * one.
   *
   * @details
   * If Repeating: First Combo = [0,0,..0] of length k
   * If Not Repeating: First Combo = [0,1,..k-1]
   *
   * @return true if the current combination equals the first combo; false
   * otherwise.
   */
  constexpr bool IsFirstCombo_() const noexcept {
    std::string firstCombo;
    if (allowRepeats_) {
      std::ranges::fill(firstCombo, 0);
    } else {
      std::iota(firstCombo.begin(), firstCombo.end(), 0);
    }
    return std::ranges::equal(firstCombo, indices_);
  }

  // Container items stored in a vector for random access.
  std::vector<ValueType> items_;

  // Total number of items.
  std::size_t numItems_;

  // Number of items per combination.
  std::size_t comboSize_;

  // The current combination, stored as indices into items_.
  std::vector<std::size_t> indices_;

  // Flag indicating if repeating elements are allowed.
  bool allowRepeats_ = false;

  // If there is an element required within the combinations
  bool isRequired_ = false;

  // Value that is required
  ValueType requiredValue_;

  // Precomputed total number of combinations (n choose k).
  std::size_t totalCombinations_;
};

}  // namespace cse

#endif  // COMBOMANAGER_H