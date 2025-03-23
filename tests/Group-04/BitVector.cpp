#include <string>

#include "catch.hpp"

#define TEST_CSE_ASSERT
#include "BitVector.hpp"
#include "CseAssert.hpp"

TEST_CASE("Test basic BitVector constructors", "[bitvector]") {
  // Basic constructors with number of bits
  cse::BitVector bv1;
  cse::BitVector bv2(8);
  cse::BitVector bv3(13);

  // Constructor with bit string
  std::string bstr = "100110101110001111011";
  cse::BitVector bv4(bstr);
  cse::BitVector bv5("abbaababaaabbbaaaabaa", 'b', 'a');

  CHECK(bv4.size() == bstr.size());
  CHECK(bv4 == bv5);

  cse::BitVector bv0(0);
  REQUIRE_ASSERT(bv0[0]);
  REQUIRE_ASSERT(bv2[8]);
  REQUIRE_ASSERT(bv2[10]);
  REQUIRE_ASSERT(bv3[13]);
  REQUIRE_ASSERT(bv3[17]);
  REQUIRE_ASSERT(bv5[31]);
}

TEST_CASE("Test setting bits", "[bitvector]") {
  // Single byte case
  cse::BitVector bv1(8);

  for (size_t i = 0; i < bv1.size(); i++) {
    bv1[i] = true;
    CHECK(bv1[i] == true);
    CHECK(bv1.count() == i + 1);
  }

  // Set all function (single byte)
  cse::BitVector bv2(8), bc2("11111111");
  bv2.set();

  CHECK(bv2.count() == bv2.size());
  CHECK(bv2 == bc2);

  // Single byte case (set function)
  cse::BitVector bv3(8);

  for (size_t i = 0; i < bv3.size(); i++) {
    bv3.set(i);
    CHECK(bv3.test(i) == true);
    CHECK(bv3.count() == i + 1);
  }

  // Single byte case (ranged set function)
  cse::BitVector bv4(8), bc4("00111000");
  bv4.set(3, 3);
  CHECK(bv4.count() == bc4.count());
  CHECK(bv4 == bc4);

  // Set all function (multi-byte case)
  cse::BitVector bv5(119);
  bv5.set();

  CHECK(bv5.count() == bv5.size());
  CHECK(bv5.count() == 119);

  // Multi element case (set function)
  cse::BitVector bv6(123);

  for (size_t i = 0; i < bv6.size(); i++) {
    bv6.set(i);
    CHECK(bv6.test(i));
    CHECK(bv6.count() == i + 1);
  }

  // Multi element case (ranged set function)
  cse::BitVector bv7(89), bc7(89);
  bv7.set(60, 19);
  for (size_t i = 0; i < 19; i++) {
    bc7.set(60 + i);
  }
  CHECK(bv7.count() == bc7.count());
  CHECK(bv7 == bc7);

  // Asserts for setting bits out of range
  REQUIRE_ASSERT(bv1.set(8));
  REQUIRE_ASSERT(bv1.set(0, 9));
  REQUIRE_ASSERT(bv1.set(4, 100));
}

TEST_CASE("Test pattern setting bits", "[bitvector]") {
  // Pattern set whole byte
  cse::BitVector bv1(8), bc1("10010110");
  bv1.pattern_set(0, 8, 0b10010110);
  CHECK(bv1.count() == 4);
  CHECK(bv1 == bc1);

  // Pattern set part of byte
  cse::BitVector bv2(8), bc2("00000110");
  bv2.pattern_set(0, 3, 0b01010110);

  CHECK(bv2.count() == bc2.count());
  CHECK(bv2 == bc2);

  // Pattern set part of byte (offset)
  cse::BitVector bv3(8), bc3("01101000");
  bv3.pattern_set(3, 4, 0b11111101);

  CHECK(bv3.count() == bc3.count());
  CHECK(bv3 == bc3);

  // Pattern set multiple elements
  cse::BitVector bv4(80);
  bv4.pattern_set((uint64_t)0xFFFFFF);
  CHECK(bv4.count() == 40);
  CHECK(bv4[64]);
  CHECK(!bv4[63]);

  // Pattern set multiple elements (offset)
  cse::BitVector bv5(80);
  bv5.pattern_set(6, 74, (uint64_t)0xFFFFFF);
  CHECK(bv5.count() == 34);
  CHECK(bv5[70]);
  CHECK(!bv5[69]);

  // Asserts for setting bits out of range
  REQUIRE_ASSERT(bv1.pattern_set(0, 9, 0b11111111));
  REQUIRE_ASSERT(bv1.pattern_set(3, 7, 0b11111111));
}

