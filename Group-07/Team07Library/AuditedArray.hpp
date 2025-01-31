/*
* @file AuditedArray.hpp
 * @author Siddhartha Bhattacharya
 */
#ifndef AUDITEDARRAY_H
#define AUDITEDARRAY_H

#include <iostream>
#include <stdexcept>
#include <algorithm>

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
  // TODO: Implement this function
}

template <typename T, size_t Size>
AuditedArray<T, Size>::AuditedArray(const AuditedArray<T, Size>& other) {
  // TODO: Implement this function
}

template <typename T, size_t Size>
AuditedArray<T, Size>& AuditedArray<T, Size>::operator=(const AuditedArray<T, Size>& other) {
  // TODO: Implement this function
}

template <typename T, size_t Size>
T& AuditedArray<T, Size>::operator[](int index) {
  // TODO: Implement this function
}

template <typename T, size_t Size>
const T& AuditedArray<T, Size>::operator[](int index) const {
  // TODO: Implement this function
}


template <typename T, size_t Size>
int AuditedArray<T, Size>::size() const {
  return Size;
}

template <typename T, size_t Size>
void AuditedArray<T, Size>::fill(const T& value) {
  // TODO: Implement this function
}

template <typename T, size_t Size>
bool AuditedArray<T, Size>::contains(const T& value) const {
  // TODO: Implement this function
}

template <typename T, size_t Size>
size_t AuditedArray<T, Size>::indexOf(const T& value) const {
  // TODO: Implement this function
}

template <typename T, size_t Size>
T* AuditedArray<T, Size>::begin() {
  // TODO: Implement this function
}

template <typename T, size_t Size>
T* AuditedArray<T, Size>::end() {
  // TODO: Implement this function
}

template <typename T, size_t Size>
const T* AuditedArray<T, Size>::begin() const {
  // TODO: Implement this function
}

template <typename T, size_t Size>
const T* AuditedArray<T, Size>::end() const {
  // TODO: Implement this function
}

#endif // AUDITEDARRAY_H
