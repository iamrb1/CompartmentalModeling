#include "app/AssertTest.hpp"

#include "cse/CseAssert.hpp"

void test_assert() {
  dbg_assert(false);
  cse_assert(true);
}
