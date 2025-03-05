/**
 * @file TestEvent.cpp
 *
 * @author Owen Haiar
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-09/Event/Event.hpp"

using namespace cse;
int FUNCTION_CALLS = 0;

TEST_CASE("Test Event", "[base]")
{
  Event e(0, 1, std::function<void()>([]() {}));
  CHECK(e.getID() == 0);
  CHECK(e.getTime() == 1);
  CHECK(e.getAction() == std::function<void()>([]() {})); // check if the function is correct
}

TEST_CASE("Test Event execute", "[base]")
{
  Event e(0, 1, std::function<void()>([]() { FUNCTION_CALLS++; }));
  e.execute();
  CHECK(functionCalls == 1); // check if the function was called
}