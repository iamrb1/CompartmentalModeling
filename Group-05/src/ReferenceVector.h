/**
 * @file ReferenceVector.h
 *
 * @author Calen Green
 *
 * A vector class that keeps pointers to Datum instances under the hood.
 */

#pragma once

#include "Datum.h"
#include <memory>
#include <stdexcept>
#include <vector>

namespace cse {

/**
 * A vector class that keeps pointers to Datum instances under the hood.
 */
class ReferenceVector {
private:
  /// Stores the reference vector.
  std::vector<Datum *> mData;

public:
  class Proxy {
  private:
    /// Stores a pointer to a Datum.
    Datum *mDatum;

  public:
    /**
     * Constructs a Proxy object for a given Datum pointer.
     *
     * @param datum A pointer to the Datum instance.
     */
    explicit Proxy(Datum *datum) : mDatum(datum) {};

    Proxy &operator=(const Datum &value);

    /**
     * Implicitly converts the Proxy object to a Datum reference.
     *
     * @return Datum& A reference to the underlying Datum.
     */
    operator Datum &() { return *mDatum; };

    /**
     * Provides pointer-like access to the underlying Datum instance.
     *
     * @return Datum* A pointer to the Datum instance.
     */
    Datum *operator->() { return mDatum; };
  };

  /**
   * Constructor for a reference vector.
   *
   * @param none
   */
  ReferenceVector() = default;

  /**
   * Pushes an element to the vector.
   *
   * @param datum The reference to a Datum.
   */
  void push_back(Datum &datum) { mData.push_back(&datum); };

  void pop_back();

  /**
   * Returns the size of the reference vector.
   *
   * @return size_t The size of the reference vector.
   */
  size_t size() const { return mData.size(); };

  /**
   * Returns true or false if the reference vector is empty.
   *
   * @return bool The boolean of if the reference vector is empty.
   */
  bool empty() const { return mData.empty(); };

  /**
   * Clears the reference vector completely.
   */
  void clear() { mData.clear(); };

  Proxy operator[](size_t index);

  Datum &at(size_t index);

  Datum &front();

  Datum &back();

  class iterator {
  private:
    std::vector<Datum *>::iterator it_;

  public:
    explicit iterator(std::vector<Datum *>::iterator it) : it_(it) {}

    Datum &operator*() const { return **it_; }

    Datum *operator->() const { return *it_; }

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
   *
   * @return std::vector<Datum*>::iterator An iterator pointing to the first
   * element.
   */
  iterator begin() { return iterator(mData.begin()); }

  /**
   * Returns an iterator to the end of the reference vector.
   *
   * @return std::vector<Datum*>::iterator An iterator pointing past the last
   * element.
   */
  iterator end() { return iterator(mData.end()); }
};

} // namespace cse
