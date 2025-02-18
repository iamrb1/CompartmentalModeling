#include "DataTracker.h"
#include <algorithm>
#include <numeric>
#include <cassert>


namespace cse {
void DataTracker::add_value(double value) {
    size_t old_size = values.size();
    values.push_back(value);

    assert(values.size() >= old_size && "Size did not increase");

}

void DataTracker::delete_value(double value) {
    size_t old_size = values.size();
    auto it = std::find(values.begin(), values.end(), value);
    if (it != values.end()) {
        values.erase(it);
    }
    assert(values.size() <= old_size && "Size did not decrease");
}

double DataTracker::mean() const {
    if (values.empty()) return 0.0;
    double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();

    assert(mean >= min() && mean <= max() && "Mean should be within the range.");

    return mean;
}

double DataTracker::median() const {
    if (values.empty()) return 0.0;
    std::vector<double> sorted_values = values;
    std::sort(sorted_values.begin(), sorted_values.end());
    size_t size = sorted_values.size();
    if (size % 2 == 0) {
        return (sorted_values[size / 2 - 1] + sorted_values[size / 2]) / 2.0;
    } else {
        return sorted_values[size / 2];
    }
}

double DataTracker::min() const {
    if (values.empty()) return 0.0;
    
    double min = *std::min_element(values.begin(), values.end());

    assert(std::all_of(values.begin(), values.end(), [&](double v) { return v >= min; }) && 
           "Min value is not actually the minimum.");

    return min;
}

double DataTracker::max() const {
    if (values.empty()) return 0.0;

    double max = *std::max_element(values.begin(), values.end());

    assert(std::all_of(values.begin(), values.end(), [&](double v) { return v <= max; }) && 
           "Max value is not actually the maximum.");

    return max;
}

size_t DataTracker::total() const {
    size_t size = values.size();
    assert(size >= 0 && "Total count cannot be negative.");
    return size;
}
}