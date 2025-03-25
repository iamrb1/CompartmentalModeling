/* --- BruteForceOptimizer.h --- */

/* ------------------------------------------
Author: Blake Crimmins
Date: 1/31/2025
------------------------------------------ */

#ifndef BRUTEFORCEOPTIMIZER_H
#define BRUTEFORCEOPTIMIZER_H

#include <vector>
#include <utility>
#include <string>
#include <limits>
#include <cassert>

namespace cse {


/**
 * @brief Structure representing an item with a name, weight, and value.
 */
struct Item {
    std::string name;
    double weight;   
    double value;      
};

/**
 * @brief A templated class for exploring possible combinations of items 
 * and identifying an optimal subset based on a scoring (value) function.
 *
 * Each item is defined by its name, weight, and value.
 */
template <typename T>
class BruteForceOptimizer
{
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
    void setOptimizer(bool optimize){
        optimized_ = optimize;
        ///sort greatest to least
        ///std::stable_sort(items_.begin(), items_.end(), [](const Item &a, const Item&b){return a.weight < b.weight;});
        /// Sort makes it slower right now
    }

    /**
     * @brief Stores the list of items in the optimizer.
     *
     * @param newItems The vector of items to manage.
     */
    void SetItems(const std::vector<T>& newItems)
    {
        items_ = newItems;
    }

    /**
     * @brief Sets a capacity/threshold.
     * This represents the maximum allowed weight.
     *
     * @param cap The maximum allowed weight.
     */
    void SetCapacity(double cap)
    {
        capacity_ = cap;
    }

    /**
     * @brief Initiates the brute force/backtracking search to find the best subset.
     *
     * @return A pair containing the best total value and the corresponding subset of items.
     */
    std::pair<double, std::vector<Item>> FindOptimalSolution()
    {
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
    void Search(size_t index, double currentWeight, double currentValue)
    {
        assert(currentWeight >= 0 && currentValue >= 0);
        if (index >= items_.size())
        {
            if (currentWeight <= capacity_ && currentValue > bestScore_)
            {
                bestScore_ = currentValue;
                bestSelection_ = currentSelection_;
            }
            return;
        }

        if (optimized_){
            if (currentWeight + items_[index].weight == capacity_ && currentValue + items_[index].value <= bestScore_){
                Search(index + 1, currentWeight, currentValue);
            }
            else{
                // Exclude the current item.
                Search(index + 1, currentWeight, currentValue);
                // Include the current item if it does not exceed capacity.
                const Item& item = items_[index];
                if (currentWeight + item.weight <= capacity_)
                {
                    currentSelection_.push_back(item);
                    Search(index + 1, currentWeight + item.weight, currentValue + item.value);
                    currentSelection_.pop_back();
                }
            }
        }
        else{
            // Exclude the current item.
            Search(index + 1, currentWeight, currentValue);
            // Include the current item if it does not exceed capacity.
            const Item& item = items_[index];
            if (currentWeight + item.weight <= capacity_)
            {
                currentSelection_.push_back(item);
                Search(index + 1, currentWeight + item.weight, currentValue + item.value);
                currentSelection_.pop_back();
            }
        }
    }

    /*
    recursive?
    if weights under the limit retun -1 over and we return value
    crono high resolution clock above and below function to test time
    vec_set
    rather than continuously creating new vectors
    preallocate all new vectors and keep reusing them
    */

private:
    
    std::vector<T> items_;
    double capacity_;
    double bestScore_;
    std::vector<T> bestSelection_;
    std::vector<T> currentSelection_;
    bool optimized_ = false;
};

} // namespace cse

#endif // BRUTEFORCEOPTIMIZER_H
