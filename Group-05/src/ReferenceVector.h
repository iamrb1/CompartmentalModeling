/**
 * @file ReferenceVector.h
 *
 * @author Calen Green and Max Krawec
 *
 * A vector class that keeps pointers to a type of objects,
 * allowing for entries into the vector to be treated like references.
 */

#pragma once

#include "Datum.h"

#include <cassert>
#include <memory>
#include <stdexcept>
#include <vector>

namespace cse {

template<typename template_type>
class ReferenceVector {
 private:
  /// Stores pointers to a type of objects.
  std::vector<template_type*> references_;

 public:
  /**
   * Default constructor for ReferenceVector.
   */
  ReferenceVector() = default;

  /**
   * Pushes an object reference into the vector.
   * @param value The reference to an object.
   */
  void PushBack(template_type &value) { references_.push_back(&value); }

  /**
   * Removes the last element in the vector.
   */
  void PopBack() {
    assert(!references_.empty());
    references_.pop_back();
  };

  /**
   * Returns the number of elements in the vector.
   * @return size_t The size of the vector.
   */
  [[nodiscard]] size_t Size() const { return references_.size(); }

  /**
   * Checks if the vector is empty.
   * @return bool True if empty, otherwise false.
   */
  [[nodiscard]] bool Empty() const { return references_.empty(); }

  /**
   * Clears all elements from the vector.
   */
  void Clear() { references_.clear(); }

  /**
 * Returns a reference to the first object in the vector.
 * @return template_type& Reference to the first template_type.
 */
  template_type &Front() {
    assert(!references_.empty());
    return *references_.front();
  };

  /**
   * Returns a reference to the last template_type in the vector.
   * @return template_type& Reference to the last template_type.
   */
  template_type &Back() {
    assert(!references_.empty());
    return *references_.back();
  };

  /**
   * Erases the element at the index.
   * @param index The index of the desired element.
   */
  void Erase(size_t index) {
    assert(index < references_.size());
    references_.erase(references_.begin() + index);
  }

  /**
   * Erases multiple elements at the indices.
   * @param first The first index to remove.
   * @param last The first index you don't want removed.
   */
  void Erase(size_t first, size_t last) {
    assert(first < references_.size());
    assert(last <= references_.size());
    assert(first < last); // TODO - Not sure if they can be equal?
    assert(first != last);
    references_.erase(references_.begin() + first, references_.begin() + last);
  }

  /**
   * Inserts a value at the desired index.
   * @param index The desired index.
   * @param value The value to add.
   */
  void Insert(size_t index, template_type &value) {
    assert(index < references_.size());
    references_.insert(references_.begin() + index, &value);
  }

  /**
  * Accesses an object reference at the given index.
  * @param index The position of the element.
  * @return template_type& Reference to the object at the index.
  */
  template_type &operator[](size_t index) {
    assert(index < references_.size());
    return *references_[index];
  };

  /**
   * Returns a reference to the object at the given index.
   * @param index The index of the desired element.
   * @return template_type& Reference to the object.
   */
  template_type &At(size_t index) {
    assert(index < references_.size());
    return *references_[index];
  };

  /**
   * Sets the reference at an index.
   * @param index The index to update.
   * @param value The value to update with.
   */
  void SetReference(size_t index, template_type &value) {
    assert(index < references_.size());
    references_[index] = &value;
  }

  class iterator {
   private:
    typename std::vector<template_type *>::iterator it_;

   public:
    explicit iterator(typename std::vector<template_type *>::iterator it) : it_(it) {}

    template_type &operator*() const { return **it_; }

    template_type *operator->() const { return *it_; }

    iterator &operator++() {
      ++it_;
      return *this;
    }

    iterator operator++(int) {
      iterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const iterator &other) const { return it_ == other.it_; }
    bool operator!=(const iterator &other) const { return it_ != other.it_; }
  };

  /**
   * Returns an iterator to the beginning of the reference vector.
   * @return std::vector<template_type*>::iterator An iterator pointing to the first
   * element.
   */
  iterator begin() { return iterator(references_.begin()); }

  /**
   * Returns an iterator to the end of the reference vector.
   * @return std::vector<template_type*>::iterator An iterator pointing past the last
   * element.
   */
  iterator end() { return iterator(references_.end()); }
};

} // namespace cse
