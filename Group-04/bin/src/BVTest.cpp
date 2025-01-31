#include <cse/BitVector.hpp>
#include <app/BVTest.hpp>
#include <string>
#include <iostream>

const std::string TEST_GUARD = "****************\n";

void test_bitvect_1() {
  std::cout << TEST_GUARD;
  std::cout << "TEST 1 - same length BitVector\n";

  cse::BitVector a(8);
  cse::BitVector b(8);

  std::cout << "a (uninitialized) - " << a << "\n";
  std::cout << "b (uninitialized) - " << b << "\n";

  a.set();

  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  b |= a;

  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  a[5] = a[4] = a[7] = false;

  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  b ^= a;
  
  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  b ^= a;
  
  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  b &= a;
  
  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  b ^= b;
  a ^= a;
  
  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  std::cout << TEST_GUARD;
}

void test_bitvect_2() {
  // Different length bitvectors
  std::cout << TEST_GUARD;
  std::cout << "TEST 2 - Different length bitvectors (same number of bytes)\n";

  cse::BitVector a(6);
  cse::BitVector b(8);

  a.set(false);
  b.set();

  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  a |= b;
  
  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  b ^= a;

  std::cout << "a - " << a << "\n";
  std::cout << "b - " << b << "\n";

  std::cout << TEST_GUARD;
}

void test_bitvect() {
  test_bitvect_1();
  test_bitvect_2();
}