TEST_CASE("Test resetting bits", "[bitvector]") {
  // Single byte case
  cse::BitVector bv1(8);
  bv1[0] = 1;
  bv1[1] = 1;
  bv1[4] = 1;
  bv1[7] = 1;

  CHECK(bv1.count() == 4);

  bv1.reset();

  cse::BitVector bv2(8);

  CHECK(bv1.count() == 0);
  CHECK(bv1 == bv2);
  CHECK(!bv1.test(0));
  CHECK(!bv1.test(1));
  CHECK(!bv1.test(4));
  CHECK(!bv1.test(7));

  REQUIRE_ASSERT(bv1.test(8));
  REQUIRE_ASSERT(bv1.test(100));
}

template <typename F, typename G, typename H>
void LAMBDA_TEST(F&& gen, G&& gen_chk, H&& op) {
  // Test with one byte
  cse::BitVector a(8), b(8), chk(8);
  for (size_t i = 0; i < (1 << 8); i++) {
    for (size_t j = 0; j < (1 << 8); j++) {
      gen(a, i);
      gen(b, j);

      gen_chk(a, b, chk);

      op(a, b);

      CHECK(a == chk);
      CHECK(a.count() == chk.count());
    }
  }

  // Test with multiple elements (near boundary)
  a.resize(128);
  b.resize(128);
  chk.resize(128);
  for (size_t i = 0; i < (1 << 7); i++) {
    for (size_t j = 0; j < (1 << 7); j++) {
      gen(a, i);
      gen(b, j);

      a <<= 59;
      b <<= 59;

      gen_chk(a, b, chk);

      op(a, b);

      CHECK(a == chk);
      CHECK(a.count() == chk.count());
    }
  }
}

TEST_CASE("Test AND operation on BitVectors", "[bitvector]") {
  auto gen = [](cse::BitVector& bv, size_t i) {
    for (size_t j = 0; j < bv.size(); j++) {
      if (1 & (i >> j)) bv[j] = true;
    }
  };

  auto gen_chk = [](const cse::BitVector& a, const cse::BitVector& b,
                    cse::BitVector& chk) {
    for (size_t i = 0; i < chk.size(); i++) {
      if (a.size() <= i || b.size() <= i) break;
      chk[i] = a[i] && b[i];
    }
  };

  auto op = [](cse::BitVector& bv1, cse::BitVector& bv2) { bv1 &= bv2; };

  LAMBDA_TEST(gen, gen_chk, op);
}

TEST_CASE("Test OR operation on BitVectors", "[bitvector]") {
  auto gen = [](cse::BitVector& bv, size_t i) {
    for (size_t j = 0; j < bv.size(); j++) {
      if (1 & (i >> j)) bv[j] = true;
    }
  };

  auto gen_chk = [](const cse::BitVector& a, const cse::BitVector& b,
                    cse::BitVector& chk) {
    for (size_t i = 0; i < chk.size(); i++) {
      if (a.size() < i)
        chk[i] = b[i];
      else if (b.size() < i)
        chk[i] = a[i];
      else
        chk[i] = a[i] || b[i];
    }
  };

  auto op = [](cse::BitVector& bv1, cse::BitVector& bv2) { bv1 |= bv2; };

  LAMBDA_TEST(gen, gen_chk, op);
}

TEST_CASE("Test bitwise XOR on BitVectors", "[bitvector]") {
  auto gen = [](cse::BitVector& bv, size_t i) {
    for (size_t j = 0; j < bv.size(); j++) {
      if (1 & (i >> j)) bv[j] = true;
    }
  };

  auto gen_chk = [](const cse::BitVector& a, const cse::BitVector& b,
                    cse::BitVector& chk) {
    for (size_t i = 0; i < chk.size(); i++) {
      if (a.size() < i)
        chk[i] = b[i];
      else if (b.size() < i)
        chk[i] = a[i];
      else
        chk[i] = a[i] != b[i];
    }
  };

  auto op = [](cse::BitVector& bv1, cse::BitVector& bv2) { bv1 ^= bv2; };

  LAMBDA_TEST(gen, gen_chk, op);
}

TEST_CASE("Test bitwise shifting", "[bitvector]") {
  // One byte
  cse::BitVector bv1(8), bc1("10101000");
  bv1.pattern_set(0, 8, 0b01010101);
  bv1 <<= 3;
  CHECK(bv1 == bc1);

  // Multi-byte
  cse::BitVector bv2(14), bc2("10101010000000");
  bv2.pattern_set(0, 14, 0b01010101);
  bv2 <<= 7;
  CHECK(bv2 == bc2);
}

TEST_CASE("Test bitwise NOT", "[bitvector]") {
  // One Byte
  cse::BitVector bv1(8), bc1("01010001");
  bv1.pattern_set(0, 8, 0b10101110);
  CHECK(bc1 == ~bv1);
  bv1.flip();
  CHECK(bc1 == bv1);

  cse::BitVector bv2(21), bc2(21);
  bv2.pattern_set(0, 21, 0b01101011);
  bc2 = bv2;

  bv2.flip(6, 13);
  for (size_t i = 6; i < 19; i++) {
    bc2.flip(i);
  }

  CHECK(bv2 == bc2);
}

