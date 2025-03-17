#pragma once
#include <bitset>
#include <concepts>
#include <cstdint>
#include <format>
#include <iostream>
#include <ranges>
#include <vector>

#include "CseAssert.hpp"

namespace cse {

// A std::byte filled with ones
constexpr uint64_t ALL_ONE = 0xFFFFFFFFFFFFFFFF;
// A std::byte filled with zeros
constexpr uint64_t ALL_ZERO = 0;
// One
constexpr uint64_t ONE_ONE = 1;
// Number of bits per element in the underlying vector
constexpr size_t BITS_PER_EL = 64;

template <typename T>
concept BoolGenerator = (std::invocable<T, size_t> &&
                         std::same_as<bool, std::invoke_result_t<T, size_t>>);

// Author(s): Kyle Gunger
// BitVector represents a dynamic array of bits, packed into bytes for space
// efficiency. BitVector supports boolean operations on itself with other
// BitVectors of varying sizes.
class BitVector {
 private:
  // The m_underlying bytes of the vector
  std::vector<uint64_t> m_underlying = {};
  // The number of bits the vector is currently holding
  size_t m_num_bits = 0;
  // The number of set bits in the BitVector
  size_t m_num_set = 0;

  // Keep the bottom "keep" bits of the given byte, discarding the rest and
  // updating the m_num_set
  void truncate_element(size_t byte, uint8_t keep);

  // Manages the case in pattern set where
  // only one byte needs changing
  BitVector& pattern_set_one(size_t start, size_t count, uint64_t pattern);

  // Convert a number of bits
  constexpr size_t bits_to_el_count(size_t bits) {
    return bits ? (1 + (bits - 1) / BITS_PER_EL) : 0;
  }

 public:
  // Author(s): Kyle Gunger
  // BitVector::proxy is a proxy for a single bit in the vector,
  // providing common bitwise operations as well as decomposing into a boolean
  class proxy {
   private:
    // A pointer to the BitVector which created this instance
    BitVector* parent = nullptr;
    // A pointer to the element where the bit is stored
    uint64_t* el = nullptr;
    // The index of the bit within the element ((1 << bit_off) & *el gets the
    // specific bit)
    uint8_t bit_off = 0;

   public:
    // Constructor
    proxy(BitVector* bv, uint64_t* e, uint8_t o)
        : parent(bv), el(e), bit_off(o) {}

    // Set the value of this bit
    proxy& operator=(bool value);
    // Set the value of this bit
    proxy& operator=(const proxy& value);
    // Flip the value of this bit
    proxy& flip();
    // Or this bit with a boolean
    proxy& operator|=(bool value);
    // And this bit with a boolean
    proxy& operator&=(bool value);
    // Xor this bit with a boolean
    proxy& operator^=(bool value);

    // Get the value of this bit
    operator bool() const;

    // Destructor
    ~proxy() = default;
  };

  // Default constructor
  BitVector() = default;

  // Constructor
  BitVector(size_t bits) {
    m_num_bits = bits;
    m_underlying.resize(bits_to_el_count(bits), ALL_ZERO);
  }

  // Bit string constructor
  BitVector(const std::string& bstr, char zero = '0', char one = '1');

  // Generate a bitvector from a function which returns booleans
  // based on the index
  template <typename T>
    requires BoolGenerator<T>
  BitVector(size_t bits, T generator) {
    m_num_bits = bits;
    m_underlying.resize(bits_to_el_count(bits), ALL_ZERO);
    for (size_t i = 0; i < bits; ++i) {
      (*this)[i] = generator(i);
    }
  }

  // Copy constructor
  BitVector(const BitVector& bv) = default;

  // Move operation
  BitVector(BitVector&& bv) = default;

  // Destructor
  ~BitVector() = default;

  // &= operation between two BitVectors
  BitVector& operator&=(const BitVector& rhs);
  // |= operation between two BitVectors
  BitVector& operator|=(const BitVector& rhs);
  // ^= operation between two BitVectors
  BitVector& operator^=(const BitVector& rhs);
  // ~ operation on a BitVector, produces a new BitVector
  BitVector operator~() const;

