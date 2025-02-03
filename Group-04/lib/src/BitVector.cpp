#include <cse/BitVector.hpp>
#include <cstdint>
#include <format>
#include <iostream>

namespace cse {

//
// Reference functions
//

// Convert a reference into a boolean
BitVector::reference::operator bool() const {
  std::byte b = (std::byte{1} << bit_off) & *byte;
  if (b == std::byte{0}) return false;
  return true;
}

// Get the inverse of this reference's boolean value
bool BitVector::reference::operator~() const {
  return !(this->operator bool());
}

// Assign a boolean value to a reference
BitVector::reference& BitVector::reference::operator=(bool value) {
  if (*this != value) {
    std::byte mask = std::byte{1} << bit_off;
    if (value) {
      *byte |= mask;
      parent->num_set++;
    } else {
      *byte ^= mask;
      parent->num_set--;
    }
  }

  return *this;
}

// Assign a reference value to a reference
BitVector::reference& BitVector::reference::operator=(
    const BitVector::reference& value) {
  return this->operator=(value.operator bool());
}

// Flip the bit
BitVector::reference& BitVector::reference::flip() {
  (*this) = !(*this);
  return *this;
}

//
// BitVector functions
//

// Get the index as a reference
BitVector::reference BitVector::operator[](size_t idx) {
  return reference(this, &underlying[idx / 8], (uint8_t)(idx % 8));
}

// Get the index as a const bool
bool BitVector::operator[](size_t idx) const {
  const std::byte b = (std::byte{1} << (idx % 8)) & underlying[idx / 8];
  return b != std::byte{0};
}

// Set all bits in the vector
BitVector& BitVector::set(bool value) {
  std::byte replace = std::byte{0};

  if (value == true) {
    replace = std::byte{0b11111111};
    num_set = num_bits;
  } else
    num_set = 0;

  for (std::byte& b : underlying) {
    b = replace;
  }
  fixup_byte(underlying.size() - 1, num_bits);

  return *this;
}

// Set a bit in the vector
BitVector& BitVector::set(size_t idx, bool value) {
  if (idx < num_bits) (*this)[idx] = value;
  return *this;
}

// Set bits in the vector within a certain range, starting from
// start and ending at (start + count - 1)
BitVector& BitVector::set(size_t start, size_t count, bool value) {
  // const std::byte rep_true = std::byte{0b11111111};
  // const std::byte rep_false = std::byte{0b00000000};
  // TODO better impl, using byte setting instead of bit setting
  for (; start < count && start < num_bits; start++) {
    (*this)[start] = value;
  }

  return *this;
}

// Fix the given byte by mapping bits to zero after keep bits
// e.g. 0b10101010 with keep 3 -> 0b00000010 (top 5 bits are cut off)
// note: keep is modded, so don't worry about putting in too high a number
void BitVector::fixup_byte(size_t byte, uint8_t keep) {
  // No need to cut off if keep is a multiple of 8
  if (keep > 0 && keep % 8 == 0) return;
  // Generate cutoff mask
  std::byte mask = std::byte{0b11111111} >> (8 - (keep % 8));

  // Get set bit diff as well as apply mask
  int8_t diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[byte])];
  underlying[byte] &= mask;
  diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[byte])] - diff;

  // Update num_set
  if (diff < 0)
    num_set -= -diff;
  else
    num_set += diff;
}

// AND the BitVector with another, setting the underlying to
// the product
BitVector& BitVector::operator&=(const BitVector& rhs) {
  size_t i = 0;

  // Main loop doing the anding
  for (; i < underlying.size() && i < rhs.underlying.size(); i++) {
    int8_t diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])];
    underlying[i] &= rhs.underlying[i];
    diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])] - diff;
    if (diff < 0)
      num_set -= -diff;
    else
      num_set += diff;
  }

  // Fixup the last byte
  if (rhs.num_bits < num_bits)
    fixup_byte(rhs.underlying.size() - 1, rhs.num_bits);
  else
    fixup_byte(i - 1, num_bits);

  // If we stopped before the last byte, all the rest should be assumed zero
  for (; i < underlying.size(); i++) {
    uint8_t diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])];
    underlying[i] = std::byte{0};
    num_bits -= diff;
  }

  return *this;
}

