/*
 * AuditedArrayTest.cpp
 */
#include "AuditedArray.hpp"
#include <iostream>
#include <cassert>

class AuditedArrayTest {
public:
  static void testAccess() {
    AuditedArray<int, 5> arr;
    assert (arr.size() == 5);

    std::cout<<"testAccess PASSED"<<std::endl;
  }

  static void runAllTests() {
    testAccess();
  }
};

int main() {
  AuditedArrayTest::runAllTests();
  return 0;
}
