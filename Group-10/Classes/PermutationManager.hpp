/* --- PermutationManager.hpp --- */

/* ------------------------------------------
Author: Ephraim Bennett
Last Changed Date: 3/25/2025
------------------------------------------ */

#ifndef PERMUTATION_MANAGER_H
#define PERMUTATION_MANAGER_H

#include <algorithm>
#include <cassert>
#include <concepts>
#include <functional>
#include <iostream>
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
 * @brief PermutationManager class template.
 *
 * This class creates the permutation of items from a container. Stepping will
 * be done through its composite class. The initial combination is available
 * immediately after construction or Reset().
 *
 * Template parameter:
 *   Container - the type of the container holding the items.
 */

template <supportedContainer Container>
class PermutationManager {
 public:
  // The type of elements stored in the container.
  using ValueType = typename Container::value_type;

  /**
   * @brief constructor for PermutationManager
   *
   * @param container the container of items
   * @param permutation_size the size of the items chosen at once.
   * @param repeat boolean indicating whether elements can repeat themselves
   */
  PermutationManager(const Container& container, size_t permutation_size,
                     bool repeat)
      : items_(std::begin(container), std::end(container)),
        numItems_(container.size()),
        comboSize_(permutation_size),
        currentPermutation_(),
        isRequired_(false),
        isRepeating_(repeat) {
    assert(comboSize_ <= numItems_ &&
           "Combination size cannot be greater than the number of items in the "
           "container.");

    totalPermutations_ = PermutationNumber(numItems_, comboSize_);

    Reset();
  }

  /**
   * @brief overloaded constructor for PermutationManager - for a required
   * value.
   *
   * @param container the container of items
   * @param permutation_size k, or the size of the items chosen at once.
   * @param repeat boolean indicating whether elements can repeat themselves
   * @param requiredIndex
   */
  PermutationManager(const Container& container, size_t permutation_size,
                     bool repeat, size_t requiredIndex)
      : items_(std::begin(container), std::end(container)),
        numItems_(container.size()),
        comboSize_(permutation_size),
        currentPermutation_(),
        isRequired_(true),
        requiredIndex_(requiredIndex),
        isRepeating_(repeat) {
    if (requiredIndex_ >= items_.size()) {
      throw std::invalid_argument(
          "Required Element Index is out of container limits");
    }

    if (comboSize_ > numItems_) {
      throw std::invalid_argument(
          "Combination size cannot be greater than the number of items in the "
          "container.");
    }

    if (!isRepeating_) {
      requiredValue_ = items_.at(requiredIndex);
      items_.erase(std::remove(items_.begin(), items_.end(), requiredValue_));
    }
    --comboSize_;
    numItems_ = items_.size();

    requiredIndex_ = 0;

    totalPermutations_ = PermutationNumber(numItems_, comboSize_);

    Reset();
  }

  /**
   * @brief resets the permutation generator to the initial state
   */
  void Reset() {
    currentIndex_ = 0;
    currentPermutation_.resize(comboSize_);
    indices_.clear();

    // the initial permutation is just the first k items
    for (size_t i = 0; i < comboSize_; ++i) {
      currentPermutation_[i] = items_[i];
      indices_.push_back(i);
    }
  }

  /**
   * @brief generates the next permutation
   * @return false if we've reached the end, true otherwise
   */
  bool Next() {
    // end condition - only if we aren't repeating elements
    if (!isRepeating_) {
      if (currentIndex_ >= totalPermutations_ - 1) return false;
    }

    // either generate the next permutation w/the std algorithm (if n == k) or
    // make a k permutation
    if (isRequired_) {
      if (numItems_ == comboSize_) {
        std::next_permutation(indices_.begin(), indices_.end());
      } else if (requiredIndex_ < comboSize_) {  //
        ++requiredIndex_;
      } else {
        // handle required elements here...
        if (!NextKPermutation_()) {
          return false;
        }
        requiredIndex_ = 0;
      }
    } else {
      if (numItems_ == comboSize_) {
        std::next_permutation(indices_.begin(), indices_.end());
      } else {
        // handle required elements here...
        if (!NextKPermutation_()) {
          return false;
        }
      }
    }

    for (size_t i = 0; i < comboSize_; ++i) {
      currentPermutation_[i] = items_[indices_[i]];
    }

    ++currentIndex_;
    return true;
  }

  /**
   * @brief generates the lexographically lower permutation
   * @return - true if it exists, false otherwise
   */
  bool Prev() {
    // end condition
    if (currentIndex_ == 0) return false;

    if (isRequired_) {
      // do it w/the standard lib algorithm if n == k
      if (numItems_ == comboSize_) {
        std::prev_permutation(indices_.begin(), indices_.end());
      } else if (requiredIndex_ >
                 0) {  // at zero we're at the last perm for this combo.
        --requiredIndex_;
      } else {
        if (!PrevKPermutation_()) return false;
        requiredIndex_ = comboSize_;
      }
    } else {
      // do it w/the standard lib algorithm if n == k
      if (numItems_ == comboSize_) {
        std::prev_permutation(indices_.begin(), indices_.end());
      } else {
        if (!PrevKPermutation_()) {
          return false;
        }
      }
    }

    for (size_t i = 0; i < comboSize_; ++i) {
      currentPermutation_[i] = items_[indices_[i]];
    }

    --currentIndex_;
    return true;
  }

  /**
   * @brief getter for the current permutation
   * @return vector size k of the value type
   */
  std::vector<ValueType> GetCurrentPermutation() {
    if (isRequired_) {
      auto temp = currentPermutation_;
      temp.insert(temp.begin() + requiredIndex_, requiredValue_);
      return temp;
    }

    return currentPermutation_;
  }

