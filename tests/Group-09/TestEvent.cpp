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
  Event<int> e(0, 1, funcInt);
  CHECK(e.getID() == 0);
  CHECK(e.getTime() == 1);
  e.getFunction()(5);
  CHECK(globInt == 5);
}

TEST_CASE("Test Event String arg", "[base]")
{
  Event<std::string> e(0, 1, funcString);
  CHECK(e.getID() == 0);
  CHECK(e.getTime() == 1);
  e.getFunction()(std::string("hello"));
  CHECK(globStr == std::string("hello"));
}

TEST_CASE("Test Event multiple args", "[base]")
{
  Event<int, std::string> e(0, 1, multipleArgs);
  CHECK(e.getID() == 0);
  CHECK(e.getTime() == 1);
  e.getFunction()(1, std::string("goodbye"));
  CHECK(globInt == 1);
  CHECK(globStr == std::string("goodbye"));
}