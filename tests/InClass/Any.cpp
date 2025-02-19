#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../InClass/Group-00/Any.hpp"

TEST_CASE("Test cse::Any", "[base]")
{
  cse::Any any1(100);
  cse::Any any2("abcd");

  CHECK(any1.Get<int>() == 100);
}