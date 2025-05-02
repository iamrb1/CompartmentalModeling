/**
 * @file TestEvent.cpp
 *
 * @author Owen Haiar, Grace Fizgerald
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

void throwsException() {
  throw std::runtime_error("Intentional");
}


TEST_CASE("Test Event Int arg", "[base]")
{
  // Reset globals
  globInt = 0;
  globStr = "";
  // Create an event that calls funcInt with 5
  Event<int> e("0", 1, funcInt, 5);
  CHECK(e.getID() == "0");
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
  Event<std::string> e("0", 1, funcString, std::string("hello"));
  CHECK(e.getID() == "0");
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
  Event<int, std::string> e("0", 1, multipleArgs, 1, std::string("goodbye"));
  CHECK(e.getID() == "0");
  CHECK(e.getTime() == 1);
  CHECK(globInt == 0);
  CHECK(globStr == std::string(""));
  e.execute(); // Execute
  CHECK(globInt == 1); // globInt should now be 1
  CHECK(globStr == std::string("goodbye")); //globString should now be "goodbye"
}

TEST_CASE("Test Event setTime", "[base]") {
  Event<int> e("1", 10, funcInt, 3);
  CHECK(e.getTime() == 10);
  for(int i = 0; i < 100; i++) {
    e.setTime(i);
    CHECK(e.getTime() == i);
  }

}


TEST_CASE("Test Event with empty function", "[error]") {
  std::function<void()> nullFunc;
  Event<> e("null", 5, nullFunc);
  CHECK_THROWS_AS(e.execute(), std::bad_function_call);
}

TEST_CASE("Test execute() modifies globals - int", "[base]") {
  globInt = 0;
  Event<int> e("ABC", 3, funcInt, 42);
  e.execute();
  CHECK(globInt == 42);
}

TEST_CASE("Test execute() modifies globals - string", "[base]") {
  globStr = "";
  Event<std::string> e("A", 7, funcString, "hello");
  e.execute();
  CHECK(globStr == "hello");
}

TEST_CASE("Test execute() with multiple arguments", "[base]") {
  globInt = 0;
  globStr = "";
  Event<int, std::string> e("AandB", 2, multipleArgs, 7, "world");
  e.execute();
  CHECK(globInt == 7);
  CHECK(globStr == "world");
}

TEST_CASE("Test operator==", "[base]") {
  Event<int> e1("X", 1, funcInt, 1);
  Event<int> e2("X", 99, funcInt, 999);
  Event<int> e3("Y", 1, funcInt, 1);
  CHECK(e1 == e2);
  CHECK_FALSE(e1 == e3);
}

TEST_CASE("Test Event with lambda function", "[base]") {
  int x = 0;
  auto lambda = [&x](int a) { x = a + 10; };
  Event<int> e("L", 4, lambda, 5);
  e.execute();
  CHECK(x == 15);
}

TEST_CASE("Test Event exception thrown inside function", "[base][exception]") {
  Event<> e("Thrower", 1, throwsException);
  CHECK_THROWS_WITH(e.execute(), "Intentional");
}