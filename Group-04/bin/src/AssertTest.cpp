#include "app/AssertTest.hpp"

#include "cse/CseAssert.hpp"

void test_assert() {
  // test basic assert
  cse_assert(true);

  // test assert with message
  cse_assert(true, "hello world");

  // test assert with comma
  cse_assert(std::is_same_v<int, int>);
  cse_assert(std::is_same_v<int, int>, "hello world");
}
