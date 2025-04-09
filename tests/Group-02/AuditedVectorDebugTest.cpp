/**
* @file AuditedVectorTestDebug.cpp
 * @author Matthew Hawkins
 *
 * Note: In debug mode the audited vector uses assert to check preconditions.
 * Because an assertion failure aborts the program, we only test valid operations here.
 */

#include <cse/AuditedVector.hpp>
#include <algorithm>
#include <catch2/catch.hpp>
#include <type_traits>
#include <vector>

TEST_CASE("AuditedVector Debug: operator[] valid indices", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec{10, 20, 30};

  SECTION("Valid indices return correct values") {
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
    REQUIRE(vec[2] == 30);
  }
}

TEST_CASE("AuditedVector Debug: front(), back(), and pop_back() on non-empty vector", "[AuditedVector][debug]") {
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

TEST_CASE("AuditedVector Debug: push_back, clear, and empty", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec;
  REQUIRE(vec.empty());
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  REQUIRE(vec.size() == 3);
  REQUIRE_FALSE(vec.empty());
  vec.clear();
  REQUIRE(vec.empty());
}

TEST_CASE("AuditedVector Debug: insert and erase", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec{1, 2, 3, 4};
  auto it = vec.insert(vec.begin() + 2, 99);
  REQUIRE(*it == 99);
  REQUIRE(vec.size() == 5);
  vec.erase(it);
  REQUIRE(vec.size() == 4);
  REQUIRE(vec[2] == 3);
}

TEST_CASE("AuditedVector Debug: copy constructor and assignment operator", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec{10, 20, 30};

  SECTION("Copy constructor") {
    cse::AuditedVector<int> vec_copy(vec);
    REQUIRE(vec_copy.size() == vec.size());
    for (size_t i = 0; i < vec.size(); ++i)
      REQUIRE(vec_copy[i] == vec[i]);
  }

  SECTION("Assignment operator") {
    cse::AuditedVector<int> vec2;
    vec2 = vec;
    REQUIRE(vec2.size() == vec.size());
    for (size_t i = 0; i < vec.size(); ++i)
      REQUIRE(vec2[i] == vec[i]);
  }
}

TEST_CASE("AuditedVector Debug: iterator traversal and std::find", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec{5, 10, 15, 20};
  int sum = 0;
  for (auto it = vec.begin(); it != vec.end(); ++it)
    sum += *it;
  REQUIRE(sum == 50);

  auto it = std::find(vec.begin(), vec.end(), 15);
  REQUIRE(it != vec.end());
  REQUIRE(*it == 15);
}

TEST_CASE("AuditedVector Debug: resize and reserve", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec{1, 2, 3};
  vec.resize(5, 99);
  REQUIRE(vec.size() == 5);
  REQUIRE(vec[3] == 99);
  vec.resize(2);
  REQUIRE(vec.size() == 2);
  REQUIRE(vec[1] == 2);

  vec.reserve(100);
  REQUIRE(vec.size() == 2);
}

TEST_CASE("AuditedVector Debug: default constructor and equality", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec1;
  cse::AuditedVector<int> vec2;
  REQUIRE(vec1 == vec2);
  vec1.push_back(42);
  REQUIRE_FALSE(vec1 == vec2);
  vec2.push_back(42);
  REQUIRE(vec1 == vec2);
}

TEST_CASE("AuditedVector Debug: at() method valid access", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec{1, 2, 3, 4};
  REQUIRE(vec.at(0) == 1);
  REQUIRE(vec.at(3) == 4);
}

TEST_CASE("AuditedVector Debug: swap functionality", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec1{1, 2, 3};
  cse::AuditedVector<int> vec2{4, 5};
  vec1.swap(vec2);
  REQUIRE(vec1.size() == 2);
  REQUIRE(vec2.size() == 3);
  REQUIRE(vec1[0] == 4);
  REQUIRE(vec2[0] == 1);
}

TEST_CASE("AuditedVector Debug: move constructor and move assignment", "[AuditedVector][debug]") {
  cse::AuditedVector<int> original{10, 20, 30};
  cse::AuditedVector<int> moved(std::move(original));
  REQUIRE(moved.size() == 3);
  cse::AuditedVector<int> another{100, 200};
  another = std::move(moved);
  REQUIRE(another.size() == 3);
}

TEST_CASE("AuditedVector Debug: assign and emplace_back", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec;
  vec.assign({7, 8, 9});
  REQUIRE(vec.size() == 3);
  REQUIRE(vec[0] == 7);
  vec.emplace_back(10);
  REQUIRE(vec.back() == 10);
}

TEST_CASE("AuditedVector Debug: range insert and erase", "[AuditedVector][debug]") {
  cse::AuditedVector<int> vec{1, 2, 3, 4, 5};
  std::vector<int> extras{100, 200, 300};
  auto it = vec.insert(vec.begin() + 2, extras.begin(), extras.end());
  REQUIRE(vec.size() == 8);
  vec.erase(it, it + extras.size());
  REQUIRE(vec.size() == 5);
  REQUIRE(vec[2] == 3);
}
