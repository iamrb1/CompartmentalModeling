#include "catch.hpp"

#define TEST_CSE_ASSERT
#include "CseAssert.hpp"

#define REQUIRE_ASSERT(...)                                                    \
  REQUIRE_THROWS_AS(__VA_ARGS__, cse::_assert_internal::AssertTestException)

#define REQUIRE_NOASSERT(...) REQUIRE_NOTHROW(__VA_ARGS__)

#include "BitVector.hpp"
#include <string>

#include "BitVector.hpp"

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
  
  // Pattern set multiple elements (offset)
  
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


template<typename F, typename G, typename H>
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
  auto gen = [](cse::BitVector& bv, size_t i){
    for(size_t j = 0; j < bv.size(); j++) {
      if (1 & (i >> j))
        bv[j] = true;
    }
  };

  auto gen_chk = [](const cse::BitVector& a, const cse::BitVector& b, cse::BitVector& chk){
    for(size_t i = 0; i < chk.size(); i++) {
      if(a.size() <= i || b.size() <= i)
        break;
      chk[i] = a[i] && b[i];
    }
  };
  
  auto op = [](cse::BitVector& bv1, cse::BitVector& bv2){
    bv1 &= bv2;
  };

  LAMBDA_TEST(gen, gen_chk, op);
}

TEST_CASE("Test OR operation on BitVectors", "[bitvector]") {
  auto gen = [](cse::BitVector& bv, size_t i){
    for(size_t j = 0; j < bv.size(); j++) {
      if (1 & (i >> j))
        bv[j] = true;
    }
  };

  auto gen_chk = [](const cse::BitVector& a, const cse::BitVector& b, cse::BitVector& chk){
    for(size_t i = 0; i < chk.size(); i++) {
      if(a.size() < i)
        chk[i] = b[i];
      else if (b.size() < i)
        chk[i] = a[i];
      else
        chk[i] = a[i] || b[i];
    }
  };
  
  auto op = [](cse::BitVector& bv1, cse::BitVector& bv2){
    bv1 |= bv2;
  };

  LAMBDA_TEST(gen, gen_chk, op);
}

TEST_CASE("Test bitwise XOR on BitVectors", "[bitvector]") {
  auto gen = [](cse::BitVector& bv, size_t i){
    for(size_t j = 0; j < bv.size(); j++) {
      if (1 & (i >> j))
        bv[j] = true;
    }
  };

  auto gen_chk = [](const cse::BitVector& a, const cse::BitVector& b, cse::BitVector& chk){
    for(size_t i = 0; i < chk.size(); i++) {
      if(a.size() < i)
        chk[i] = b[i];
      else if (b.size() < i)
        chk[i] = a[i];
      else
        chk[i] = a[i] != b[i];
    }
  };
  
  auto op = [](cse::BitVector& bv1, cse::BitVector& bv2){
    bv1 ^= bv2;
  };

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
