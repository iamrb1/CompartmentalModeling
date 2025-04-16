#include <array>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <type_traits>
#include <vector>
#include <type_traits>

#include "../../Group-10/Classes/PermutationManager.hpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

// Test 1: Using PermutationManager with std::vector<int>, no repeats, no required
TEST_CASE("PermutationManager: std::vector<int>, no repeats, no required elements",
          "[PermutationManager]") {
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  // There are P(5,2) = 20 combinations.
  cse::PermutationManager<std::vector<int>> permManager(numbers, 2, false);
  const std::size_t expectedTotal = 20;
  REQUIRE(permManager.TotalPermutations() == expectedTotal);


  int total = permManager.TotalPermutations();
  int t = 0;

  // store all forward permutations
  std::vector<std::vector<int>> res;
  do {
      const auto& perm = permManager.GetCurrentPermutation();
      res.push_back(perm);
      ++t;
  } while (permManager.Next());

  std::vector<std::vector<int>> expected = {{1, 2},{2, 1},{1, 3},{3, 1},{1, 4},{4, 1},{1, 5},{5, 1},{2, 3},{3, 2},{2, 4},
    {4, 2},{2, 5},{5, 2},{3, 4},{4, 3},{3, 5},{5, 3},{4, 5},{5, 4}
  };

  REQUIRE(res == expected);
  REQUIRE(t == total);

  res.clear();
  // Go backwards
  do {
    const auto& perm = permManager.GetCurrentPermutation();
    res.push_back(perm);
    --t;
  } while (permManager.Prev());

  expected = {{5, 4},{4, 5},{5, 3},{3, 5},{4, 3},{3, 4},{5, 2},{2, 5},
  {4, 2},{2, 4},{3, 2},{2, 3},{5, 1},{1, 5},{4, 1},{1, 4},
  {3, 1},{1, 3},{2, 1},{1, 2}};
 

  REQUIRE(res == expected);
  REQUIRE(t == 0);


  // new set of numbers and size to choose
  numbers = {1, 2, 3, 4, 5, 6};
  cse::PermutationManager<std::vector<int>> permManager2(numbers, 3, false);
  total = permManager2.TotalPermutations();

  // store all forward permutations
  res.clear();
  do {
      const auto& perm = permManager2.GetCurrentPermutation();
      res.push_back(perm);
      ++t;
  } while (permManager2.Next());

  expected = {
    {1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1},
    {1, 2, 4}, {1, 4, 2}, {2, 1, 4}, {2, 4, 1}, {4, 1, 2}, {4, 2, 1},
    {1, 2, 5}, {1, 5, 2}, {2, 1, 5}, {2, 5, 1}, {5, 1, 2}, {5, 2, 1},
    {1, 2, 6}, {1, 6, 2}, {2, 1, 6}, {2, 6, 1}, {6, 1, 2}, {6, 2, 1},
    {1, 3, 4}, {1, 4, 3}, {3, 1, 4}, {3, 4, 1}, {4, 1, 3}, {4, 3, 1},
    {1, 3, 5}, {1, 5, 3}, {3, 1, 5}, {3, 5, 1}, {5, 1, 3}, {5, 3, 1},
    {1, 3, 6}, {1, 6, 3}, {3, 1, 6}, {3, 6, 1}, {6, 1, 3}, {6, 3, 1},
    {1, 4, 5}, {1, 5, 4}, {4, 1, 5}, {4, 5, 1}, {5, 1, 4}, {5, 4, 1},
    {1, 4, 6}, {1, 6, 4}, {4, 1, 6}, {4, 6, 1}, {6, 1, 4}, {6, 4, 1},
    {1, 5, 6}, {1, 6, 5}, {5, 1, 6}, {5, 6, 1}, {6, 1, 5}, {6, 5, 1},
    {2, 3, 4}, {2, 4, 3}, {3, 2, 4}, {3, 4, 2}, {4, 2, 3}, {4, 3, 2},
    {2, 3, 5}, {2, 5, 3}, {3, 2, 5}, {3, 5, 2}, {5, 2, 3}, {5, 3, 2},
    {2, 3, 6}, {2, 6, 3}, {3, 2, 6}, {3, 6, 2}, {6, 2, 3}, {6, 3, 2},
    {2, 4, 5}, {2, 5, 4}, {4, 2, 5}, {4, 5, 2}, {5, 2, 4}, {5, 4, 2},
    {2, 4, 6}, {2, 6, 4}, {4, 2, 6}, {4, 6, 2}, {6, 2, 4}, {6, 4, 2},
    {2, 5, 6}, {2, 6, 5}, {5, 2, 6}, {5, 6, 2}, {6, 2, 5}, {6, 5, 2},
    {3, 4, 5}, {3, 5, 4}, {4, 3, 5}, {4, 5, 3}, {5, 3, 4}, {5, 4, 3},
    {3, 4, 6}, {3, 6, 4}, {4, 3, 6}, {4, 6, 3}, {6, 3, 4}, {6, 4, 3},
    {3, 5, 6}, {3, 6, 5}, {5, 3, 6}, {5, 6, 3}, {6, 3, 5}, {6, 5, 3},
    {4, 5, 6}, {4, 6, 5}, {5, 4, 6}, {5, 6, 4}, {6, 4, 5}, {6, 5, 4}
  };
  

  REQUIRE(res == expected);
  REQUIRE(total == t);
  res.clear();

  // now reverse
  do {
      const auto& perm = permManager2.GetCurrentPermutation();
      res.push_back(perm);
      --t;
  } while (permManager2.Prev());

  expected = {
    {6, 5, 4}, {6, 4, 5}, {5, 6, 4}, {5, 4, 6}, {4, 6, 5}, {4, 5, 6},
    {6, 5, 3}, {6, 3, 5}, {5, 6, 3}, {5, 3, 6}, {3, 6, 5}, {3, 5, 6},
    {6, 4, 3}, {6, 3, 4}, {4, 6, 3}, {4, 3, 6}, {3, 6, 4}, {3, 4, 6},
    {5, 4, 3}, {5, 3, 4}, {4, 5, 3}, {4, 3, 5}, {3, 5, 4}, {3, 4, 5},
    {6, 5, 2}, {6, 2, 5}, {5, 6, 2}, {5, 2, 6}, {2, 6, 5}, {2, 5, 6},
    {6, 4, 2}, {6, 2, 4}, {4, 6, 2}, {4, 2, 6}, {2, 6, 4}, {2, 4, 6},
    {5, 4, 2}, {5, 2, 4}, {4, 5, 2}, {4, 2, 5}, {2, 5, 4}, {2, 4, 5},
    {6, 3, 2}, {6, 2, 3}, {3, 6, 2}, {3, 2, 6}, {2, 6, 3}, {2, 3, 6},
    {5, 3, 2}, {5, 2, 3}, {3, 5, 2}, {3, 2, 5}, {2, 5, 3}, {2, 3, 5},
    {4, 3, 2}, {4, 2, 3}, {3, 4, 2}, {3, 2, 4}, {2, 4, 3}, {2, 3, 4},
    {6, 5, 1}, {6, 1, 5}, {5, 6, 1}, {5, 1, 6}, {1, 6, 5}, {1, 5, 6},
    {6, 4, 1}, {6, 1, 4}, {4, 6, 1}, {4, 1, 6}, {1, 6, 4}, {1, 4, 6},
    {5, 4, 1}, {5, 1, 4}, {4, 5, 1}, {4, 1, 5}, {1, 5, 4}, {1, 4, 5},
    {6, 3, 1}, {6, 1, 3}, {3, 6, 1}, {3, 1, 6}, {1, 6, 3}, {1, 3, 6},
    {5, 3, 1}, {5, 1, 3}, {3, 5, 1}, {3, 1, 5}, {1, 5, 3}, {1, 3, 5},
    {4, 3, 1}, {4, 1, 3}, {3, 4, 1}, {3, 1, 4}, {1, 4, 3}, {1, 3, 4},
    {6, 2, 1}, {6, 1, 2}, {2, 6, 1}, {2, 1, 6}, {1, 6, 2}, {1, 2, 6},
    {5, 2, 1}, {5, 1, 2}, {2, 5, 1}, {2, 1, 5}, {1, 5, 2}, {1, 2, 5},
    {4, 2, 1}, {4, 1, 2}, {2, 4, 1}, {2, 1, 4}, {1, 4, 2}, {1, 2, 4},
    {3, 2, 1}, {3, 1, 2}, {2, 3, 1}, {2, 1, 3}, {1, 3, 2}, {1, 2, 3}
  };
  
  
  REQUIRE(res == expected);
  REQUIRE(t == 0);
  
}

