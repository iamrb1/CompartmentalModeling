#include "WeightedSet.hpp"
#include <algorithm>
#include <stdexcept>

void cse::WeightedSet::Add(size_t id, double weight) {
    if (weight < 0) {
        throw std::invalid_argument("Weight must be a positive value");
    }
    ids.push_back(id);
    weights.push_back(weight);
}

void cse::WeightedSet::Remove(size_t id) {
    auto it = std::find(ids.begin(), ids.end(), id);
    if (it == ids.end()) {
        throw std::invalid_argument("ID does not exist");
    }
    size_t i = std::distance(ids.begin(), it);
    ids.erase(ids.begin() + i);
    weights.erase(weights.begin() + i);
}

double cse::WeightedSet::TotalWeight() {
    double total = 0;
    for (double weight : weights) {
        total += weight;
    }
    return total;
}

double cse::WeightedSet::Weight(size_t id) {
    auto it = std::find(ids.begin(), ids.end(), id);
    if (it == ids.end()) {
        throw std::invalid_argument("ID does not exist");
    }
    return weights[std::distance(ids.begin(), it)];
}

size_t cse::WeightedSet::At(double position) {
    if (position < 0 || position > TotalWeight()) {
        throw std::invalid_argument("Invalid position");
    }
    double cumulativeWeight = 0.0;
    for (size_t i = 0; i < ids.size(); i++)
    {
        cumulativeWeight += weights[i];
        if (cumulativeWeight >= position) {
            return ids[i];
        }
    }
}

