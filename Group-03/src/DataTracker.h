/**
* @file DataTracker.h
* adds or removes data from a vector and provides statistcal data from it
*
*/

#ifndef DATATRACKER_H
#define DATATRACKER_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cassert>
#include <optional>

namespace cse {

    template <typename T>
    class DataTracker {
    public:

        bool delete_value(const T& value);

        double mean() const;
        double median() const;
        T mode() const;
        double variance() const;
        T min() const;
        T max() const;

        std::optional<T> winner() const;

        // Adds a specified value to the vector
        constexpr void add_value(const T& value) {values.push_back(value);}

        // Returns the total number of elements in the vector
        constexpr size_t total() const {return values.size();}

        // Returns the number of occurrences of a given value
        size_t count(const T& value) const {
            return std::count(values.begin(), values.end(), value);
        }

        // Returns true if the value exists in the dataset
        bool has(const T& value) const {
            return std::find(values.begin(), values.end(), value) != values.end();
        }

    private:
        std::vector<T> values;
    };

} // namespace cse
#endif