// Test 2: Using PermutationManager with std::vector<int> required index, yes repeating elements
TEST_CASE("PermutationManager: std::vector<int> required index, yes repeating elements",
  "[PermutationManager]") {
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  // There are P(5,2) = 20 combinations.
  cse::PermutationManager<std::vector<int>> permManager(numbers, 2, true, 1);

  // store all forward permutations
  std::vector<std::vector<int>> res;
  do {
    const auto& perm = permManager.GetCurrentPermutation();
    res.push_back(perm);
  } while (permManager.Next());

  std::vector<std::vector<int>> expected = {
    {2, 1}, {1, 2},
    {2, 3}, {3, 2},
    {2, 4}, {4, 2},
    {2, 5}, {5, 2}
  };

  REQUIRE(res == expected);

  // now backwards
  res.clear();
  do {
  const auto& perm = permManager.GetCurrentPermutation();
  res.push_back(perm);
  } while (permManager.Prev());

  expected = {
    {5, 2}, {2, 5},
    {4, 2}, {2, 4},
    {3, 2}, {2, 3},
    {1, 2}, {2, 1}
  };

  REQUIRE(res == expected);
}

// Test 3: Using PermutationManager with std::vector<int> with required index, no repeating elements
TEST_CASE("PermutationManager: std::vector<int> required index, no repeating elements",
  "[PermutationManager]")  {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    cse::PermutationManager<std::vector<int>> permManager(numbers, 3, false, 1);

    // store all forward permutations
    std::vector<std::vector<int>> res;
    do {
      const auto& perm = permManager.GetCurrentPermutation();
      res.push_back(perm);
    } while (permManager.Next());

    std::vector<std::vector<int>> expected = {
      {2, 1, 3}, {1, 2, 3}, {1, 3, 2},
      {2, 3, 1}, {3, 2, 1}, {3, 1, 2},
      {2, 1, 4}, {1, 2, 4}, {1, 4, 2},
      {2, 4, 1}, {4, 2, 1}, {4, 1, 2}
    };

    REQUIRE(res == expected);

    // now backwards
    res.clear();
    do {
      const auto& perm = permManager.GetCurrentPermutation();
      res.push_back(perm);
    } while (permManager.Prev());
    expected = {
      {4, 1, 2}, {4, 2, 1}, {2, 4, 1},
      {1, 4, 2}, {1, 2, 4}, {2, 1, 4},
      {3, 1, 2}, {3, 2, 1}, {2, 3, 1},
      {1, 3, 2}, {1, 2, 3}, {2, 1, 3}
    };

    REQUIRE(res == expected);
    
}

