/*
 * @file AuditedArray.hpp
 * @author Siddhartha Bhattacharya
 */
#ifndef AUDITEDARRAY_H
#define AUDITEDARRAY_H

#include <algorithm>
#include <iostream>
#include <stdexcept>

#ifdef NDEBUG
  template <typename T, size_t N>
  using AuditedArray = std::array<T,N>;

#else
template <typename T, size_t Size>
class AuditedArray {
 public:
  AuditedArray();
  AuditedArray(const AuditedArray<T, Size>& other);
  ~AuditedArray() = default;
  AuditedArray<T, Size>& operator=(const AuditedArray<T, Size>& other);
  T& operator[](int index);
  const T& operator[](int index) const;
  int size() const;
  void fill(const T& value);
  bool contains(const T& value) const;
  size_t indexOf(const T& value) const;
  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

 private:
  T data[Size];
};

template <typename T, size_t Size>
AuditedArray<T, Size>::AuditedArray() {
  // Default constructor
  std::fill(begin(), end(), T());
}

template <typename T, size_t Size>
AuditedArray<T, Size>::AuditedArray(const AuditedArray<T, Size>& other) {
  // Copy constructor
  if (this->size() != other.size()) {
    throw std::invalid_argument("Array sizes do not match");
  }
  std::copy(other.begin(), other.end(), begin());
}

template <typename T, size_t Size>
AuditedArray<T, Size>& AuditedArray<T, Size>::operator=(
    const AuditedArray<T, Size>& other) {
  // Assignment operator
  if (this == &other) {
    return *this;
  }
  if (this->size() != other.size()) {
    throw std::invalid_argument("Array sizes do not match");
  }
  std::copy(other.begin(), other.end(), begin());
  return *this;
}

template <typename T, size_t Size>
T& AuditedArray<T, Size>::operator[](int index) {
  if (index < 0 || index >= Size) {
    throw std::out_of_range("Index out of range");
  }
  return data[index];
}

template <typename T, size_t Size>
const T& AuditedArray<T, Size>::operator[](int index) const {
  if (index < 0 || index >= Size) {
    throw std::out_of_range("Index out of range");
  }
  return data[index];
}

template <typename T, size_t Size>
int AuditedArray<T, Size>::size() const {
  return Size;
}

template <typename T, size_t Size>
void AuditedArray<T, Size>::fill(const T& value) {
  for (int i = 0; i < Size; i++) {
    data[i] = value;
  }
}

template <typename T, size_t Size>
bool AuditedArray<T, Size>::contains(const T& value) const {
  for (int i = 0; i < Size; i++) {
    if (data[i] == value) {
      return true;
    }
  }
  return false;
}

template <typename T, size_t Size>
size_t AuditedArray<T, Size>::indexOf(const T& value) const {
  for (int i = 0; i < Size; i++) {
    if (data[i] == value) {
      return i;
    }
  }
  return -1;
}

template <typename T, size_t Size>
T* AuditedArray<T, Size>::begin() {
  return &data[0];
}

template <typename T, size_t Size>
T* AuditedArray<T, Size>::end() {
  return &data[Size];
}

template <typename T, size_t Size>
const T* AuditedArray<T, Size>::begin() const {
  return &data[0];
}

template <typename T, size_t Size>
const T* AuditedArray<T, Size>::end() const {
  return &data[Size];
}

#endif // NDEBUG
#endif //AUDITEDARRAY_H