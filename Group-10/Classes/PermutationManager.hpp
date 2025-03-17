#ifndef PERMUTATION_MANAGER_H
#define PERMUTATION_MANAGER_H

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
  n_(container.size()) {
    
    // ensure k is less than or equal to n
    if (k_ > n_) {
      throw std::invalid_argument("Combination size cannot be greater than the number of items in the container.");
    }
    
    totalCombinations_ = permutationNumber(n_, k_);
  }

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

  // Total Combinationsw
  std::size_t totalCombinations_;


  /**
   * @brief static helper function that is used to calculate permutation size
   * @param x - the number we are performing the factorial operation on
   * @return x!
   */
  static size_t factorial(size_t x) {
    if (x == 0 || x == 1)
      return 1;
    
    size_t res = 1;
    for (size_t i = 2; i <= num; ++i) {
      res *= i;
    }
    return res;
  }

};


}

#endif