// Test 3: Using PermutationManager with std::vector<int> with no required index, yes repeating elements
TEST_CASE("PermutationManager: std::vector<int> no required index, yes repeating elements",
  "[PermutationManager]") {
  std::vector<int> items = {1, 2, 3, 4};
  cse::PermutationManager<std::vector<int>> permManager(items, 3, true);


  // store all forward permutations
  std::vector<std::vector<int>> res;
  do {
    const auto& perm = permManager.GetCurrentPermutation();
    res.push_back(perm);
  } while (permManager.Next());
  
  std::vector<std::vector<int>> expected = {
    {1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1},
    {1, 2, 4}, {1, 4, 2}, {2, 1, 4}, {2, 4, 1}, {4, 1, 2}, {4, 2, 1},
    {1, 3, 3}, {3, 1, 3}, {3, 3, 1},
    {1, 3, 4}, {1, 4, 3}, {3, 1, 4}, {3, 4, 1}, {4, 1, 3}, {4, 3, 1},
    {1, 4, 4}, {4, 1, 4}, {4, 4, 1},
    {2, 2, 2}, {2, 2, 3}, {2, 3, 2}, {3, 2, 2},
    {2, 2, 4}, {2, 4, 2}, {4, 2, 2},
    {2, 3, 3}, {3, 2, 3}, {3, 3, 2},
    {2, 3, 4}, {2, 4, 3}, {3, 2, 4}, {3, 4, 2}, {4, 2, 3}, {4, 3, 2},
    {2, 4, 4}, {4, 2, 4}, {4, 4, 2},
    {3, 3, 3}, {3, 3, 4}, {3, 4, 3}, {4, 3, 3},
    {3, 4, 4}, {4, 3, 4}, {4, 4, 3},
    {4, 4, 4}
  };

  REQUIRE(res == expected);

  // now check prev
  res.clear();
  do {
    const auto& perm = permManager.GetCurrentPermutation();
    res.push_back(perm);
  } while (permManager.Prev());
  expected = {
    {4, 4, 4},
    {4, 4, 3}, {4, 3, 4}, {3, 4, 4},
    {4, 3, 3}, {3, 4, 3}, {3, 3, 4}, {3, 3, 3},
    {4, 4, 2}, {4, 2, 4}, {2, 4, 4},
    {4, 3, 2}, {4, 2, 3}, {3, 4, 2}, {3, 2, 4}, {2, 4, 3}, {2, 3, 4},
    {3, 3, 2}, {3, 2, 3}, {2, 3, 3},
    {4, 2, 2}, {2, 4, 2}, {2, 2, 4},
    {3, 2, 2}, {2, 3, 2}, {2, 2, 3}, {2, 2, 2},
    {4, 4, 1}, {4, 1, 4}, {1, 4, 4},
    {4, 3, 1}, {4, 1, 3}, {3, 4, 1}, {3, 1, 4}, {1, 4, 3}, {1, 3, 4},
    {3, 3, 1}, {3, 1, 3}, {1, 3, 3},
    {4, 2, 1}, {4, 1, 2}, {2, 4, 1}, {2, 1, 4}, {1, 4, 2}, {1, 2, 4},
    {3, 2, 1}, {3, 1, 2}, {2, 3, 1}, {2, 1, 3}, {1, 3, 2}, {1, 2, 3}
  };

  REQUIRE(res == expected);
  
}


