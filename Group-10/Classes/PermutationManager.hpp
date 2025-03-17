#ifndef PERMUTATION_MANAGER_H
#define PERMUTATION_MANAGER_H

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace cse {

/**
 * @brief PermutationManager class template.
 *
 * This class creates the permutation of items from a container. Stepping will be done through its composite class.
 * The initial combination is available immediately after construction or Reset().
 *
 * Template parameter:
 *   Container - the type of the container holding the items.
 */

template <typename Container>
class PermutationManager {

public:
  // The type of elements stored in the container.
  using ValueType = typename Container::value_type;

  /**
   * @brief constructor for PermutationManager
   * 
   * @param container the container of items
   * @param permutation_size k, or the size of the items chosen at once.
   */
  PermutationManager(const Container & container, size_t permutation_size) : 
  items_(std::begin(container), std::end(container)),
  k_(permutation_size),
  n_(container.size()),
  currentPermutation_() {
    
    // ensure k is less than or equal to n
    if (k_ > n_) {
      throw std::invalid_argument("Combination size cannot be greater than the number of items in the container.");
    }
    
    totalPermutations_ = permutationNumber(n_, k_);

    reset();
  }

  /**
   * @brief resets the permutation generator to the initial state
   */
  void reset() {
    currentIndex_ = 0;
    currentPermutation_.resize(k_);
    indices_.clear();

    // the initial permutation is just the first k items
    for (size_t i = 0; i < k_; ++i) {
      currentPermutation_[i] = items_[i];
      indices_.push_back(i);
    }
  }

  /**
   * @brief generates the next permutation
   * @return false if we've reached the end, true otherwise
   */
  bool next() {
    // end condition
    if (currentIndex_ >= totalPermutations_ - 1)
      return false;

    // either generate the next permutation w/the std algorithm (if n == k) or make a k permutation
    if (n_ == k_) {
      std::next_permutation(indices_.begin(), indices_.end());
    } else {
      nextKPermutation();
    }

    for (size_t i = 0; i < k_; ++i) {
      currentPermutation_[i] = items_[indices_[i]];
    }

    ++currentIndex_;
    return true;
  }

  /**
   * @brief getter for the current permutation
   * @return vector size k of the value type
   */
  std::vector<ValueType> getCurrentPermutation() {
    return currentPermutation_;
  }

  /**
   * @brief calculates the total number of permutatins
   * @param n - the number of items
   * @param k - the number of items we choose
   * @return size_t for the number
   */
  size_t permutationNumber(std::size_t n, std::size_t k) {
    return factorial(n) / factorial(n - k);
  }

private:

  // Container items stored in a vector for random access.
  std::vector<ValueType> items_;

  // Total number of items.
  std::size_t n_;

  // Items chosen
  std::size_t k_;

  // Total Permutations
  std::size_t totalPermutations_;

  // The current permutation
  std::size_t currentIndex_;

  // Indices which indicate which items are in the current permutation
  std::vector<size_t> indices_;

  // The current permutation
  std::vector<ValueType> currentPermutation_;


  /**
   * @brief this function generates the next K permutation 
   * The general idea is that we either just permute the existing indices again, OR that we need a new combination
   * Because in theory we are just stepping through each combination, and grabbing each permutation of such
   * so much of this algorithm will look similar to the one in combo manager
   */
  void nextKPermutation() {
    // check if we can permute the existing set 
    if (std::next_permutation(indices_.begin(), indices_.end()))
      return;

    std::sort(indices_.begin(), indices_.end());

    size_t i = k_ - 1;
    // Find the first index from the right that can be incremented.
    // The condition indices_[i] == i + n_ - k_ indicates that the current index is at its maximum allowable value.
    while (i >= 0 && indices_[i] == i + n_ - k_) {
      --i; // Move to the left if the current index is "maxed out".
    }

    // If no index can be incremented, we've reached the final combination.
    if (i < 0) {
      return;
    }

    // For every index position after i, set it to be one greater than its predecessor.
    for (std::size_t j = i + 1; j < k_; ++j) {
      indices_[j] = indices_[j - 1] + 1;
    }
  }


  /**
   * @brief static helper function that is used to calculate permutation size
   * @param x - the number we are performing the factorial operation on
   * @return x!
   */
  static size_t factorial(size_t x) {
    if (x == 0 || x == 1)
      return 1;
    
    size_t res = 1;
    for (size_t i = 2; i <= x; ++i) {
      res *= i;
    }
    return res;
  }

};


}

#endif