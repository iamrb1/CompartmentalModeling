/*
 * AuditedArrayTest.cpp
 */
#include <cassert>
#include <iostream>

#include "AuditedArray.hpp"

class AuditedArrayTest {
 public:
  static void testAccess() {
    AuditedArray<int, 5> arr;
    assert(arr.size() == 5);

    std::cout << "testAccess PASSED" << std::endl;
  }
  static void testOperators() {
    AuditedArray<int, 5> arr;

    // test []
    arr[0] = 1;
    assert(arr[0] == 1);

    // test =
    AuditedArray<int, 5> arr2 = arr;
    for (int i = 0; i < 5; i++) {
      assert(arr2[i] == arr[i]);
    }
    std::cout << "testOperators PASSED" << std::endl;
  }
  static void testHelpers() {
    AuditedArray<int, 5> arr;
    arr.fill(1);
    for (int i = 0; i < 5; i++) {
      assert(arr[i] == 1);
    }
    arr[1] = 3;
    assert(arr.contains(3));
    assert(!arr.contains(2));

    assert(arr.indexOf(1) == 0);
    assert(arr.indexOf(2) == -1);

    std::cout << "testHelpers PASSED" << std::endl;
  }
  static void runAllTests() {
    testAccess();
    testOperators();
  }
};

int main() {
  AuditedArrayTest::runAllTests();
  return 0;
}
