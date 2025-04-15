/**
 * @file TestEvent.cpp
 *
 * @author Owen Haiar
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-09/Event/Event.hpp"

using namespace cse;

int globInt = 0;
std::string globStr;

void funcInt(int x) {
  globInt = x;
}

void funcString(std::string x) {
  globStr = x;
}

void multipleArgs(int y, std::string x) {
  globInt = y;
  globStr = x;
}

TEST_CASE("Test Event Int arg", "[base]")
{
  // Reset globals
  globInt = 0;
  globStr = "";
  // Create an event that calls funcInt with 5
  Event<int> e(0, 1, funcInt, 5);
  CHECK(e.getID() == 0);
  CHECK(e.getTime() == 1);
  CHECK(globInt == 0);
  e.execute(); // Execute
  CHECK(globInt == 5); // globInt should now be 5
}

TEST_CASE("Test Event String arg", "[base]")
{
  // Reset globals
  globInt = 0;
  globStr = "";
  // Create an event that calls funcString with "hello"
  Event<std::string> e(0, 1, funcString, std::string("hello"));
  CHECK(e.getID() == 0);
  CHECK(e.getTime() == 1);
  CHECK(globStr == std::string(""));
  e.execute(); // Execute
  CHECK(globStr == std::string("hello")); // globStr should now be "hello"
}

TEST_CASE("Test Event multiple args", "[base]")
{
  // Reset globals
  globInt = 0;
  globStr = "";
  // Create an event that calls multipleArgs with 1 and "goodbye"
  Event<int, std::string> e(0, 1, multipleArgs, 1, std::string("goodbye"));
  CHECK(e.getID() == 0);
  CHECK(e.getTime() == 1);
  CHECK(globInt == 0);
  CHECK(globStr == std::string(""));
  e.execute(); // Execute
  CHECK(globInt == 1); // globInt should now be 1
  CHECK(globStr == std::string("goodbye")); //globString should now be "goodbye"
}