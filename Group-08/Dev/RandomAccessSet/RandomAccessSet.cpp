#include "RandomAccessSet.h"

/**
 * Adds a new unique element to the set.
 * - If the element already exists, it returns false.
 * - Otherwise, it adds the element to both the `elements` vector and `indexMap` for quick lookup.
 */
template <typename T>
bool RandomAccessSet<T>::add(const T& item) {
    if (indexMap.find(item) != indexMap.end()) {
        return false; // Element already exists
    }

    indexMap[item] = elements.size(); // Store index
    elements.push_back(item); // Store element in order
    return true;
}

/**
 * Checks if an element exists in the set.
 * - Uses `indexMap` for O(1) lookup time.
 */
template <typename T>
bool RandomAccessSet<T>::contains(const T& item) const {
    return indexMap.find(item) != indexMap.end();
}

/**
 * Retrieves an element by index.
 * - Throws an error if the index is out of bounds.
 */
template <typename T>
T RandomAccessSet<T>::get(size_t index) const {
    if (index >= elements.size()) {
        throw std::out_of_range("Index out of bounds");
    }
    return elements[index];
}

/**
 * Returns the size of the set.
 */
template <typename T>
size_t RandomAccessSet<T>::size() const {
    return elements.size();
}

/**
 * (Not Finished Yet) Removes an element from the set.
 * - Will require reordering elements in the vector and updating the map.
 */
template <typename T>
bool RandomAccessSet<T>::remove(const T& item) {
    return false; // Placeholder for now
}
