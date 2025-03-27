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