// Test 4: Using PermutationManager with std::list<std::string> with no required index, no repeating elements
TEST_CASE("PermutationManager: std::list<std::string> no required index, no repeating elements",
  "[PermutationManager]") {
  std::list<std::string> fruits = {"apple", "banana", "cherry", "date"};
  cse::PermutationManager permManager(fruits, 3, false);
  int total = permManager.TotalPermutations();
  const int EXPECTED_TOTAL = 24;
  REQUIRE(total == EXPECTED_TOTAL);

  int t = 0;

  std::vector<std::vector<std::string>> res;
  do {
    const auto& perm = permManager.GetCurrentPermutation();
    res.push_back(perm);
    ++t;
  } while (permManager.Next());

  std::vector<std::vector<std::string>> expected = {
    {"apple", "banana", "cherry"},{"apple", "cherry", "banana"},{"banana", "apple", "cherry"},
    {"banana", "cherry", "apple"},{"cherry", "apple", "banana"},{"cherry", "banana", "apple"},
    {"apple", "banana", "date"},{"apple", "date", "banana"},{"banana", "apple", "date"},
    {"banana", "date", "apple"},{"date", "apple", "banana"},{"date", "banana", "apple"},
    {"apple", "cherry", "date"},{"apple", "date", "cherry"},{"cherry", "apple", "date"},
    {"cherry", "date", "apple"},{"date", "apple", "cherry"},{"date", "cherry", "apple"},
    {"banana", "cherry", "date"},{"banana", "date", "cherry"},{"cherry", "banana", "date"},
    {"cherry", "date", "banana"},{"date", "banana", "cherry"},{"date", "cherry", "banana"}
  };
  REQUIRE(res == expected);

  // now test previous
  res.clear();
  do {
    const auto& perm = permManager.GetCurrentPermutation();
    res.push_back(perm);
    --t;
  } while (permManager.Prev());

  expected = {
    {"date", "cherry", "banana"},{"date", "banana", "cherry"},{"cherry", "date", "banana"},
    {"cherry", "banana", "date"},{"banana", "date", "cherry"},{"banana", "cherry", "date"},
    {"date", "cherry", "apple"},{"date", "apple", "cherry"},{"cherry", "date", "apple"},
    {"cherry", "apple", "date"},{"apple", "date", "cherry"},{"apple", "cherry", "date"},
    {"date", "banana", "apple"},{"date", "apple", "banana"},{"banana", "date", "apple"},
    {"banana", "apple", "date"},{"apple", "date", "banana"},{"apple", "banana", "date"},
    {"cherry", "banana", "apple"},{"cherry", "apple", "banana"},{"banana", "cherry", "apple"},
    {"banana", "apple", "cherry"},{"apple", "cherry", "banana"},{"apple", "banana", "cherry"}
  };
  REQUIRE(res == expected);
  REQUIRE(t == 0);

  
}