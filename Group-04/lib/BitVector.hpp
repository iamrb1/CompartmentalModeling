#pragma once
#include <cstdint>
#include <format>
#include <iostream>
#include <vector>
#include <ranges>

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
    // A pointer to the byte where the bit is stored
    uint64_t* el = nullptr;
    // The index of the bit within the byte ((1 << bit_off) & *byte gets the
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
  inline int8_t BIT_LOOKUP(uint64_t el) const {
    return std::popcount(el);
  }

  // Resize the BitVector to the provided size, filling any new bits with the
  // given value
  void resize(size_t size, bool fill = false);
  void resize(size_t size, uint64_t fill);

  // Append the given bit to the BitVector
  void append(bool value);
  // Append the provided bit vector at the end of this bit vector
  void append(const BitVector& bv);
  // Prepend the given bit to the BitVector
  void prepend(bool value);
  // Prepend the given BitVector to this bit vector
  void prepend(const BitVector& bv);
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
BitVector::proxy& BitVector::proxy::operator=(
    const BitVector::proxy& value) {
  return this->operator=(value.operator bool());
}

// Flip the bit
BitVector::proxy& BitVector::proxy::flip() {
  (*this) = !(*this);
  return *this;
}

BitVector::proxy& BitVector::proxy::operator&= (bool value) {
  (*this) = (*this) && value;
  return *this;
}

BitVector::proxy& BitVector::proxy::operator|= (bool value) {
  (*this) = (*this) || value;
  return *this;
}

BitVector::proxy& BitVector::proxy::operator^= (bool value) {
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
    } else if (bstr[idx] != zero) {
      throw std::invalid_argument(std::format(
          "Unexpected character in bit string (1 = '{}') (0 = '{}'): '{}'", one,
          zero, bstr[idx]));
    }
  }
}

// Get the index as a reference
BitVector::proxy BitVector::operator[](size_t idx) {
#ifndef NDEBUG
  dbg_assert(idx < m_num_bits, std::format("Invalid index into BitVector: idx - {}, max - {}", idx, m_num_bits).c_str());
#endif
  return proxy(this, &m_underlying[idx / BITS_PER_EL],
                   (uint8_t)(idx % BITS_PER_EL));
}

// Get the index as a const bool
bool BitVector::operator[](size_t idx) const {
#ifndef NDEBUG
  if (m_num_bits <= idx)
    throw std::out_of_range(std::format(
        "Invalid index into BitVector: idx - {}, max - {}", idx, m_num_bits));
#endif
  const uint64_t bit =
      (ONE_ONE << (idx % BITS_PER_EL)) & m_underlying[idx / BITS_PER_EL];
  return bit != ALL_ZERO;
}

// Update a single byte with pattern set
BitVector& BitVector::pattern_set_one(size_t start, size_t count,
                                      uint64_t pattern) {
  uint64_t mask = ALL_ONE;
  if (count % BITS_PER_EL != 0) mask >>= (BITS_PER_EL - (count) % BITS_PER_EL);
  mask <<= (start % BITS_PER_EL);

  pattern = (pattern << (start % BITS_PER_EL)) |
            (pattern >> (BITS_PER_EL - (start % BITS_PER_EL)));

  size_t idx = start / BITS_PER_EL;

  m_num_set -= BIT_LOOKUP(m_underlying[idx] & mask);
  m_num_set += BIT_LOOKUP(pattern & mask);

  m_underlying[idx] = (m_underlying[idx] & ~mask) | (pattern & mask);

  return (*this);
}

// Set bits in a repeating pattern
// start - the index in the BitVector to start filling with the sequence
// count - the number of bits to fill with the sequence
// pattern - the pattern of bits to fill with
BitVector& BitVector::pattern_set(size_t start, size_t count,
                                  uint64_t pattern) {
  if (count == 0)
    return *this;
  cse_assert((start + count) <= m_num_bits,
      std::format("Invalid range to pattern_set BitVector: start: {}, count: "
                    "{}, number of bits is: {}",
                    start, count, m_num_bits).c_str());

  // Case where we only need to change one byte
  if (start % BITS_PER_EL + count <= BITS_PER_EL)
    return pattern_set_one(start, count, pattern);

  // Masks for end bytes in the seq
  uint64_t bot_mask = ALL_ONE << (start % BITS_PER_EL);
  uint64_t top_mask = ALL_ONE;
  if ((start + count) % BITS_PER_EL)
    top_mask >>= (BITS_PER_EL - (start + count) % BITS_PER_EL);

  // Total bits set in the new sequence
  size_t ps_total = (size_t)BIT_LOOKUP(pattern) * (count / BITS_PER_EL);
  ps_total += (size_t)BIT_LOOKUP(
      pattern & (ALL_ONE >> (BITS_PER_EL - count % BITS_PER_EL)));
  // Counter for the number of bytes in the old sequence
  size_t ps_before = 0;

  // Offset the pattern due to where the start bit is
  pattern = (pattern << (start % BITS_PER_EL)) |
            (pattern >> (BITS_PER_EL - (start % BITS_PER_EL)));

  // Index of the byte we are changing
  size_t idx = start / BITS_PER_EL;

  // First byte
  ps_before += BIT_LOOKUP(m_underlying[idx] & bot_mask);
  m_underlying[idx] = (m_underlying[idx] & ~bot_mask) | (pattern & bot_mask);

  // Loop set
  for (++idx; (idx + 1) * BITS_PER_EL < start + count; ++idx) {
    ps_before += BIT_LOOKUP(m_underlying[idx]);
    m_underlying[idx] = pattern;
  }

  // Last byte
  ps_before += BIT_LOOKUP(m_underlying[idx] & top_mask);
  m_underlying[idx] = (m_underlying[idx] & ~top_mask) | (pattern & top_mask);

  m_num_set += ps_total;
  m_num_set -= ps_before;

  return *this;
}

