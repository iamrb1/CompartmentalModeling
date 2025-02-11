/**
 * @file AuditedVectorTestDebug.cpp
 * @author Matthew Hawkins
 */

#define CATCH_CONFIG_MAIN
#include "../../include/third-party/Catch/single_include/catch2/catch.hpp"
#include "../../include/cse/AuditedVector.h"
#include <stdexcept>

TEST_CASE("AuditedVector Debug: operator[] bounds checking", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec { 10, 20, 30 };

  SECTION("Valid indices do not throw") {
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
    REQUIRE(vec[2] == 30);
  }

  SECTION("Accessing an out-of-range index throws std::out_of_range") {
    // In debug mode, our operator[] checks bounds.
    REQUIRE_THROWS_AS(vec[3], std::out_of_range);
  }
}

TEST_CASE("AuditedVector Debug: front(), back(), and pop_back() error checking", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec;

  SECTION("front() on an empty vector throws") {
    REQUIRE_THROWS_AS(vec.front(), std::out_of_range);
  }

  SECTION("back() on an empty vector throws") {
    REQUIRE_THROWS_AS(vec.back(), std::out_of_range);
  }

  SECTION("pop_back() on an empty vector throws") {
    REQUIRE_THROWS_AS(vec.pop_back(), std::out_of_range);
  }
}

TEST_CASE("AuditedVector Debug: normal operations", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec { 100, 200, 300 };

  SECTION("front() returns the first element") {
    REQUIRE(vec.front() == 100);
  }

  SECTION("back() returns the last element") {
    REQUIRE(vec.back() == 300);
  }

  SECTION("pop_back() removes the last element") {
    vec.pop_back();
    REQUIRE(vec.size() == 2);
    REQUIRE(vec.back() == 200);
  }
}
