/* --- PermutationManager.hpp --- */

/* ------------------------------------------
Author: Ephraim Bennett
Last Changed Date: 3/22/2025
------------------------------------------ */

#ifndef PERMUTATION_MANAGER_H
#define PERMUTATION_MANAGER_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <functional>

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
  PermutationManager(const Container & container, size_t permutation_size, bool required=false,
    ValueType toRequire = ValueType{}) : 
  items_(std::begin(container), std::end(container)),
  k_(permutation_size),
  n_(container.size()),
  requiredValue_(toRequire),
  isRequired_(required),
  requiredIndex_(0),
  currentPermutation_() {

    if (required) {
      k_ -= 1;
      items_.erase(std::remove(items_.begin(), items_.end(), toRequire));
    }
    
    // ensure k is less than or equal to n
    if (k_ > n_) {
      throw std::invalid_argument("Combination size cannot be greater than the number of items in the container.");
    }
    
    totalPermutations_ = PermutationNumber(n_, k_);

    Reset();
  }

  /**
   * @brief resets the permutation generator to the initial state
   */
  void Reset() {
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
  bool Next() {
    // end condition - only if we aren't repeating elements
    if (!isRepeating_) {
      if (currentIndex_ >= totalPermutations_ - 1)
      return false;
    }
    

    // either generate the next permutation w/the std algorithm (if n == k) or make a k permutation
    if (isRequired_) {
      if (n_ == k_) {
        std::next_permutation(indices_.begin(), indices_.end());
      } else if (requiredIndex_ < k_) { // 
        ++requiredIndex_;
      } else {
        // handle required elements here...
        NextKPermutation_();
        requiredIndex_ = 0;
      }
    } else {
      if (n_ == k_) {
        std::next_permutation(indices_.begin(), indices_.end());
      } else {
        // handle required elements here...
        NextKPermutation_();
        requiredIndex_ = 0;
      }
    }
    

    for (size_t i = 0; i < k_; ++i) {
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
    if (currentIndex_ == 0)
      return false;

    if (isRequired_) {
      // do it w/the standard lib algorithm if n == k
      if (n_ == k_) {
        std::prev_permutation(indices_.begin(), indices_.end());
      } else if (requiredIndex_ > 0) { // 
        --requiredIndex_;
      } else {
        PrevKPermutation_();
        requiredIndex_ = k_;
      }
    } else {
      // do it w/the standard lib algorithm if n == k
      if (n_ == k_) {
        std::prev_permutation(indices_.begin(), indices_.end());
      } else {
        PrevKPermutation_();
      }
    }
    

    for (size_t i = 0; i < k_; ++i) {
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
   * @brief - Setter for the repeating attribute
   * @param repeating - if it's repearting or not
   */
  void SetRepeating(bool repeating) {
    isRepeating_ = repeating;
  }

  /**
   * @brief - Getter for the repeating attribute
   * @return true if repeating, false otherwise
   */
  bool GetRepeating() {
    return isRepeating_;
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
   * 
   */
  void PrevKPermutation_() {
    if (std::prev_permutation(indices_.begin(), indices_.end()))
      return;

    // sort them in ascending, this allows us to find the "largest" index that can be decremented.
    std::sort(indices_.begin(), indices_.end());
    
    // use long long instead of size_t because it could be negative
    long long i = static_cast<long long>(k_) - 1;
    // Find the first index from the right that can be decremented. 2, 3, 4 => 1, 3, 4 => 1, 2, 4 => 1, 2, 3
    // For each index, the minimum allowed value is:
    //   - 0 for the first index, or
    //   - one more than the previous index for subsequent indices.
    while (i >= 0) {
      std::size_t minValue = (i == 0) ? 0 : (indices_[i - 1] + 1);
      if (indices_[i] > minValue) {  // This index can be decremented.
        break;
      }
      --i; // Move left if the current index is at its minimum allowable value.
    }
    // Should not happen if currentIndex_ is > 0, but check nonetheless.
    if (i < 0) {
      return;
    }
    // Decrement the identified index.
    --indices_[i];

    // We want to sort by descending here, becuase we want the largest lexographical combination to be given to 
    // the std::prev_permutation algorithm
    std::sort(indices_.begin(), indices_.end(), std::greater<size_t>());
  }


  /**
   * @brief this function generates the next K permutation 
   * The general idea is that we either just permute the existing indices again, OR that we need a new combination
   * Because in theory we are just stepping through each combination, and grabbing each permutation of such
   * so much of this algorithm will look similar to the one in combo manager
   */
  void NextKPermutation_() {
    // check if we can permute the existing set 
    if (std::next_permutation(indices_.begin(), indices_.end()))
      return;

    std::sort(indices_.begin(), indices_.end());

    // use long long instead of size_t because it could be negative
    long long i = static_cast<long long>(k_) - 1;

    // Find the first index from the right that can be incremented.
    if (isRepeating_) { 
      /**
       * 1, 2, 3, 4
       * 2, 3
       * n - k + (i == 0) = 2
       */
      // This allows the same indices to be included more than once in our combo
      while (i >= 0 && indices_[i] == n_ - 1) {
        --i;
      }

    } else {
      // The condition indices_[i] == i + n_ - k_ indicates that the current index is at its maximum allowable value.
      while (i >= 0 && indices_[i] == n_ - k_ + i) {
        --i; // Move to the left if the current index is "maxed out".
      }
    }
    

    // If no index can be incremented, we've reached the final combination.
    if (i == 0 && indices_[i] == n_ - 1) {
      return;
    }

    // upgrade the valid indice
    ++indices_[i];

    if (isRepeating_) {
      // For every index after i, set t i's index so we get a different combination
      for (size_t j = i + 1; j < k_; ++j) {
        indices_[j] = indices_[i];
      }
    } else {
      // For every index position after i, set it to be one greater than its predecessor.
      for (std::size_t j = i + 1; j < k_; ++j) {
        indices_[j] = indices_[j - 1] + 1;
      }
    }

    //std::cout << "Updated the indice: " << i << std::endl;
    //std::cout << "New indices: ";
    for (auto x : indices_) {
      //std::cout << x << " ";
    }
    //std::cout << std::endl; 
  }


  /**
   * @brief static helper function that is used to calculate permutation size
   * @param x - the number we are performing the factorial operation on
   * @return x!
   */
  static size_t Factorial_(size_t x) {
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