#include <cse/BitVector.hpp>
#include <app/BVTest.hpp>
#include <string>
#include <iostream>
#include <assert.h>

const std::string TEST_GUARD = "****************\n";

void test_bitvect_1() {
  std::cout << TEST_GUARD;
  std::cout << "TEST 1 - Logical AND\n";

  cse::BitVector a(8);
  cse::BitVector b(8);

  std::cout << "*** 1a - same bit length (same value) ***\n";
  for(uint16_t i = 0; i < 256; ++i) {
    a.set(false);
    b.set(false);

    for(uint16_t j = 0; j < 8; ++j) {
      if (((i >> j) & 1) == 1)
        a[j] = b[j] = true;
    }

    b &= a;
    
    if (b != a || b.count() != a.count()){
      std::cout << "a - " << a << " count: " << a.count() << "\n";
      std::cout << "b - " << b << " count: " << b.count() << "\n";
      std::cout << "TEST FAILED\n";
    }
  }

  std::cout << "*** 1a - OK ***\n";


  std::cout << TEST_GUARD;
}

void test_bitvect_2() {
  std::cout << TEST_GUARD;
  std::cout << "TEST 2 - Logical OR\n";

  cse::BitVector a(8);
  cse::BitVector b(8);

  std::cout << "*** 2a - same bit length (same value) ***\n";
  for(uint16_t i = 0; i < 256; ++i) {
    a.set(false);
    b.set(false);

    for(uint16_t j = 0; j < 8; ++j) {
      if (((i >> j) & 1) == 1)
        a[j] = b[j] = true;
    }

    b |= a;
    
    if (b != a || b.count() != a.count()){
      std::cout << "a - " << a << " count: " << a.count() << "\n";
      std::cout << "b - " << b << " count: " << b.count() << "\n";
      std::cout << "TEST FAILED\n";
    }
  }

  std::cout << "*** 2a - OK ***\n";


  std::cout << TEST_GUARD;
}

void test_bitvect_3() {
  std::cout << TEST_GUARD;
  std::cout << "TEST 3 - Logical XOR\n";

  cse::BitVector a(8);
  cse::BitVector b(8);
  cse::BitVector e(8);
  e.set(false);

  std::cout << "*** 3a - same bit length (same value) ***\n";
  for(uint16_t i = 0; i < 256; ++i) {
    a.set(false);
    b.set(false);

    for(uint16_t j = 0; j < 8; ++j) {
      if (((i >> j) & 1) == 1)
        a[j] = b[j] = true;
    }

    b ^= a;
    
    if (b != e || b.count() != 0){
      std::cout << "a - " << a << " count: " << a.count() << "\n";
      std::cout << "b - " << b << " count: " << b.count() << "\n";
      std::cout << "TEST FAILED\n";
    }
  }

  std::cout << "*** 3a - OK ***\n";


  std::cout << TEST_GUARD;
}

void test_bitvect() {
  test_bitvect_1();
  test_bitvect_2();
  test_bitvect_3();
}

