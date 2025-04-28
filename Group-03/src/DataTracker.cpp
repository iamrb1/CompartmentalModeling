/** 
* @file DataTracker.cpp
* adds or removes data from a vector and provides statistcal data from it
* 
*/

#include "DataTracker.h"
#include <string>
#include <type_traits>
#include <numeric>
#include <unordered_map>
#include <algorithm>
#include <optional>

namespace cse {

// Removes a value from the vector if found. 
// Returns true if successful. Returns false if the value was not found.
template <typename T>
bool DataTracker<T>::delete_value(const T& value) {

    auto it = std::ranges::find(values, value);
    
    //iterator is not at the end (value is found)
    if (it != values.end()) {
        values.erase(it);
        return true;
    }
    //iterator is at the end so the value was not found
    return false;
}

// Calculates and returns the mean of all of the values in the vector
template <typename T>
double DataTracker<T>::mean() const {
    if (values.empty()) return 0.0;

    //only calculates if the type is numerical
    if constexpr (std::is_arithmetic_v<T>) {//chatgpt
        
        //adds the values and divides by the number of elements in the vector
        return static_cast<double>(std::accumulate(values.begin(), values.end(), static_cast<T>(0))) / values.size(); //chatgpt
    } else {

        //returns zero if non-numeric type
        return 0.0;
    }

}

// Calculates and returns the median of all of the values in the vector
template <typename T>
double DataTracker<T>::median() const {
    if (values.empty()) return 0.0;

    if constexpr (std::is_arithmetic_v<T>) {
        std::vector<T> sorted_values = values;
        std::sort(sorted_values.begin(), sorted_values.end());

        size_t size = sorted_values.size();
        if (size % 2 == 0) {
            return (static_cast<double>(sorted_values[size / 2 - 1]) +
                    static_cast<double>(sorted_values[size / 2])) / 2.0;
        } else {
            return static_cast<double>(sorted_values[size / 2]);
        }
    } else {
        // Return 0.0 for non-numeric types
        return 0.0;
    }
}

    

// Calculates and returns the mode of the values in the vector
template <typename T>
T DataTracker<T>::mode() const {
    if (values.empty()) return T();

    std::unordered_map<T, int> frequency;
    for (const auto& val : values) {
        frequency[val]++;
    }

    return std::max_element(frequency.begin(), frequency.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; })->first; //chatgpt
}

// Calculates and returns the variance
template <typename T>
double DataTracker<T>::variance() const {
    if (values.empty()) return 0.0;

    //only calculates if the type is numeric 
    if constexpr (std::is_arithmetic_v<T>) { //chatgpt
        double mean_value = mean();

        // Use std::accumulate to sum the squared differences from the mean
        double variance_sum = std::accumulate(values.begin(), values.end(), 0.0,

            [mean_value](double acc, const T& val) { //chatgpt
                // For each value, compute (val - mean)^2 and add it to the accumulator
                return acc + ((val - mean_value) * (val - mean_value));
            });

        return variance_sum / values.size();
    } else {
        return 0.0;
    }
}

// Returns the minimum value of the values in the vector
template <typename T>
T DataTracker<T>::min() const {
    if (values.empty()) return T();
    return *std::min_element(values.begin(), values.end());
}

template <typename T>
T DataTracker<T>::max() const {
    if (values.empty()) return T();
    return *std::max_element(values.begin(), values.end());
}



// Determines if there is a winner (if any value surpasses 80% of total)
template <typename T>
std::optional<T> DataTracker<T>::winner() const {
    if (values.empty()) return std::nullopt;

    std::unordered_map<T, int> frequency;
        
    // Calculate the 80% threshold based on the size of the values vector
    size_t threshold = static_cast<size_t>(values.size() * 0.8);

    for (const auto& val : values) {
        
        // If the frequency exceeds the 80% threshold, return this value as the winner
        if (++frequency[val] >= static_cast<int>(threshold)) return val;

    }

    return std::nullopt;
}

template class cse::DataTracker<int>;
template class cse::DataTracker<double>;
template class cse::DataTracker<unsigned int>;
template class cse::DataTracker<std::string>;

} // namespace cse
