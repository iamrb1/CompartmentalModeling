#pragma once
#include <cstdint>
#include <iostream>
#include <vector>

namespace cse {

// Author(s): Kyle Gunger
// BitVector represents a dynamic array of bits, packed into bytes for space
// efficiency. BitVector supports boolean operations on itself with other
// BitVectors of varying sizes.
class BitVector {
 private:
  // The underlying bytes of the vector
  std::vector<std::byte> underlying = {};
  // The number of bits the vector is currently holding
  size_t num_bits = 0;
  // The number of set bits in the BitVector
  size_t num_set = 0;

  // Lookup table for how many bits are 'on' in a byte
  const int8_t BYTE_SET_LOOKUP[256] = {
      0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,  // 0b00001111
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,  // 0b00011111
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,  // have
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  // 0b00111111
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,  // to
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  // put
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  // these stupid comments
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,  // 0b01111111
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,  // here
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  // so
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  // that
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,  // clang-format
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  // won't
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,  // destroy
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,  // my nice rectangle.
      4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,  // 0b11111111
  };

  // Keep the bottom "keep" bits of the given byte, discarding the rest and
  // updating the num_set
  void fixup_byte(size_t byte, uint8_t keep);

 public:
  // Author(s): Kyle Gunger
  // BitVector::reference is a reference to a single bit in the vector,
  // providing common bitwise operations as well as decomposing into a boolean
  class reference {
   private:
    // A pointer to the BitVector which created this instance
    BitVector* parent = nullptr;
    // A pointer to the byte where the bit is stored
    std::byte* byte = nullptr;
    // The index of the bit within the byte ((1 << bit_off) & *byte gets the
    // specific bit)
    uint8_t bit_off = 0;

   public:
    // Constructor
    reference(BitVector* bv, std::byte* b, uint8_t o)
        : parent(bv), byte(b), bit_off(o) {}

    // Set the value of this bit
    reference& operator=(bool value);
    // Set the value of this bit
    reference& operator=(const reference& value);
    // Flip the value of this bit
    reference& flip();

    // Get the value of this bit
    operator bool() const;
    // Get the negated value of this bit
    bool operator~() const;

    // Destructor
    ~reference() = default;
  };

  // Constructor
  BitVector(size_t bits) {
    num_bits = bits;
    underlying.resize(bits / 8 + (bits % 8 == 0 ? 0 : 1), std::byte{0});
  }

  // Copy constructor
  BitVector(const BitVector& bv) {
    underlying = bv.underlying;
    num_bits = bv.num_bits;
    num_set = bv.num_set;
  }

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
  reference operator[](size_t idx);

  // Copy assignment
  BitVector& operator=(const BitVector& bv);

  // Set all bits in the vector to true
  BitVector& set(bool value = true);
  // Set a specific bit in the vector to the given value (defaults true)
  BitVector& set(size_t pos, bool value = true);
  // Set count bits to a specific value starting at the index start
  BitVector& set(size_t start, size_t count, bool value = true);

  // Returns true if all the bits in the BitVector are true
  bool all() const { return num_set == num_bits; }
  // Returns true if any of the bits in the BitVector are true
  bool any() const { return num_set > 0; }
  // Returns true if none of the bits in the BitVector are true
  bool none() const { return num_set == 0; }
  // Returns the number of set bits in the BitVector
  size_t count() const { return num_set; }
  // Returns the number of bits contained by the BitVector
  size_t size() const { return num_bits; }
};

};  // namespace cse
