/* --- StaticVector.h --- */

/* ------------------------------------------
Author: Alex Mueller 
Date: 2/7/2025
------------------------------------------ */

#ifndef STATICVECTOR_H
#define STATICVECTOR_H

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace cse {
/**
 * @brief A Class designed to create a vector like structure with a static size
 * @tparam T a templated object pointing to the type of objects in the vector
 * @tparam MAX_SIZE a size_t object that is the size of the vector
 */
template <typename T, size_t MAX_SIZE>
class StaticVector {
 private:
  /// @brief Array of size MAX_SIZE
  T data_[MAX_SIZE];
  
  /// @brief Size of the StaticVector
  size_t currentSize_{0};

 public:
  /// @brief Iterator for StaticVector
  using iterator = T*;

  /// @brief Adds param value to next open slot of StaticVector
  /// @param value value to be added to StaticVector
  void push_back(const T& value) {
    if (currentSize_ > MAX_SIZE) {
      throw std::out_of_range("Attempt to exceed static vector capacity.");
    }
    data_[currentSize_++] = value;
  }

  /// @brief Removes ending value of StaticVector
  void pop_back() {
    if (currentSize_ == 0) {
      throw std::out_of_range("Attempt to pop_back from empty vector.");
    }
    --currentSize_;
  }

  /// @brief Clears contents of StaticVector
  void clear() { currentSize_ = 0; }

  /// @brief Gets value from vector at param index
  /// @param index Index to get value from in StaticVector
  T& at(size_t index) {
    if (index >= currentSize_) {
      throw std::out_of_range("Index out of bounds.");
    }
    return data_[index];
  }

  /// @brief Gets const value from vector at param index
  /// @param index Index to get value from in StaticVector
  const T& at(size_t index) const{
    if (index >= currentSize_) {
      throw std::out_of_range("Index out of bounds.");
    }
    return data_[index];
  }

  /// @brief Overload operator of [] and returns value
  /// @param index Index to get value from in StaticVector
  T& operator[](size_t index) { 
    assert(index < currentSize_ && "Trying to index out of range");
    return data_[index]; 
  } 

  /// @brief Const version Overload operator of [] and returns value
  /// @param index Index to get value from in StaticVector
  const T& operator[](size_t index) const { 
    assert(index < currentSize_ && "Trying to index out of range");
    return data_[index];
  }

  /// @brief Returns front value of StaticVector
  T& front() {
    assert(currentSize_ != 0 && "Accessing front from an empty vector.");
    return data_[0];
  }

  /// @brief Returns constant front value of StaticVector
  const T& front() const{
    assert(currentSize_ != 0 && "Accessing front from an empty vector.");
    return data_[0];
  }

  /// @brief Returns back value of StaticVector
  T& back() {
    assert(currentSize_ != 0 && "Accessing back from an empty vector.");
    return data_[currentSize_ - 1];
  }

  /// @brief Returns const back value of StaticVector
  const T& back() const{
    assert(currentSize_ != 0 && "Accessing back from an empty vector.");
    return data_[currentSize_ - 1];
  }

  /// @brief Checks if StaticVector is empty
  bool empty() const { return currentSize_ == 0; }

  /// @brief Returns current size of StaticVector
  size_t size() const { return currentSize_; }

  /// @brief Returns max size of StaticVector
  constexpr size_t max_size() const { return MAX_SIZE; }

  /// @brief Begin iterator for StaticVector
  iterator begin() { return data_; }

  /// @brief End iterator for StaticVector
  iterator end() { return data_+ currentSize_; }
};

/// @brief Creates standard vector from StaticVector
/// @param sv StaticVector to be casted to std vector
template <typename T, size_t MAX_SIZE>
std::vector<T> ToVector(const StaticVector<T, MAX_SIZE>& sv) {
  std::vector<T> result;
  result.reserve(sv.size());
  for (size_t i = 0; i < sv.size(); ++i) {
    result.push_back(sv[i]);
  }
  return result;
}

}  // namespace cse

#endif  // STATICVECTOR_H