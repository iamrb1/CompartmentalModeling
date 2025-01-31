#include <cse/BitVector.hpp>

namespace cse {

BitVector& BitVector::set(bool value) {
  std::byte replace = std::byte{0};
  
  if (value == true) {
    replace = std::byte{0b11111111};
    num_set = num_bits;
  } else
    num_set = 0;
  
  for (std::byte & b : underlying) {
    b = replace;
  }
  underlying.back() = underlying.back() >> (8 - (num_bits % 8));

  return *this;
}

BitVector& BitVector::set(size_t idx, bool value) {
  if (idx > num_bits)
    return *this;

  std::byte &b = underlying[idx / 8];
  std::byte mask = std::byte{1} << (idx % 8);
  if (value == true && ((b & mask) == std::byte{0})) {
    b |= mask;
    num_set++;
  }
  else if (value == false && ((b & mask) != std::byte{0})) {
    b ^= mask;
    num_set--;
  }

  return *this;
}

BitVector& BitVector::set(size_t start, size_t count, bool value) {
  const std::byte rep_true = std::byte{0b11111111};
  const std::byte rep_false = std::byte{0b00000000};
  // TODO
  return *this;
}

};  // namespace cse