// Set all bits in the vector
BitVector& BitVector::set() { return pattern_set(0, m_num_bits, ALL_ONE); }

// Set a bit in the vector
BitVector& BitVector::set(size_t idx) {
  cse_assert(idx < m_num_bits,
    std::format(
        "Attempt to set BitVector at index {}, number of bits is {}", idx,
        m_num_bits).c_str());
  (*this)[idx] = true;
  return *this;
}

// Set bits in the vector within a certain range, starting from
// start and ending at (start + count - 1)
BitVector& BitVector::set(size_t start, size_t count) {
  cse_assert((start + count) <= m_num_bits,
        std::format("Invalid range to set BitVector: start: {}, count: {}, "
                    "number of bits is: {}",
                    start, count, m_num_bits).c_str());
  return pattern_set(start, count, ALL_ONE);
}

// Reset all bits in the vector
BitVector& BitVector::reset() { return pattern_set(0, m_num_bits, ALL_ZERO); }

// Reset a bit in the vector
BitVector& BitVector::reset(size_t idx) {
  cse_assert(idx < m_num_bits,
    std::format(
        "Attempt to reset BitVector at index {}, number of bits is {}", idx,
        m_num_bits).c_str());
  (*this)[idx] = false;
  return *this;
}

// Reset bits in the vector within a certain range, starting from
// start and ending at (start + count - 1)
BitVector& BitVector::reset(size_t start, size_t count) {
  cse_assert((start + count) <= m_num_bits,
        std::format("Invalid range to reset BitVector: start: {}, count: {}, "
                    "number of bits is: {}",
                    start, count, m_num_bits).c_str());
  return pattern_set(start, count, ALL_ZERO);
}

// Test to see if a bit is set
bool BitVector::test(size_t idx) const {
  cse_assert(idx < m_num_bits,
    std::format(
        "Attempt to test BitVector at index {}, number of bits is {}", idx,
        m_num_bits).c_str());
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
  cse_assert(idx < m_num_bits,
    std::format(
        "Attempt to flip BitVector at index {}, number of bits is {}", idx,
        m_num_bits).c_str());

  (*this)[idx].flip();

  return *this;
}

// Flip all bits in the BitVector
BitVector& BitVector::flip(size_t start, size_t count) {
  if (count == 0)
    return *this;
  cse_assert((start + count) <= m_num_bits,
        std::format("Invalid range to flip BitVector: start: {}, count: "
                    "{}, number of bits is: {}",
                    start, count, m_num_bits).c_str());

  uint64_t flipper = ALL_ONE;
  // Index of the byte we are changing
  size_t idx = start / BITS_PER_EL;

  // We only need to change one byte in this case
  if (start % BITS_PER_EL + count <= BITS_PER_EL) {
    // Constrict the flipped bits to only the ones we want
    if (count % BITS_PER_EL) flipper >>= (BITS_PER_EL - (count % BITS_PER_EL));

    m_num_set -= BIT_LOOKUP(m_underlying[idx]);
    m_underlying[idx] ^= (flipper << (start % BITS_PER_EL));
    m_num_set += BIT_LOOKUP(m_underlying[idx]);

    return (*this);
  }

  // First byte
  m_num_set -= BIT_LOOKUP(m_underlying[idx]);
  m_underlying[idx] ^= flipper << (start % BITS_PER_EL);
  m_num_set += BIT_LOOKUP(m_underlying[idx]);

  // Loop flip
  for (++idx; (idx + 1) * BITS_PER_EL < start + count; ++idx) {
    m_num_set -= BIT_LOOKUP(m_underlying[idx]);
    m_underlying[idx] ^= flipper;
    m_num_set += BIT_LOOKUP(m_underlying[idx]);
  }

  // Last byte
  m_num_set -= BIT_LOOKUP(m_underlying[idx]);
  if ((start + count) % BITS_PER_EL)
    flipper >>= (BITS_PER_EL - (start + count) % BITS_PER_EL);
  m_underlying[idx] ^= flipper;
  m_num_set += BIT_LOOKUP(m_underlying[idx]);

  return *this;
}