  /**
   * @brief calculates the total number of permutatins
   * @param n - the number of items
   * @param k - the number of items we choose
   * @return size_t for the number
   */
  size_t PermutationNumber(std::size_t n, std::size_t k) {
    return Factorial_(n) / Factorial_(n - k);
  }

  /**
   * @brief getter for total permutations
   * @return the total number of permutations for this container, given no
   * repeating or required
   */
  size_t TotalPermutations() { return totalPermutations_; }

  /**
   * @brief - Setter for the repeating attribute
   * @param repeating - if it's repearting or not
   */
  void SetRepeating(bool repeating) { isRepeating_ = repeating; }

  /**
   * @brief - Getter for the repeating attribute
   * @return true if repeating, false otherwise
   */
  bool GetRepeating() { return isRepeating_; }

 private:
  // Container items stored in a vector for random access.
  std::vector<ValueType> items_;

  // Total number of items.
  std::size_t numItems_;

  // Items chosen
  std::size_t comboSize_;

  // Total Permutations
  std::size_t totalPermutations_;

  // The current permutation
  std::size_t currentIndex_;

  // Indices which indicate which items are in the current permutation
  std::vector<size_t> indices_;

  // The current permutation
  std::vector<ValueType> currentPermutation_;

  // Determines if we have any required values
  bool isRequired_;

  // The value that is required
  ValueType requiredValue_;

  // The index of the required value
  size_t requiredIndex_;

  // Indicates if we allow indices to be repeated in each combination
  bool isRepeating_;

  /**
   * @brief same as next k permutation but in the opposite direction
   * @return true of a previous permutation was found, false otherwise
   */
  bool PrevKPermutation_() {
    if (std::prev_permutation(indices_.begin(), indices_.end())) return true;

    // sort them in ascending, this allows us to find the "largest" index that
    // can be decremented.
    std::sort(indices_.begin(), indices_.end());

    // use long long instead of size_t because it could be negative
    long long i = static_cast<long long>(comboSize_) - 1;
    if (isRepeating_) {
      while (i >= 0) {
        /**
         * Find the first index from the left that can be decremented.
         * 4, 4, 4 => 3, 4, 4 => 3, 3, 4 => 3, 3, 4
         * The min is either the previous value, or 0 if i == 9
         */
        size_t min = (i == 0) ? 0 : indices_[i - 1];

        if (indices_[i] > min) {
          break;
        }
        --i;
      }

    } else {
      // Find the first index from the right that can be decremented. 2, 3, 4 =>
      // 1, 3, 4 => 1, 2, 4 => 1, 2, 3 3, 4 => 2, 5 For each index, the minimum
      // allowed value is:
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
    }

    // If no index can be incremented, we've reached the final combination.
    if (i == -1) {
      return false;
    }

    // Decrement the identified index.
    --indices_[i];

    if (isRepeating_) {
      // we want to max out all the values past the one we've decremented
      for (size_t j = i + 1; j < comboSize_; ++j) {
        indices_[j] = numItems_ - 1;
      }
    } else {
      for (size_t j = i + 1; j < comboSize_; ++j) {
        indices_[j] = numItems_ - comboSize_ + j;
      }
    }

    // We want to sort by descending here, becuase we want the largest
    // lexographical combination to be given to the std::prev_permutation
    // algorithm
    std::sort(indices_.begin(), indices_.end(), std::greater<size_t>());

    return true;
  }

  /**
   * @brief this function generates the next K permutation
   * The general idea is that we either just permute the existing indices again,
   * OR that we need a new combination Because in theory we are just stepping
   * through each combination, and grabbing each permutation of such so much of
   * this algorithm will look similar to the one in combo manager
   * @return true if there was a new permutation found, false otherwise
   */
  bool NextKPermutation_() {
    // check if we can permute the existing set
    if (std::next_permutation(indices_.begin(), indices_.end())) return true;

    std::sort(indices_.begin(), indices_.end());

    // use long long instead of size_t because it could be negative
    long long i = static_cast<long long>(comboSize_) - 1;

    // Find the first index from the right that can be incremented.
    if (isRepeating_) {
      /**
       * 1, 2, 3, 4
       * 2, 3
       * n - k + (i == 0) = 2
       */
      // This allows the same indices to be included more than once in our combo
      while (i >= 0 && indices_[i] == numItems_ - 1) {
        --i;
      }

    } else {
      // The condition indices_[i] == i + numItems_ - comboSize_ indicates that
      // the current index is at its maximum allowable value.
      while (i >= 0 && indices_[i] == numItems_ - comboSize_ + i) {
        --i;  // Move to the left if the current index is "maxed out".
      }
    }

    // If no index can be incremented, we've reached the final combination.
    if (i == -1) {
      return false;
    }

    // upgrade the valid indice
    ++indices_[i];

    if (isRepeating_) {
      // For every index after i, set t i's index so we get a different
      // combination
      for (size_t j = i + 1; j < comboSize_; ++j) {
        indices_[j] = indices_[i];
      }
    } else {
      // For every index position after i, set it to be one greater than its
      // predecessor.
      for (std::size_t j = i + 1; j < comboSize_; ++j) {
        indices_[j] = indices_[j - 1] + 1;
      }
    }

    return true;
  }

  /**
   * @brief static helper function that is used to calculate permutation size
   * @param x - the number we are performing the factorial operation on
   * @return x!
   */
  static size_t Factorial_(size_t x) {
    if (x == 0 || x == 1) return 1;

    size_t res = 1;
    for (size_t i = 2; i <= x; ++i) {
      res *= i;
    }
    return res;
  }
};

}  // namespace cse

#endif