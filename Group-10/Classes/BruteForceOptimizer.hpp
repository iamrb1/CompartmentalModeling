/* --- BruteForceOptimizer.hpp --- */

/* ------------------------------------------
Author: Blake Crimmins, Devon FoxElster, Alex Mueller
Last Changed Date: 03/26/2025
------------------------------------------ */

#ifndef BRUTEFORCEOPTIMIZER_H
#define BRUTEFORCEOPTIMIZER_H

#include <algorithm>
#include <cassert>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace cse {

/**
 * @brief Structure representing an item with a name, weight, and value.
 */
struct Item {
  std::string name;
  double weight;
  double value;

  /**
   * @brief == operator for two Items
   * @return indication whether two Items are equal
   */
  friend bool operator==(const Item& a, const Item& b) {
    return (a.name == b.name && a.weight == b.weight && a.value == b.value);
  }
};

/**
 * @brief A templated class for exploring possible combinations of items
 * and identifying an optimal subset based on a scoring (value) function.
 *
 * Each item is defined by its name, weight, and value.
 */
class BruteForceOptimizer {
 public:
  /**
   * @brief Default constructor for BruteForceOptimizer.
   */
  BruteForceOptimizer() = default;

  /**
   * @brief Set whether we want to optimize search.
   *
   * @param optimize Boolean stating whether we want to optimize or not.
   */
  void setOptimizer(bool optimize) {
    optimized_ = optimize;
    /// sort greatest to least
    if (optimized_) {
      std::stable_sort(
          items_.begin(), items_.end(),
          [](const Item& a, const Item& b) { return a.weight < b.weight; });
    }
  }

  /**
   * @brief Stores the list of items in the optimizer.
   *
   * @param newItems The vector of items to manage.
   */
  void SetItems(const std::vector<Item>& newItems) { items_ = newItems; }

  /**
   * @brief Sets a capacity/threshold.
   * This represents the maximum allowed weight.
   *
   * @param cap The maximum allowed weight.
   */
  void SetCapacity(double cap) { capacity_ = cap; }

  /**
   * @brief Initiates the brute force/backtracking search to find the best
   * subset.
   *
   * @return A pair containing the best total value and the corresponding subset
   * of items.
   */
  std::pair<double, std::vector<Item>> FindOptimalSolution() {
    bestScore_ = std::numeric_limits<double>::lowest();
    currentSelection_.clear();
    Search(0, 0.0, 0.0);
    return {bestScore_, bestSelection_};
  }

  /**
   * @brief Recursive helper that decides to include or exclude each item.
   *
   * @param index Index of the current item in the items vector.
   * @param currentWeight The total weight of the selected items so far.
   * @param currentValue The total value of the selected items so far.
   */
  void Search(size_t index, double currentWeight, double currentValue) {
    assert(currentWeight >= 0 && currentValue >= 0);
    // BASE CASE: Index has reached the end of the items_ vector
    if (index >= items_.size()) {
      if (currentWeight <= capacity_ && currentValue > bestScore_) {
        bestScore_ = currentValue;
        bestSelection_ = currentSelection_;
      }
      return;
    }

    if (optimized_) {
      // With items sorted by weight, if this item goes over limit all remaining
      // ones will as well. Update scores if possible, otherwise back up
      if (currentWeight + items_[index].weight > capacity_) {
        if (currentWeight <= capacity_ && currentValue > bestScore_) {
          bestScore_ = currentValue;
          bestSelection_ = currentSelection_;
        }
        return;
      }
      // If adding the current item prevents any further items from being
      // evaluated, evaluate adding it but do not recurse again (as the next
      // item will not fit)
      // Still test without current item
      if (capacity_ - currentWeight - items_[index].weight <=
          items_[index].weight) {
        Search(index + 1, currentWeight, currentValue);

        if (currentValue + items_[index].value > bestScore_) {
          currentSelection_.push_back(items_[index]);
          bestScore_ = currentValue + items_[index].value;
          bestSelection_ = currentSelection_;
        }
        return;
        // Operate as normal
      } else {
        // Exclude the current item.
        Search(index + 1, currentWeight, currentValue);

        // Include the current item if it does not exceed capacity.
        const Item& item = items_[index];

        if (currentWeight + item.weight <= capacity_) {
          currentSelection_.push_back(item);
          Search(index + 1, currentWeight + item.weight,
                 currentValue + item.value);
          currentSelection_.pop_back();
        }
      }

      /*
        Original Brute Force Code
      */
    } else {
      // Exclude the current item.
      Search(index + 1, currentWeight, currentValue);
      // Include the current item if it does not exceed capacity.
      const Item& item = items_[index];
      if (currentWeight + item.weight <= capacity_) {
        currentSelection_.push_back(item);
        Search(index + 1, currentWeight + item.weight,
               currentValue + item.value);
        currentSelection_.pop_back();
      }
    }
  }

 private:
  // Vector containing all Items to optimized
  std::vector<Item> items_;
  // Total capacity to fill from the Items
  double capacity_;
  // Best Score recorded
  double bestScore_;
  // Selection resulting in the best score
  std::vector<Item> bestSelection_;
  // Current selection (used during Search algorithm to record place)
  std::vector<Item> currentSelection_;
  // Boolean indicating whether to optimize the search
  bool optimized_ = false;
};

}  // namespace cse

#endif  // BRUTEFORCEOPTIMIZER_H
