/**
 * @file ReferenceVector.h
 *
 * @author Calen Green
 *
 * A vector class that keeps pointers to Datum instances under the hood.
 */

#pragma once

#include <vector>
#include <stdexcept>
#include "Datum.h"

namespace cse {

class ReferenceVector {
 private:
  /// Stores pointers to Datum instances.
  std::vector<Datum*> mData;

 public:
  /**
   * Default constructor for ReferenceVector.
   */
  ReferenceVector() = default;

  /**
   * Pushes a Datum reference into the vector.
   *
   * @param datum The reference to a Datum.
   */
  void push_back(Datum& datum) { mData.push_back(&datum); }

  /**
   * Removes the last element in the vector.
   */
  void pop_back();

  /**
   * Returns the number of elements in the vector.
   *
   * @return size_t The size of the vector.
   */
  size_t size() const { return mData.size(); }

  /**
   * Checks if the vector is empty.
   *
   * @return bool True if empty, otherwise false.
   */
  bool empty() const { return mData.empty(); }

  /**
   * Clears all elements from the vector.
   */
  void clear() { mData.clear(); }

  /**
   * Accesses a Datum reference at the given index.
   *
   * @param index The position of the element.
   * @return Datum& Reference to the Datum at the index.
   */
  Datum& operator[](size_t index);

  /**
   * Returns a reference to the Datum at the given index.
   *
   * @param index The index of the desired element.
   * @return Datum& Reference to the Datum.
   */
  Datum& at(size_t index);

  /**
   * Returns a reference to the first Datum in the vector.
   *
   * @return Datum& Reference to the first Datum.
   */
  Datum& front();

  /**
   * Returns a reference to the last Datum in the vector.
   *
   * @return Datum& Reference to the last Datum.
   */
  Datum& back();

  /**
   * Returns an iterator to the beginning of the vector.
   */
  std::vector<Datum*>::iterator begin() { return mData.begin(); }

  /**
   * Returns an iterator to the end of the vector.
   */
  std::vector<Datum*>::iterator end() { return mData.end(); }
};

}  // namespace cse 