  // == operator
  bool operator==(const BitVector& rhs) const;

  // != operator
  bool operator!=(const BitVector& rhs) const { return !(*this == rhs); }

  // << operation on a BitVector
  BitVector operator<<(size_t pos) const;
  // <<= operation on a BitVector
  BitVector& operator<<=(size_t pos);
  // >> operation on a BitVector
  BitVector operator>>(size_t pos) const;
  // >>= operation on a BitVector
  BitVector& operator>>=(size_t pos);

  // ostream output on bit vector
  friend std::ostream& operator<<(std::ostream& os, const BitVector& bv);

  // & operation between two BitVectors
  BitVector operator&(const BitVector& rhs) const;
  // | operation between two BitVectors
  BitVector operator|(const BitVector& rhs) const;
  // ^ operation between two BitVectors
  BitVector operator^(const BitVector& rhs) const;

  // Get the bool representation of the bit at idx
  bool operator[](size_t idx) const;
  // Get a reference to the bit at idx
  proxy operator[](size_t idx);

  // Copy assignment
  BitVector& operator=(const BitVector& bv);

  // Set count bits in a repeating pattern starting at the index start
  BitVector& pattern_set(size_t start, size_t count, uint64_t pattern);
  BitVector& pattern_set(uint64_t pattern);

  // Set all bits in the vector to true
  BitVector& set();
  // Set a specific bit in the vector to true
  BitVector& set(size_t pos);
  // Set count bits to true starting at the index start
  BitVector& set(size_t start, size_t count);

  // Set all bits in the vector to false
  BitVector& reset();
  // Set a specific bit in the vector to false
  BitVector& reset(size_t pos);
  // Set count bits to false starting at the index start
  BitVector& reset(size_t start, size_t count);

  // Test a specific bit in the BitVector
  bool test(size_t idx) const;

  // Flip all bits in the BitVector
  BitVector& flip();
  // Flip a specific bit in the BitVector
  BitVector& flip(size_t idx);
  // Flip count bits starting at index start
  BitVector& flip(size_t start, size_t count);

  // Returns true if all the bits in the BitVector are true
  bool all() const { return m_num_set == m_num_bits; }
  // Returns true if any of the bits in the BitVector are true
  bool any() const { return m_num_set > 0; }
  // Returns true if none of the bits in the BitVector are true
  bool none() const { return m_num_set == 0; }
  // Returns the number of set bits in the BitVector
  size_t count() const { return m_num_set; }
  // Returns the number of bits contained by the BitVector
  size_t size() const { return m_num_bits; }

  // Returns the number of bits in this element
  inline size_t BIT_LOOKUP(uint64_t el) const { return std::popcount(el); }

  // Resize the BitVector to the provided size, filling any new bits with the
  // given value
  BitVector& resize(size_t size);
  BitVector& resize(size_t size, uint64_t fill);

  // Append the given bit to the BitVector
  void append(bool value);
  // Append the provided bit vector at the end of this bit vector
  void append(const BitVector& bv);

  // Prepend the given bit to the BitVector
  void prepend(bool value);
  // Prepend the given BitVector to this bit vector
  void prepend(const BitVector& bv);

  // Turn the BitVector into a vector of type bool
  // if num_bits is zero, then it will export the entire BitVector
  std::vector<bool> vectorize(size_t num_bits = 0) {
    cse_assert(num_bits <= m_num_bits);
    if (num_bits == 0) num_bits = m_num_bits;
    std::vector<bool> out;

    for (size_t i = 0; i < num_bits; ++i) {
      out.push_back((*this)[i]);
    }

    return out;
  }