// OR the BitVector with another, setting the underlying to
// the product
BitVector& BitVector::operator|=(const BitVector& rhs) {
  size_t i = 0;

  // Main loop doing the or
  for (; i < underlying.size() - 1 && i < rhs.underlying.size() - 1; i++) {
    int8_t diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])];
    underlying[i] |= rhs.underlying[i];
    diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])] - diff;
    // diff can never be less than zero, since we don't lose bits when we or
    num_set += diff;
  }

  // Or the last byte with a mask
  if (i < underlying.size()) {
    int8_t diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])];
    uint8_t mask_num = (8 - std::min(num_bits, rhs.num_bits) % 8);
    std::byte or_mask = mask_num != 8 ? std::byte{0b11111111} >> mask_num
                                      : std::byte{0b11111111};
    underlying[i] |= (rhs.underlying[i] & or_mask);
    diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])] - diff;
    // diff can never be less than zero, since we don't lose bits when we or
    num_set += diff;
  }

  return *this;
}

// XOR the BitVector with another, setting the underlying to
// the product
BitVector& BitVector::operator^=(const BitVector& rhs) {
  size_t i = 0;

  // Main loop doing the xor
  for (; i < underlying.size() - 1 && i < rhs.underlying.size() - 1; i++) {
    int8_t diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])];
    underlying[i] ^= rhs.underlying[i];
    diff -= BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])];
    if (diff < 0)
      num_set -= -diff;
    else
      num_set += diff;
  }

  // Xor the last byte with a mask
  if (i < underlying.size()) {
    int8_t diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])];
    uint8_t mask_num = (8 - std::min(num_bits, rhs.num_bits) % 8);
    std::byte xor_mask = mask_num != 8 ? std::byte{0b11111111} >> mask_num
                                      : std::byte{0b11111111};
    underlying[i] ^= (rhs.underlying[i] & xor_mask);
    diff = BYTE_SET_LOOKUP[std::to_integer<uint8_t>(underlying[i])] - diff;
    // diff can never be less than zero, since we don't lose bits when we or
    if (diff < 0)
      num_set -= -diff;
    else
      num_set += diff;
  }

  return *this;
}

// Produce a BitVector with all bits flipped
BitVector BitVector::operator~() const {
  BitVector out(num_bits);

  for (size_t i = 0; i < underlying.size(); i++) {
    out.underlying[i] = underlying[i] ^ std::byte{0b11111111};
    out.num_set += BYTE_SET_LOOKUP[std::to_integer<uint8_t>(out.underlying[i])];
  }
  out.fixup_byte(out.underlying.size() - 1, out.num_bits);

  return out;
}

BitVector& BitVector::operator=(const BitVector& bv) {
  underlying = bv.underlying;
  num_bits = bv.num_bits;
  num_set = bv.num_set;
  return *this;
}

BitVector BitVector::operator&(const BitVector& rhs) const {
  if (num_bits < rhs.num_bits) {
    BitVector out = rhs;
    out &= *this;
    return out;
  }

  BitVector out = *this;
  out &= rhs;
  return out;
}

BitVector BitVector::operator|(const BitVector& rhs) const {
  if (num_bits < rhs.num_bits) {
    BitVector out = rhs;
    out |= *this;
    return out;
  }

  BitVector out = *this;
  out |= rhs;
  return out;
}

BitVector BitVector::operator^(const BitVector& rhs) const {
  if (num_bits < rhs.num_bits) {
    BitVector out = rhs;
    out ^= *this;
    return out;
  }

  BitVector out = *this;
  out ^= rhs;
  return out;
}

// How to format when outputting to a stream
// Current decision: groups of 8 bits space separated,
// and with max groupings of 32 bits per line
std::ostream& operator<<(std::ostream& os, const BitVector& bv) {
  int byte = 0;
  for (auto b = bv.underlying.rbegin(); b != bv.underlying.rend(); ++b) {
    if (byte % 4 == 3)
      os << std::format("{:0>8b}\n", std::to_integer<uint8_t>(*b));
    else
      os << std::format("{:0>8b} ", std::to_integer<uint8_t>(*b));
    byte++;
  }

  return os;
}

};  // namespace cse
