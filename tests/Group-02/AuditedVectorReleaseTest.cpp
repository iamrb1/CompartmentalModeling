/**
* @file AuditedVectorTestRelease.cpp
 * @author Matthew Hawkins
 *
 * In release mode, our audited vector is just an alias for std::vector.
 * Thus, we test its behavior as a normal vector.
 */

#define NDEBUG
#include <cse/AuditedVector.h>
#include <algorithm>
#include <catch2/catch.hpp>
#include <type_traits>
#include <vector>

// Basic element access tests
TEST_CASE("AuditedVector Release: operator[] works like std::vector", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec{10, 20, 30};

  SECTION("Valid index access returns the correct elements") {
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
    REQUIRE(vec[2] == 30);
  }
}

TEST_CASE("AuditedVector Release: front(), back(), and pop_back() work normally", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec{100, 200, 300};

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

TEST_CASE("AuditedVector Release: behaves like std::vector overall", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec;
  for (int i = 0; i < 5; ++i)
    vec.push_back(i * 10);
  REQUIRE(vec.size() == 5);
  for (int i = 0; i < 5; ++i)
    REQUIRE(vec[i] == i * 10);
}

TEST_CASE("AuditedVector Release: push_back, pop_back, clear, and empty", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec;
  REQUIRE(vec.empty());
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  REQUIRE(vec.size() == 3);
  REQUIRE(vec.front() == 1);
  REQUIRE(vec.back() == 3);
  vec.pop_back();
  REQUIRE(vec.size() == 2);
  vec.clear();
  REQUIRE(vec.empty());
}

TEST_CASE("AuditedVector Release: insert and erase", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec{1, 2, 3, 4};
  auto it = vec.insert(vec.begin() + 2, 99);
  REQUIRE(*it == 99);
  REQUIRE(vec.size() == 5);
  vec.erase(it);
  REQUIRE(vec.size() == 4);
  REQUIRE(vec[2] == 3);
}

TEST_CASE("AuditedVector Release: copy constructor", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec{10, 20, 30};
  cse::AuditedVector<int> vec_copy(vec);
  REQUIRE(vec_copy.size() == vec.size());
  for (size_t i = 0; i < vec.size(); ++i)
    REQUIRE(vec_copy[i] == vec[i]);
}

TEST_CASE("AuditedVector Release: assignment operator", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec{10, 20, 30};
  cse::AuditedVector<int> vec2;
  vec2 = vec;
  REQUIRE(vec2.size() == vec.size());
  for (size_t i = 0; i < vec.size(); ++i)
    REQUIRE(vec2[i] == vec[i]);
}

TEST_CASE("AuditedVector Release: iterator traversal and std::find", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec{5, 10, 15, 20};
  int sum = 0;
  for (auto it = vec.begin(); it != vec.end(); ++it)
    sum += *it;
  REQUIRE(sum == 50);

  auto it = std::find(vec.begin(), vec.end(), 15);
  REQUIRE(it != vec.end());
  REQUIRE(*it == 15);
}

TEST_CASE("AuditedVector Release: resize and reserve", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec{1, 2, 3};
  vec.resize(5, 99);
  REQUIRE(vec.size() == 5);
  REQUIRE(vec[3] == 99);
  vec.resize(2);
  REQUIRE(vec.size() == 2);
  REQUIRE(vec[1] == 2);

  // Reserve doesn't change size.
  vec.reserve(100);
  REQUIRE(vec.size() == 2);
}

TEST_CASE("AuditedVector Release: default constructor and equality", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec1;
  cse::AuditedVector<int> vec2;
  REQUIRE(vec1 == vec2);
  vec1.push_back(42);
  REQUIRE_FALSE(vec1 == vec2);
  vec2.push_back(42);
  REQUIRE(vec1 == vec2);
}

TEST_CASE("AuditedVector Release: at() method", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec{1, 2, 3, 4};
  REQUIRE(vec.at(0) == 1);
  REQUIRE(vec.at(3) == 4);
  REQUIRE_THROWS_AS(vec.at(4), std::out_of_range);
}

TEST_CASE("AuditedVector Release: swap functionality", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec1{1, 2, 3};
  cse::AuditedVector<int> vec2{4, 5};
  vec1.swap(vec2);
  REQUIRE(vec1.size() == 2);
  REQUIRE(vec2.size() == 3);
  REQUIRE(vec1[0] == 4);
  REQUIRE(vec2[0] == 1);
}

TEST_CASE("AuditedVector Release: move constructor and move assignment", "[AuditedVector][release]") {
  cse::AuditedVector<int> original{10, 20, 30};
  cse::AuditedVector<int> moved(std::move(original));
  REQUIRE(moved.size() == 3);
  cse::AuditedVector<int> another{100, 200};
  another = std::move(moved);
  REQUIRE(another.size() == 3);
}

TEST_CASE("AuditedVector Release: assign and emplace_back", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec;
  vec.assign({7, 8, 9});
  REQUIRE(vec.size() == 3);
  REQUIRE(vec[0] == 7);
  vec.emplace_back(10);
  REQUIRE(vec.back() == 10);
}

TEST_CASE("AuditedVector Release: range insert and erase", "[AuditedVector][release]") {
  cse::AuditedVector<int> vec{1, 2, 3, 4, 5};
  std::vector<int> extras{100, 200, 300};
  auto it = vec.insert(vec.begin() + 2, extras.begin(), extras.end());
  REQUIRE(vec.size() == 8);
  vec.erase(it, it + extras.size());
  REQUIRE(vec.size() == 5);
  REQUIRE(vec[2] == 3);
}

TEST_CASE("AuditedVector Release: alias to std::vector", "[AuditedVector][release]") {
  STATIC_REQUIRE(std::is_same_v<cse::AuditedVector<int>, std::vector<int>>);
}
