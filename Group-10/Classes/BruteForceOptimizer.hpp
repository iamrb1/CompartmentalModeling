/* --- BruteForceOptimizer.hpp --- */

/* ------------------------------------------
Author: Blake Crimmins, Devon FoxElster, Alex Mueller
Last Changed Date: 03/26/2025
------------------------------------------ */

#ifndef BRUTEFORCEOPTIMIZER_H
#define BRUTEFORCEOPTIMIZER_H

#include <algorithm>
#include <cassert>
#include <iostream>
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

  friend std::ostream& operator<<(std::ostream& os, const Item& item){
    os << "Item: " << item.name << ", Weight: " << item.weight
              << ", Value: " << item.value << std::endl;
    return os;
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
  void SetOptimizer(bool optimize) {
    optimizeEnabled_ = optimize;
    /// Sort greatest value first, with tiebreaker going to heavier items
    if (optimizeEnabled_) {
      std::stable_sort(items_.begin(), items_.end(),
                       [](const Item& a, const Item& b) {
                         return (a.value == b.value) ? a.weight > b.weight
                                                     : a.value > b.value;
                       });
    }
    double totalValue = 0.0;
    for (auto iter = items_.rbegin(); iter != items_.rend(); ++iter) {
      totalValue += (*iter).value;
      scoreTracker_.push_back(totalValue);
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
    ExploreCombinations(0, 0.0, 0.0);
    return {bestScore_, bestSelection_};
  }

  /**
   * @brief Recursive helper that decides to include or exclude each item.
   *
   * @param index Index of the current item in the items vector.
   * @param currentWeight The total weight of the selected items so far.
   * @param currentValue The total value of the selected items so far.
   */
  void ExploreCombinations(size_t index, double currentWeight,
                           double currentValue) {
    assert(currentWeight >= 0 && currentValue >= 0);

    auto updateScore = [this](double currentValue) {
      bestScore_ = currentValue;
      bestSelection_ = currentSelection_;
    };

    auto scoreCheck = [this, currentWeight, currentValue,
                       updateScore]() mutable {
      if (currentWeight <= capacity_ && currentValue > bestScore_) {
        updateScore(currentValue);
      }
    };

    if (index >= items_.size()) {
      scoreCheck();
      return;
    }

    if (optimizeEnabled_) {
      double remainingValue = scoreTracker_.at(items_.size() - 1 - index);
      if (currentValue + remainingValue < bestScore_) {
        return;
      }
    }
    AverageCaseCombinations_(index, currentWeight, currentValue);
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
  bool optimizeEnabled_ = false;
  // Vector containing the total value possible at any point in items_
  // (Inverse: First element in scoreTracker corresponds to last in items_)
  std::vector<double> scoreTracker_;

  /**
   * @brief Helper Function for ExploreCombinations
   */
  void AverageCaseCombinations_(std::size_t index, double currentWeight,
                                double currentValue) {
    // Exclude the current item.
    ExploreCombinations(index + 1, currentWeight, currentValue);
    // Include the current item if it does not exceed capacity.
    const Item& item = items_[index];
    if (currentWeight + item.weight <= capacity_) {
      currentSelection_.push_back(item);
      ExploreCombinations(index + 1, currentWeight + item.weight,
                          currentValue + item.value);
      currentSelection_.pop_back();
    }
  }
};

}  // namespace cse

#endif  // BRUTEFORCEOPTIMIZER_H