TEST_CASE("Test basic resize operation", "[bitvector]") {
  // Basic constructors with number of bits
  cse::BitVector bv1(8), bc1("0011111111");
  bv1.set();
  CHECK(bv1.size() == 8);

  bv1.resize(10);
  CHECK(bv1.size() == 10);
  CHECK(bv1 == bc1);
}

TEST_CASE("Test basic append and prepend operations", "[bitvector]") {
  cse::BitVector bv1(7), bc1("10000000");
  bv1.append(true);
  CHECK(bv1 == bc1);

  cse::BitVector bv2(7), bc2("00000001");
  bv2.prepend(true);
  CHECK(bv2 == bc2);

  cse::BitVector bv3(16), bc3("101010101010101010");
  bv3.pattern_set(0, 16, 0b0101010101010101);
  bv3.append(true);
  bv3.prepend(false);
  CHECK(bv3[17]);
  CHECK(bv3 == bc3);
}

TEST_CASE("Test advanced append and prepend operations", "[bitvector]") {
  // Append two bitvectors
  cse::BitVector bv1(4), ba1("1010"), bc1("10100000");
  bv1.append(ba1);
  CHECK(bv1 == bc1);

  // Append to an empty bitvector
  cse::BitVector bv2(0), ba2("1010"), bc2("1010");
  bv2.append(ba2);
  CHECK(bv2 == bc2);

  // Append an empty bitvector
  cse::BitVector bv3("0101"), ba3(0), bc3("0101");
  bv3.append(ba3);
  CHECK(bv3 == bc3);

  // Appending when creating new elements
  cse::BitVector bv4(40), ba4(40);
  bv4.pattern_set(0xFAFAFAFAFA);
  ba4.pattern_set(0xB5B5B5B5B5);
  bv4.append(ba4);
  CHECK(bv4.count() == 30 + 25);
  for (size_t i = 0; i < 5; i++) {
    size_t idx = i * 8;
    CHECK(!bv4[idx]);
    CHECK(!bv4[idx + 2]);
  }
  for (size_t i = 0; i < 5; i++) {
    size_t idx = (i + 5) * 8;
    CHECK(bv4[idx]);
    CHECK(bv4[idx + 2]);
  }

  // Check prepend (should just call append under the hood)
  cse::BitVector bv5(4), ba5("1010"), bc5("00001010");
  bv5.prepend(ba5);
  CHECK(bv5 == bc5);
}

TEST_CASE("Test class template features", "[bitvector]") {
  // Template constructor
  cse::BitVector bv1(34, [](size_t i) -> bool { return i % 3 == 0; });
  for (size_t i = 0; i < bv1.size(); ++i) {
    CHECK(bv1[i] == (i % 3 == 0));
  }

  // Template set
  bv1.generator_set([](size_t i) { return i % 2 == 0; });
  for (size_t i = 0; i < bv1.size(); ++i) {
    CHECK(bv1[i] == (i % 2 == 0));
  }

  // Assign from number
  bv1 = (uint64_t)5;
  CHECK(bv1.size() == 64);
  cse::BitVector bc1(64);
  bc1.pattern_set(0, 5, 5);
  CHECK(bv1 == bc1);

  // Or, and, and xor
  bv1 |= 3;
  bc1.pattern_set(0, 5, 7);
  CHECK(bv1 == bc1);

  bv1 &= 1;
  bc1.pattern_set(0, 5, 1);
  CHECK(bv1 == bc1);

  bv1 ^= 7;
  bc1.pattern_set(0, 5, 6);
  CHECK(bv1 == bc1);

  // Test template assign with other integral types
  bv1 = (uint16_t)5;
  CHECK(bv1.size() == 16);

  // Test exporting to a bitset
  std::bitset<8> bs1 = bv1.to_bitset<8>();
  for (size_t i = 0; i < bs1.size(); i++) {
    CHECK(bs1[i] == bv1[i]);
  }

  // Test exporting to a vector of bools
  std::vector<bool> vb1 = bv1.vectorize();
  for (size_t i = 0; i < vb1.size(); i++) {
    CHECK(vb1[i] == bv1[i]);
  }

  // Test creating bitvectors from bitsets and vectors of boolean
  cse::BitVector bv2, bv3;
  bv2 = bs1;
  bv3 = vb1;
  CHECK(bv2.size() == bs1.size());
  CHECK(bv3.size() == vb1.size());
  for (size_t i = 0; i < bv2.size(); i++) {
    CHECK(bv2[i] == bs1[i]);
  }
  for (size_t i = 0; i < bv3.size(); i++) {
    CHECK(bv3[i] == vb1[i]);
  }

  bv1.resize(34);
  bv1.pattern_set(0xB2BDBDFAFA);

  // Test integral vectorize
  std::vector<uint16_t> shorts = bv1.vectorize<uint16_t>();
  CHECK(shorts.size() == 3);
  CHECK(shorts[0] == 0xFAFA);
  CHECK(shorts[1] == 0xBDBD);
  CHECK(shorts[2] == 0x0002);
}
