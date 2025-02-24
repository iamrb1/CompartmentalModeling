#define CSE4_IMPL
#include "BVTest.hpp"
#include "index_set_test.hpp"
#include "CseStringTest.hpp"

int main() {
  test_bitvect();
  index_set_test();
  cse_string_test();
  return 0;
}