// Fix the given byte by mapping bits to zero after keep bits
// e.g. 0b10101010 with keep 3 -> 0b00000010 (top 5 bits are cut off)
// note: keep is modded, so don't worry about putting in too high a number
void BitVector::truncate_element(size_t byte, uint8_t keep) {
  // No need to cut off if keep is a multiple of 8
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

  // Main loop doing the anding
  for (; i < m_underlying.size() && i < rhs.m_underlying.size(); i++) {
    m_num_set -= BIT_LOOKUP(m_underlying[i]);
    m_underlying[i] &= rhs.m_underlying[i];
    m_num_set += BIT_LOOKUP(m_underlying[i]);
  }

  // Fixup the last byte
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

  // Fixup last byte just in case extra bits from rhs
  // were put at end of byte
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

  // Fixup last byte just in case extra bits from rhs
  // were put at end of byte
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
  // The first byte that has information we will keep
  size_t skip = pos / BITS_PER_EL;
  // The number of bits to interpolate between shift and shift+1 bytes
  uint8_t slide = pos % BITS_PER_EL;
  // The index of the byte we are modifying
  size_t idx = m_underlying.size() - 1;

  // Loop for combining neighboring bytes
  for (; skip < idx; --idx) {
    m_underlying[idx] = m_underlying[idx - skip] << slide;
    m_underlying[idx] |=
        m_underlying[idx - (skip + 1)] >> (BITS_PER_EL - slide);
    m_num_set += BIT_LOOKUP(m_underlying[idx]);
  }

  // Last byte
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
  // The number of bits to interpolate between shift and shift+1 bytes
  uint8_t slide = pos % BITS_PER_EL;
  // The index of the byte we are modifying
  size_t idx = 0;

  // Combine neighbor bytes
  for (; idx + skip < m_underlying.size() - 1; ++idx) {
    m_underlying[idx] = m_underlying[idx + skip] >> slide;
    m_underlying[idx] |= m_underlying[idx + skip + 1] << (BITS_PER_EL - slide);
    m_num_set += BIT_LOOKUP(m_underlying[idx]);
  }

  // Last byte
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
  int byte = 0;
  for (auto b : std::ranges::reverse_view(bv.m_underlying)) {
    os << std::format("{:0>64b}\n", b);
    byte++;
  }

  return os;
}

// Resize this BitVector
void BitVector::resize(size_t size, uint64_t fill) {
  size_t bytes = (size / BITS_PER_EL);
  if (size % BITS_PER_EL) bytes++;

  if (size > m_num_bits) {
    if (m_num_bits % BITS_PER_EL) {
      // Add bits from the pattern to the end of the last byte
      int8_t diff = BIT_LOOKUP(m_underlying.back());
      m_underlying.back() |= fill << (m_num_bits % BITS_PER_EL);
      diff -= BIT_LOOKUP(m_underlying.back());

      // Update m_num_set
      m_num_set += diff;
      fill = (fill >> (BITS_PER_EL - (m_num_bits % BITS_PER_EL))) |
             (fill << (m_num_bits % BITS_PER_EL));
    }

    // Add to num set based on the number of bytes we will be appending on to
    // m_underlying
    m_num_set += BIT_LOOKUP(fill) * (bytes - m_underlying.size());
  }

  m_underlying.resize(bytes, fill);
  m_num_bits = size;
  if(m_underlying.size() > 0)
    truncate_element(m_underlying.size() - 1, m_num_bits);
}

// Resize this bitvector, optionally filling with bits
void BitVector::resize(size_t size, bool fill) {
  uint64_t b = fill ? ALL_ONE : ALL_ZERO;
  resize(size, b);
}

// Append a bit to this BitVector
void BitVector::append(bool value) {
  if (m_num_bits % BITS_PER_EL == 0) m_underlying.push_back(ALL_ZERO);
  m_num_bits++;
  (*this)[m_num_bits - 1] = value;
}

// Append a BitVector to this BitVector
void BitVector::append(const BitVector& bv) {
  if (bv.m_num_bits == 0) return;

  // Pack bits into the last byte if there is room
  uint8_t over = m_num_bits % BITS_PER_EL;
  if (over > 0) m_underlying.back() |= bv.m_underlying[0] << over;

  // Loop to push new bytes on to the back
  for (size_t i = 0; i < bv.m_underlying.size() - 1; ++i) {
    uint64_t add = bv.m_underlying[i];
    // Merge with next byte if offset
    if (over > 0) {
      add = add >> (BITS_PER_EL - over);
      add |= bv.m_underlying[i + 1] << over;
    }
    m_underlying.push_back(add);
  }

  // The number of bits we had in our upper byte to begin with
  uint8_t open_slots = (BITS_PER_EL - (m_num_bits % BITS_PER_EL)) % BITS_PER_EL;
  // The number of bits taken up in the upper byte of bv
  uint8_t req_slots = BITS_PER_EL - (bv.m_num_bits % BITS_PER_EL);
  // Add the remaining bits if required
  if (req_slots < open_slots) {
    uint64_t add = bv.m_underlying.back() >> (BITS_PER_EL - over);
    m_underlying.push_back(add);
  }

  m_num_bits += bv.m_num_bits;
  m_num_set += bv.m_num_set;
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
  BitVector tmp = *this;
  *this = bv;
  append(tmp);
}

#endif  // CSE4_IMPL

};  // namespace cse