  // Convert the BitVector into a bitset of size T
  template <size_t T>
  std::bitset<T> to_bitset(size_t start_at = 0) {
    cse_assert(T <= m_num_bits);

    std::bitset<T> out;
    // Lowest element to pull bits from (based on start_at)
    size_t start_el = start_at ? bits_to_el_count(start_at) - 1 : 0;
    // First element to pull bits from (based on T)
    size_t el = std::min(bits_to_el_count(T) + start_el, m_underlying.size());
    // How much to shift each element by (based on start_at)
    size_t shift = start_at % BITS_PER_EL;

    // Have to do the first element separately since other
    // elements may need to be interpolated and we don't want
    // to go past the last element in underlying
    out |= m_underlying[el - 1] >> shift;

    // Add interpolated elements in a loop
    for (--el; el > start_at; --el) {
      uint64_t add = m_underlying[el - 1] >> shift;
      add |= m_underlying[el] << (BITS_PER_EL - shift);
      out <<= BITS_PER_EL;
      out |= m_underlying[el - 1];
    }

    // Bits too far left-shifted will be dropped by the
    // bitset so no work to do here.

    return std::move(out);
  }
};

// IMPL GUARD
#ifdef CSE4_IMPL

//
// Reference functions
//

// Convert a reference into a boolean
BitVector::proxy::operator bool() const {
  uint64_t bit = (ONE_ONE << bit_off) & *el;
  return !(bit == ALL_ZERO);
}

// Assign a boolean value to a reference
BitVector::proxy& BitVector::proxy::operator=(bool value) {
  if (*this != value) {
    uint64_t mask = ONE_ONE << bit_off;
    if (value) {
      *el |= mask;
      parent->m_num_set++;
    } else {
      *el ^= mask;
      parent->m_num_set--;
    }
  }

  return *this;
}

// Assign a reference value to a reference
BitVector::proxy& BitVector::proxy::operator=(const BitVector::proxy& value) {
  return this->operator=(value.operator bool());
}

// Flip the bit
BitVector::proxy& BitVector::proxy::flip() {
  (*this) = !(*this);
  return *this;
}

BitVector::proxy& BitVector::proxy::operator&=(bool value) {
  (*this) = (*this) && value;
  return *this;
}

BitVector::proxy& BitVector::proxy::operator|=(bool value) {
  (*this) = (*this) || value;
  return *this;
}

BitVector::proxy& BitVector::proxy::operator^=(bool value) {
  (*this) = (*this) != value;
  return *this;
}

//
// BitVector functions
//

// Bit string constructor
BitVector::BitVector(const std::string& bstr, char zero, char one) {
  m_num_bits = bstr.size();
  m_num_set = 0;
  m_underlying.resize(bits_to_el_count(m_num_bits), ALL_ZERO);

  for (size_t i = 0; i < m_num_bits; ++i) {
    size_t idx = m_num_bits - (i + 1);
    if (bstr[idx] == one) {
      (*this)[i] = true;
    } else {
      cse_assert_eq(
          bstr[idx], zero,
          std::format(
              "Unexpected character in bit string (1 = '{}') (0 = '{}'): '{}'",
              one, zero, bstr[idx]));
    }
  }
}

// Get the index as a reference
BitVector::proxy BitVector::operator[](size_t idx) {
  dbg_assert(idx < m_num_bits,
             std::format("Invalid index into BitVector: idx - {}, max - {}",
                         idx, m_num_bits)
                 .c_str());
  return proxy(this, &m_underlying[idx / BITS_PER_EL],
               (uint8_t)(idx % BITS_PER_EL));
}

// Get the index as a const bool
bool BitVector::operator[](size_t idx) const {
  dbg_assert(m_num_bits > idx,
             std::format("Invalid index into BitVector: idx - {}, max - {}",
                         idx, m_num_bits));
  const uint64_t bit =
      (ONE_ONE << (idx % BITS_PER_EL)) & m_underlying[idx / BITS_PER_EL];
  return bit != ALL_ZERO;
}

// Update a single element with pattern set
BitVector& BitVector::pattern_set_one(size_t start, size_t count,
                                      uint64_t pattern) {
  // Generate mask (will be exactly the bits we will be changing
  uint64_t mask = ALL_ONE;
  if (count % BITS_PER_EL != 0) mask >>= (BITS_PER_EL - (count) % BITS_PER_EL);
  mask <<= (start % BITS_PER_EL);

  // Shift pattern over so first bit in pattern lines up with
  // the "starting" bit in the element
  pattern = (pattern << (start % BITS_PER_EL)) |
            (pattern >> (BITS_PER_EL - (start % BITS_PER_EL)));

  // Get the index of the element we are changing
  size_t idx = start / BITS_PER_EL;

  // Update num_set
  m_num_set -= BIT_LOOKUP(m_underlying[idx] & mask);
  m_num_set += BIT_LOOKUP(pattern & mask);

  // Actually change the element
  m_underlying[idx] = (m_underlying[idx] & ~mask) | (pattern & mask);

  return (*this);
}

// Set bits in a repeating pattern
// start - the index in the BitVector to start filling with the sequence
// count - the number of bits to fill with the sequence
// pattern - the pattern of bits to fill with
BitVector& BitVector::pattern_set(size_t start, size_t count,
                                  uint64_t pattern) {
  if (count == 0) return *this;
  cse_assert(
      (start + count) <= m_num_bits,
      std::format("Invalid range to pattern_set BitVector: start: {}, count: "
                  "{}, number of bits is: {}",
                  start, count, m_num_bits)
          .c_str());

  // Case where we only need to change one byte (helper function)
  if (start % BITS_PER_EL + count <= BITS_PER_EL)
    return pattern_set_one(start, count, pattern);

  // Generate masks for the first element we are changing and the last
  // element we are changing (such that we only change the proper upper bits
  // in the lower element and the proper lower bits in the upper one)
  uint64_t bot_mask = ALL_ONE << (start % BITS_PER_EL);
  uint64_t top_mask = ALL_ONE;
  if ((start + count) % BITS_PER_EL)
    top_mask >>= (BITS_PER_EL - (start + count) % BITS_PER_EL);

  // Pre-compute the total population count (set bits) in the sequence
  // after the change
  size_t ps_total = (size_t)BIT_LOOKUP(pattern) * (count / BITS_PER_EL);
  ps_total += (size_t)BIT_LOOKUP(
      pattern & (ALL_ONE >> (BITS_PER_EL - count % BITS_PER_EL)));
  // Counter for the number of set bits in the old sequence (pre-change)
  size_t ps_before = 0;

  // Bit wrap the pattern due to where the start bit is
  pattern = (pattern << (start % BITS_PER_EL)) |
            (pattern >> (BITS_PER_EL - (start % BITS_PER_EL)));

  // Index of the first element we are changing
  size_t idx = start / BITS_PER_EL;

  // We need to do extra masking for the first element (bot_mask)
  ps_before += BIT_LOOKUP(m_underlying[idx] & bot_mask);
  m_underlying[idx] = (m_underlying[idx] & ~bot_mask) | (pattern & bot_mask);

  // Loop over and set the intermediate elements in the sequence
  // to the pattern (offset before)
  for (++idx; (idx + 1) * BITS_PER_EL < start + count; ++idx) {
    ps_before += BIT_LOOKUP(m_underlying[idx]);
    m_underlying[idx] = pattern;
  }

  // Perform any extra masking for the last element (top_mask)
  ps_before += BIT_LOOKUP(m_underlying[idx] & top_mask);
  m_underlying[idx] = (m_underlying[idx] & ~top_mask) | (pattern & top_mask);

  // Update set bit totals
  m_num_set += ps_total;
  m_num_set -= ps_before;

  return *this;
}

BitVector& BitVector::pattern_set(uint64_t pattern) {
  return this->pattern_set(0, m_num_bits, pattern);
}

// Set all bits in the vector
BitVector& BitVector::set() { return pattern_set(0, m_num_bits, ALL_ONE); }

// Set a bit in the vector
BitVector& BitVector::set(size_t idx) {
  cse_assert(
      idx < m_num_bits,
      std::format("Attempt to set BitVector at index {}, number of bits is {}",
                  idx, m_num_bits)
          .c_str());
  (*this)[idx] = true;
  return *this;
}

// Set bits in the vector within a certain range, starting from
// start and ending at (start + count - 1)
BitVector& BitVector::set(size_t start, size_t count) {
  cse_assert(
      (start + count) <= m_num_bits,
      std::format("Invalid range to set BitVector: start: {}, count: {}, "
                  "number of bits is: {}",
                  start, count, m_num_bits)
          .c_str());
  return pattern_set(start, count, ALL_ONE);
}

// Reset all bits in the vector
BitVector& BitVector::reset() { return pattern_set(0, m_num_bits, ALL_ZERO); }

// Reset a bit in the vector
BitVector& BitVector::reset(size_t idx) {
  cse_assert(idx < m_num_bits,
             std::format(
                 "Attempt to reset BitVector at index {}, number of bits is {}",
                 idx, m_num_bits)
                 .c_str());
  (*this)[idx] = false;
  return *this;
}

// Reset bits in the vector within a certain range, starting from
// start and ending at (start + count - 1)
BitVector& BitVector::reset(size_t start, size_t count) {
  cse_assert(
      (start + count) <= m_num_bits,
      std::format("Invalid range to reset BitVector: start: {}, count: {}, "
                  "number of bits is: {}",
                  start, count, m_num_bits)
          .c_str());
  return pattern_set(start, count, ALL_ZERO);
}

// Test to see if a bit is set
bool BitVector::test(size_t idx) const {
  cse_assert(
      idx < m_num_bits,
      std::format("Attempt to test BitVector at index {}, number of bits is {}",
                  idx, m_num_bits)
          .c_str());
  return (*this)[idx];
}

// Flip all bits in the BitVector
BitVector& BitVector::flip() {
  m_num_set = 0;

  for (size_t i = 0; i < m_underlying.size(); i++) {
    m_underlying[i] = m_underlying[i] ^ ALL_ONE;
    m_num_set += BIT_LOOKUP(m_underlying[i]);
  }
  truncate_element(m_underlying.size() - 1, m_num_bits);

  return *this;
}

// Flip a specific bit in the BitVector
BitVector& BitVector::flip(size_t idx) {
  cse_assert(
      idx < m_num_bits,
      std::format("Attempt to flip BitVector at index {}, number of bits is {}",
                  idx, m_num_bits)
          .c_str());

  (*this)[idx].flip();

  return *this;
}

// Flip all bits in the BitVector
BitVector& BitVector::flip(size_t start, size_t count) {
  if (count == 0) return *this;
  cse_assert((start + count) <= m_num_bits,
             std::format("Invalid range to flip BitVector: start: {}, count: "
                         "{}, number of bits is: {}",
                         start, count, m_num_bits)
                 .c_str());

  // Index of the first element we are changing
  size_t idx = start / BITS_PER_EL;

  // We only need to change one element in this case
  if (start % BITS_PER_EL + count <= BITS_PER_EL) {
    // Constrict the flipped bits to only the ones we want
    uint64_t flipper = ALL_ONE;
    if (count % BITS_PER_EL) flipper >>= (BITS_PER_EL - (count % BITS_PER_EL));

    m_num_set -= BIT_LOOKUP(m_underlying[idx]);
    m_underlying[idx] ^= (flipper << (start % BITS_PER_EL));
    m_num_set += BIT_LOOKUP(m_underlying[idx]);

    return (*this);
  }

  // We might not need to flip all bits in the first element, so
  // generate a "flip mask" from all ones and shifting so we only
  // change the proper bits
  m_num_set -= BIT_LOOKUP(m_underlying[idx]);
  m_underlying[idx] ^= ALL_ONE << (start % BITS_PER_EL);
  m_num_set += BIT_LOOKUP(m_underlying[idx]);

  // Flip all elements in between by XORing with ALL_ONES
  for (++idx; (idx + 1) * BITS_PER_EL < start + count; ++idx) {
    m_num_set -= BIT_LOOKUP(m_underlying[idx]);
    m_underlying[idx] ^= ALL_ONE;
    m_num_set += BIT_LOOKUP(m_underlying[idx]);
  }

  // Similar to the comment for the first element, but for the last
  // element
  m_num_set -= BIT_LOOKUP(m_underlying[idx]);
  m_underlying[idx] ^= ALL_ONE >> (BITS_PER_EL - (start + count) % BITS_PER_EL);
  m_num_set += BIT_LOOKUP(m_underlying[idx]);

  return *this;
}

// Fix the given element by mapping bits to zero after keep bits
// e.g. 0b10101010 with keep 3 -> 0b00000010 (top 5 bits are cut off)
// note: keep is modded, so don't worry about putting in too high a number
void BitVector::truncate_element(size_t byte, uint8_t keep) {
  // No need to cut off if keep is a multiple of 8 (unless we are keeping zero
  // bits)
  if (keep > 0 && keep % BITS_PER_EL == 0) return;
  // Generate cutoff mask
  uint64_t mask = ALL_ONE >> (BITS_PER_EL - (keep % BITS_PER_EL));

  // Get set bit diff as well as apply mask
  m_num_set -= BIT_LOOKUP(m_underlying[byte]);
  m_underlying[byte] &= mask;
  m_num_set += BIT_LOOKUP(m_underlying[byte]);
}

// AND the BitVector with another, setting the m_underlying to
// the product
BitVector& BitVector::operator&=(const BitVector& rhs) {
  size_t i = 0;

  // Main loop doing the ANDing
  for (; i < m_underlying.size() && i < rhs.m_underlying.size(); i++) {
    m_num_set -= BIT_LOOKUP(m_underlying[i]);
    m_underlying[i] &= rhs.m_underlying[i];
    m_num_set += BIT_LOOKUP(m_underlying[i]);
  }

  // Truncate the last element (since the bitvectors might not be the same
  // length)
  if (rhs.m_num_bits < m_num_bits)
    truncate_element(rhs.m_underlying.size() - 1, rhs.m_num_bits);
  else
    truncate_element(i - 1, m_num_bits);

  // If we stopped before the last byte, all the rest should be assumed zero
  for (; i < m_underlying.size(); i++) {
    m_num_set -= BIT_LOOKUP(m_underlying[i]);
    m_underlying[i] = ALL_ZERO;
  }

  return *this;
}

// OR the BitVector with another, setting the m_underlying to
// the product
BitVector& BitVector::operator|=(const BitVector& rhs) {
  size_t i = 0;

  // Main loop doing the or
  for (; i < m_underlying.size() && i < rhs.m_underlying.size(); i++) {
    m_num_set -= BIT_LOOKUP(m_underlying[i]);
    m_underlying[i] |= rhs.m_underlying[i];
    m_num_set += BIT_LOOKUP(m_underlying[i]);
  }

  // Truncate the last element (since the bitvectors might not be the same
  // length)
  if (i == m_underlying.size())
    truncate_element(m_underlying.size() - 1, m_num_bits);

  return *this;
}

// XOR the BitVector with another, setting the m_underlying to
// the product
BitVector& BitVector::operator^=(const BitVector& rhs) {
  size_t i = 0;

  // Main loop doing the xor
  for (; i < m_underlying.size() && i < rhs.m_underlying.size(); i++) {
    m_num_set -= BIT_LOOKUP(m_underlying[i]);
    m_underlying[i] ^= rhs.m_underlying[i];
    m_num_set += BIT_LOOKUP(m_underlying[i]);
  }

  // Truncate the last element (since the bitvectors might not be the same
  // length)
  if (i == m_underlying.size())
    truncate_element(m_underlying.size() - 1, m_num_bits);

  return *this;
}

// Produce a BitVector with all bits flipped
BitVector BitVector::operator~() const {
  BitVector out(m_num_bits);

  for (size_t i = 0; i < m_underlying.size(); i++) {
    out.m_underlying[i] = m_underlying[i] ^ ALL_ONE;
    out.m_num_set += BIT_LOOKUP(out.m_underlying[i]);
  }
  out.truncate_element(out.m_underlying.size() - 1, out.m_num_bits);

  return out;
}

// equals operator
bool BitVector::operator==(const BitVector& rhs) const {
  if (rhs.m_num_bits != m_num_bits || rhs.m_num_set != m_num_set) return false;

  for (size_t i = 0; i < m_underlying.size(); ++i) {
    if (m_underlying[i] != rhs.m_underlying[i]) return false;
  }

  return true;
}

// Copy assign
BitVector& BitVector::operator=(const BitVector& bv) {
  m_underlying = bv.m_underlying;
  m_num_bits = bv.m_num_bits;
  m_num_set = bv.m_num_set;
  return *this;
}

// AND operator with another BitVector
BitVector BitVector::operator&(const BitVector& rhs) const {
  if (m_num_bits < rhs.m_num_bits) {
    BitVector out = rhs;
    out &= *this;
    return out;
  }

  BitVector out = *this;
  out &= rhs;
  return out;
}

// OR operator with another BitVector
BitVector BitVector::operator|(const BitVector& rhs) const {
  if (m_num_bits < rhs.m_num_bits) {
    BitVector out = rhs;
    out |= *this;
    return out;
  }

  BitVector out = *this;
  out |= rhs;
  return out;
}

// XOR operator with another BitVector
BitVector BitVector::operator^(const BitVector& rhs) const {
  if (m_num_bits < rhs.m_num_bits) {
    BitVector out = rhs;
    out ^= *this;
    return out;
  }

  BitVector out = *this;
  out ^= rhs;
  return out;
}

// Left-shift and set operator
BitVector& BitVector::operator<<=(size_t pos) {
  if (pos == 0)
    return *this;
  else if (pos >= m_num_bits) {
    reset();
    return *this;
  }

  m_num_set = 0;
  // The first element that has information we will keep
  size_t skip = pos / BITS_PER_EL;
  // The number of bits to interpolate between elements
  uint8_t slide = pos % BITS_PER_EL;
  // The index of the element we are modifying
  size_t idx = m_underlying.size() - 1;

  // Loop for combining neighboring elements
  for (; skip < idx; --idx) {
    m_underlying[idx] = m_underlying[idx - skip] << slide;
    m_underlying[idx] |=
        m_underlying[idx - (skip + 1)] >> (BITS_PER_EL - slide);
    m_num_set += BIT_LOOKUP(m_underlying[idx]);
  }

  // We slide the last element seperately so as to not access
  // past the beginning
  m_underlying[idx] = m_underlying[0] << slide;
  m_num_set += BIT_LOOKUP(m_underlying[idx]);
  truncate_element(m_underlying.size() - 1, m_num_bits);

  // Clear the rest of the bytes (<< fills with 0s from the right)
  while (idx) {
    --idx;
    m_underlying[idx] = ALL_ZERO;
  }

  return *this;
}

// Right-shift and set operator
BitVector& BitVector::operator>>=(size_t pos) {
  if (pos == 0)
    return *this;
  else if (pos >= m_num_bits) {
    reset();
    return *this;
  }

  m_num_set = 0;
  // The first byte that has information we will keep
  size_t skip = pos / BITS_PER_EL;
  // The number of bits to interpolate between elements
  uint8_t slide = pos % BITS_PER_EL;
  // The index of the element we are modifying
  size_t idx = 0;

  // Loop for combining neighboring elements
  for (; idx + skip < m_underlying.size() - 1; ++idx) {
    m_underlying[idx] = m_underlying[idx + skip] >> slide;
    m_underlying[idx] |= m_underlying[idx + skip + 1] << (BITS_PER_EL - slide);
    m_num_set += BIT_LOOKUP(m_underlying[idx]);
  }

  // We slide the last element seperately so as to not access
  // past the end
  m_underlying[idx] = m_underlying[idx + skip] >> slide;
  m_num_set += BIT_LOOKUP(m_underlying[idx]);

  // Clear the rest of the bytes (>> fills with 0s from the left)
  for (++idx; idx < m_underlying.size(); ++idx) {
    m_underlying[idx] = ALL_ZERO;
  }

  return *this;
}

// Left-shift operator
BitVector BitVector::operator<<(size_t pos) const {
  BitVector out = *this;
  out <<= pos;
  return out;
}

// Right-shift operator
BitVector BitVector::operator>>(size_t pos) const {
  BitVector out = *this;
  out >>= pos;
  return out;
}

// How to format when outputting to a stream
// Current decision: groups of 8 bits space separated,
// and with max groupings of 32 bits per line
std::ostream& operator<<(std::ostream& os, const BitVector& bv) {
  for (size_t i = 0; i < bv.m_num_bits; ++i) {
    os << bv[bv.m_num_bits - 1 - i];
  }

  return os;
}

// Resize this BitVector
BitVector& BitVector::resize(size_t size, uint64_t fill) {
  size_t prev_last = m_num_bits;
  m_num_bits = size;
  m_underlying.resize(bits_to_el_count(size), ALL_ZERO);

  if (size > prev_last && fill) pattern_set(prev_last, size - prev_last, fill);
  if (m_underlying.size() > 0)
    truncate_element(m_underlying.size() - 1, m_num_bits);

  return (*this);
}

BitVector& BitVector::resize(size_t size) { return resize(size, ALL_ZERO); }

// Append a single bit to the bitvector
void BitVector::append(bool value) {
  if (m_num_bits % BITS_PER_EL == 0) m_underlying.push_back(ALL_ZERO);
  m_num_bits++;
  (*this)[m_num_bits - 1] = value;
}

// Append another BitVector to the BitVector
void BitVector::append(const BitVector& bv) {
  if (bv.m_num_bits == 0) return;

  // Pre-calculate shift
  size_t shift = m_num_bits % BITS_PER_EL;
  // Pre-calculated first element to set bits on
  size_t first_el = 0;
  if (m_num_bits > 0) first_el = m_underlying.size() - (shift ? 1 : 0);

  // Update internal values to store more bits
  m_num_bits += bv.m_num_bits;
  m_underlying.resize(bits_to_el_count(m_num_bits), ALL_ZERO);

  // Add to num_set from the other BitVector
  m_num_set += bv.m_num_set;

  // Loop setting elements based on the other BitVector
  // shifting based on the previous number of bits
  size_t i = 0;
  for (; i < bv.m_underlying.size() - 1; i++) {
    m_underlying[first_el + i] |= bv.m_underlying[i] << shift;
    m_underlying[first_el + i + 1] =
        bv.m_underlying[i] >> (BITS_PER_EL - shift);
  }

  // For last element we may not want to access one past
  // first_el + i (depends on whether we are shifting)
  m_underlying[first_el + i] |= bv.m_underlying[i] << shift;
  if (first_el + i + 1 < m_underlying.size())
    m_underlying[first_el + i + 1] =
        bv.m_underlying[i] >> (BITS_PER_EL - shift);
}

// Prepend a single bit to the BitVector
void BitVector::prepend(bool value) {
  if (m_num_bits % BITS_PER_EL == 0) m_underlying.push_back(ALL_ZERO);
  m_num_bits++;
  *this <<= 1;
  (*this)[0] = value;
}

// Prepend another BitVector to the BitVector
void BitVector::prepend(const BitVector& bv) {
  BitVector tmp = std::move(*this);
  *this = bv;
  append(tmp);
}

#endif  // CSE4_IMPL

};  // namespace cse
