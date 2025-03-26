#include <array>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <type_traits>
#include <vector>
#include <type_traits>

#include "../../Group-10/Classes/ComboManager.hpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

// Test 1: Using ComboManager with std::vector<int>
TEST_CASE("ComboManager: std::vector<int> with combination size 2",
          "[ComboManager]") {
  std::vector<int> numbers = {0, 1, 2, 3, 4};
  // There are C(5,2) = 10 combinations.
  cse::ComboManager<std::vector<int>> cm(numbers, 2);
  const std::size_t expectedTotal = 10;
  REQUIRE(cm.TotalCombinations() == expectedTotal);

  int count = 0;
  // Iterate forward through all combinations.
  do {
    std::vector<int> combo = cm.GetCurrentCombo();
    INFO("Forward Combo " << count << ": " << (combo.empty() ? "(empty)" : ""));
    count++;
  } while (cm.NextCombo());
  REQUIRE(count == expectedTotal);

  // Now, iterate backward using PrevCombo.
  int reverseCount = 0;
  while (cm.PrevCombo()) {
    std::vector<int> combo = cm.GetCurrentCombo();
    INFO("Reverse Combo " << reverseCount << ": "
                          << (combo.empty() ? "(empty)" : ""));
    reverseCount++;
  }
  // Verify that after going back, the first combination is reached: {0, 1}
  std::vector<int> firstCombo = cm.GetCurrentCombo();
  REQUIRE(firstCombo.size() == 2);
  REQUIRE(firstCombo[0] == 0);
  REQUIRE(firstCombo[1] == 1);
}

// Test 2: Using ComboManager with std::list<std::string>
TEST_CASE("ComboManager: std::list<std::string> with combination size 2",
          "[ComboManager]") {
  std::list<std::string> fruits = {"apple", "banana", "cherry", "date"};
  // There are C(4,2) = 6 combinations.
  cse::ComboManager<std::list<std::string>> cm(fruits, 2);
  const std::size_t expectedTotal = 6;
  REQUIRE(cm.TotalCombinations() == expectedTotal);

  int count = 0;
  do {
    std::vector<std::string> combo = cm.GetCurrentCombo();
    INFO("Forward Combo " << count << ": " << (combo.empty() ? "(empty)" : ""));
    count++;
  } while (cm.NextCombo());
  REQUIRE(count == expectedTotal);

  // Iterate backwards.
  int reverseCount = 0;
  while (cm.PrevCombo()) {
    std::vector<std::string> combo = cm.GetCurrentCombo();
    INFO("Reverse Combo " << reverseCount << ": "
                          << (combo.empty() ? "(empty)" : ""));
    reverseCount++;
  }
  std::vector<std::string> firstCombo = cm.GetCurrentCombo();
  REQUIRE(firstCombo.size() == 2);
  REQUIRE(firstCombo[0] == "apple");
  REQUIRE(firstCombo[1] == "banana");
}

// Test 3: Using ComboManager with std::array<double, 5>
TEST_CASE("ComboManager: std::array<double, 5> with combination size 3",
          "[ComboManager]") {
  std::array<double, 5> arr = {1.1, 2.2, 3.3, 4.4, 5.5};
  // There are C(5,3) = 10 combinations.
  cse::ComboManager<std::array<double, 5>> cm(arr, 3);
  const std::size_t expectedTotal = 10;
  REQUIRE(cm.TotalCombinations() == expectedTotal);

  int count = 0;
  do {
    std::vector<double> combo = cm.GetCurrentCombo();
    INFO("Forward Combo " << count << ": " << (combo.empty() ? "(empty)" : ""));
    count++;
  } while (cm.NextCombo());
  REQUIRE(count == expectedTotal);
}

// Test 4: Combination size 0 (should yield one empty combination).
TEST_CASE("ComboManager: Combination size 0", "[ComboManager]") {
  std::vector<int> numbers = {1, 2, 3, 4};
  // There is exactly 1 combination when k == 0.
  cse::ComboManager<std::vector<int>> cm(numbers, 0);
  const std::size_t expectedTotal = 1;
  REQUIRE(cm.TotalCombinations() == expectedTotal);

  std::vector<int> combo = cm.GetCurrentCombo();
  INFO("Combo is empty: " << (combo.empty() ? "true" : "false"));
  REQUIRE(combo.empty());
  // No next combination should exist.
  REQUIRE_FALSE(cm.NextCombo());
}

// Test 5: Combination size equal to container size.
TEST_CASE("ComboManager: Combination size equal to container size",
          "[ComboManager]") {
  std::vector<char> letters = {'a', 'b', 'c'};
  // There is exactly 1 combination when k equals the container size.
  cse::ComboManager<std::vector<char>> cm(letters, letters.size());
  const std::size_t expectedTotal = 1;
  REQUIRE(cm.TotalCombinations() == expectedTotal);

  std::vector<char> combo = cm.GetCurrentCombo();
  INFO("Combo contents: ");
  REQUIRE(combo.size() == letters.size());
  for (size_t i = 0; i < letters.size(); ++i) {
    REQUIRE(combo[i] == letters[i]);
  }
  // No next combination should exist.
  REQUIRE_FALSE(cm.NextCombo());
}

// Test 6: Reset functionality
TEST_CASE("ComboManager: Reset functionality returns to first combination",
          "[ComboManager]") {
  std::vector<int> numbers = {10, 20, 30, 40};
  cse::ComboManager<std::vector<int>> cm(numbers, 2);

  // Advance through all combinations (we don't need to track the count).
  while (cm.NextCombo()) {
  }

  // After exhausting combinations, reset back.
  cm.Reset();
  std::vector<int> firstCombo = cm.GetCurrentCombo();
  REQUIRE(firstCombo.size() == 2);
  // The first combination should match the first two elements.
  REQUIRE(firstCombo[0] == numbers[0]);
  REQUIRE(firstCombo[1] == numbers[1]);
}

// Test 7: Current indices correctness
TEST_CASE("ComboManager: GetCurrentIndices returns increasing indices",
          "[ComboManager]") {
  std::vector<char> letters = {'x', 'y', 'z', 'w'};
  cse::ComboManager<std::vector<char>> cm(letters, 3);
  std::vector<std::size_t> indices = cm.GetCurrentIndices();

  // For the first combination, the indices should be {0, 1, 2}.
  REQUIRE(indices.size() == 3);
  REQUIRE(indices[0] == 0);
  REQUIRE(indices[1] == 1);
  REQUIRE(indices[2] == 2);

  // Move to next combination and ensure the indices remain strictly increasing.
  bool moved = cm.NextCombo();
  REQUIRE(moved);
  indices = cm.GetCurrentIndices();
  for (std::size_t i = 1; i < indices.size(); ++i) {
    INFO("Index " << i << " is " << indices[i] << ", previous index was "
                  << indices[i - 1]);
    REQUIRE(indices[i] > indices[i - 1]);
  }
}

// Test 8: Single element container
TEST_CASE("ComboManager: Single element container produces one combination",
          "[ComboManager]") {
  std::vector<int> single = {42};
  cse::ComboManager<std::vector<int>> cm(single, 1);
  const std::size_t expectedTotal = 1;
  REQUIRE(cm.TotalCombinations() == expectedTotal);

  std::vector<int> combo = cm.GetCurrentCombo();
  REQUIRE(combo.size() == 1);
  REQUIRE(combo[0] == 42);
  // No further combination should be available.
  REQUIRE_FALSE(cm.NextCombo());
}

// Test 9: Empty container with combination size 0
TEST_CASE("ComboManager: Empty container with combination size 0",
          "[ComboManager]") {
  std::vector<int> empty;
  // By convention, C(0, 0) is 1.
  cse::ComboManager<std::vector<int>> cm(empty, 0);
  REQUIRE(cm.TotalCombinations() == 1);

  std::vector<int> combo = cm.GetCurrentCombo();
  REQUIRE(combo.empty());
  REQUIRE_FALSE(cm.NextCombo());
}

// Test 10: Repeated calls to NextCombo after exhaustion
TEST_CASE(
    "ComboManager: Repeated calls to NextCombo after last combination always "
    "return false",
    "[ComboManager]") {
  std::vector<int> numbers = {0, 1, 2};
  cse::ComboManager<std::vector<int>> cm(numbers, 2);
  // Advance to the last combination.
  while (cm.NextCombo()) {
  }
  REQUIRE(cm.TotalCombinations() == 3);
  // After reaching the final combination, repeated calls return false.
  for (int i = 0; i < 5; i++) {
    REQUIRE_FALSE(cm.NextCombo());
  }
}

// Test 11: Repeated calls to PrevCombo on first combination always return
// false.
TEST_CASE(
    "ComboManager: Repeated calls to PrevCombo on first combination always "
    "return false",
    "[ComboManager]") {
  std::vector<int> numbers = {5, 6, 7, 8};
  cse::ComboManager<std::vector<int>> cm(numbers, 2);
  // Already at the first combination.
  for (int i = 0; i < 3; i++) {
    REQUIRE_FALSE(cm.PrevCombo());
  }
}

// Test 12: Alternate NextCombo and PrevCombo consistency
TEST_CASE(
    "ComboManager: Alternating NextCombo and PrevCombo yields consistent "
    "results",
    "[ComboManager]") {
  std::vector<int> numbers = {0, 1, 2, 3};
  cse::ComboManager<std::vector<int>> cm(numbers, 2);

  // Save the first combination.
  std::vector<int> first = cm.GetCurrentCombo();

  // Move to the next combination.
  bool moved = cm.NextCombo();
  REQUIRE(moved);
  std::vector<int> second = cm.GetCurrentCombo();
  // Ensure the two combinations are different.
  REQUIRE(first != second);

  // Move back to the first combination.
  moved = cm.PrevCombo();
  REQUIRE(moved);
  std::vector<int> firstAgain = cm.GetCurrentCombo();
  REQUIRE(first == firstAgain);
}

// Test 13: Lexicographical order of generated combinations
TEST_CASE("ComboManager: Generated combinations are in lexicographical order",
          "[ComboManager]") {
  std::vector<int> numbers = {0, 1, 2, 3};
  cse::ComboManager<std::vector<int>> cm(numbers, 2);
  std::vector<std::vector<int>> combinations;
  do {
    combinations.push_back(cm.GetCurrentCombo());
  } while (cm.NextCombo());

  // Compare each combination with the next to ensure lexicographical order.
  for (size_t i = 1; i < combinations.size(); i++) {
    INFO("Comparing combination " << i - 1 << " with combination " << i);
    REQUIRE(combinations[i - 1] < combinations[i]);
  }
}

// Test 14: Container with duplicate elements
TEST_CASE("ComboManager: Container with duplicate elements", "[ComboManager]") {
  std::vector<int> numbers = {1, 2, 2, 3};
  // With 4 items, choosing 2 gives C(4,2) = 6 index combinations,
  // even though some value combinations may be identical.
  cse::ComboManager<std::vector<int>> cm(numbers, 2);
  REQUIRE(cm.TotalCombinations() == 6);

  int count = 0;
  do {
    std::vector<int> combo = cm.GetCurrentCombo();
    INFO("Combination " << count
                        << " values: " << (combo.empty() ? "(empty)" : ""));
    REQUIRE(combo.size() == 2);
    count++;
  } while (cm.NextCombo());
  REQUIRE(count == 6);
}

// Test 15: Larger container test for TotalCombinations correctness
TEST_CASE("ComboManager: Larger container total combinations test",
          "[ComboManager]") {
  std::vector<int> numbers(10);
  for (int i = 0; i < 10; i++) {
    numbers[i] = i;
  }
  cse::ComboManager<std::vector<int>> cm(numbers, 3);
  // C(10,3) should equal 120.
  REQUIRE(cm.TotalCombinations() == 120);
}

/**
 * Advanced Test Cases

   - Iterators
   - Permutations
   - Repeating Elements
   - Required Elements
 */

/**
 * Iterators Tests
 */
TEST_CASE("ComboManager: Testing Basic Iterators", "[ComboManager]") {
  std::vector<int> testVector1 = {5, 4, 3, 2, 1};
  cse::ComboManager<std::vector<int>> manager(testVector1, 3);

  auto beginning = manager.begin();
  auto beforeBeginning = manager.rbegin();
  std::vector<int> firstCombo = std::vector<int>{5, 4, 3};
  // Require that the begin() operator points at what we expect it to
  REQUIRE(*beginning == firstCombo);
  // begin should point to the first element, rbegin should point to 'before'
  // that
  REQUIRE(beginning != beforeBeginning);

  auto end = manager.rend();
  auto afterEnd = manager.end();
  std::vector<int> lastCombo = std::vector<int>{3, 2, 1};
  REQUIRE(*end == lastCombo);
  // .rend() points to the last possible element, .end() is after the last
  // element
  REQUIRE(end != afterEnd);

  std::vector<std::vector<int>> correctCombos = {
      {5, 4, 3}, {5, 4, 2}, {5, 4, 1}, {5, 3, 2}, {5, 3, 1},
      {5, 2, 1}, {4, 3, 2}, {4, 3, 1}, {4, 2, 1}, {3, 2, 1}};
  std::size_t index = 0;
  for (auto iter = manager.begin(); iter != manager.end(); iter++) {
    REQUIRE(correctCombos.at(index++) == *iter);
  }
  REQUIRE(index == correctCombos.size());
}

TEST_CASE("ComboManager: Testing Range-Based For Loop", "[ComboManager]") {
  std::vector<int> testVector1 = {5, 4, 3, 2, 1};
  cse::ComboManager<std::vector<int>> manager(testVector1, 3);

  std::vector<std::vector<int>> correctCombos = {
      {5, 4, 3}, {5, 4, 2}, {5, 4, 1}, {5, 3, 2}, {5, 3, 1},
      {5, 2, 1}, {4, 3, 2}, {4, 3, 1}, {4, 2, 1}, {3, 2, 1}};
  std::size_t index = 0;
  for (auto val : manager) {
    REQUIRE(val == correctCombos.at(index++));
  }
  REQUIRE(index == correctCombos.size());
}

TEST_CASE("ComboManager: Testing a reverse For Loop", "[ComboManager]") {
  std::vector<int> testVector1 = {5, 4, 3, 2, 1};
  cse::ComboManager<std::vector<int>> manager(testVector1, 3);
  std::vector<std::vector<int>> correctCombos = {
      {3, 2, 1}, {4, 2, 1}, {4, 3, 1}, {4, 3, 2}, {5, 2, 1},
      {5, 3, 1}, {5, 3, 2}, {5, 4, 1}, {5, 4, 2}, {5, 4, 3}};
  std::size_t index = 0;
  for (auto iter = manager.rend(); iter != manager.rbegin(); --iter) {
    REQUIRE(correctCombos.at(index++) == *iter);
  }
  REQUIRE(index == correctCombos.size());
}

TEST_CASE("ComboManager: Testing Basic Required Element functionality",
          "[ComboManager]") {
  std::vector<int> testVector1 = {5, 4, 3, 2, 1};

  // Should Require 1 in every combination
  cse::ComboManager<std::vector<int>> manager(testVector1, 3, 4, false);
  std::vector<std::vector<int>> correctCombos = {
      {5, 4, 1}, {5, 3, 1}, {5, 2, 1}, {4, 3, 1}, {4, 2, 1}, {3, 2, 1}};

  std::size_t index = 0;
  for (auto combo : manager) {
    REQUIRE(correctCombos.at(index) == combo);
    index++;
  }
  REQUIRE(index == correctCombos.size());
}

TEST_CASE("ComboManager: Testing Various Required Elements", "[ComboManager]") {
  std::vector<int> testVector1 = {5, 4, 3, 3, 1};

  // Should Require 1 in every combination
  cse::ComboManager<std::vector<int>> manager(testVector1, 3, 2, false);
  std::vector<std::vector<int>> correctCombos = {
      {5, 4, 3}, {5, 3, 3}, {5, 1, 3}, {4, 3, 3}, {4, 1, 3}, {3, 1, 3}};

  std::size_t index = 0;
  for (auto combo : manager) {
    REQUIRE(correctCombos.at(index) == combo);
    index++;
  }
  REQUIRE(index == correctCombos.size());

  try {
    cse::ComboManager<std::vector<int>> manager2(testVector1, 3, 47, false);
  } catch (const std::invalid_argument& error) {
    // Invalid Argument should be called on the out_of_range required index
    CHECK(true);
  } catch(...) {
    // No other exceptions should be called for that construction
    CHECK(false);
  }

  try {
    cse::ComboManager<std::vector<int>> manager2(testVector1, 3, -1, false);
  } catch (const std::invalid_argument& error) {
    // Invalid Argument should be called on the out_of_range required index
    CHECK(true);
  } catch(...) {
    // No other exceptions should be called for that construction
    CHECK(false);
  }
}

TEST_CASE("ComboManager: Repeating Elements Without Required Element", "[ComboManager]") {
  std::vector<int> testVector = {1, 2, 3};
  // With repeats allowed and combination size 2, we should have C(3+2-1, 2) = C(4,2) = 6 combinations.
  // Expected combinations (non-decreasing order):
  // {1, 1}, {1, 2}, {1, 3}, {2, 2}, {2, 3}, {3, 3}
  cse::ComboManager<std::vector<int>> manager(testVector, 2, true);
  std::vector<std::vector<int>> correctCombos = {
      {1, 1}, {1, 2}, {1, 3}, {2, 2}, {2, 3}, {3, 3}
  };

  std::size_t index = 0;
  for (auto combo : manager) {
    REQUIRE(correctCombos.at(index) == combo);
    index++;
  }
  REQUIRE(index == correctCombos.size());
}

TEST_CASE("ComboManager: Repeating Elements With Required Element", "[ComboManager]") {
  std::vector<int> testVector = {10, 20, 30, 40};
  // Require the element at index 2 (value 30) in every combination.
  // For combination size 3, the required mode removes the required element and works with an effective size of 2.
  // With repeats allowed, the available elements become {10, 20, 40} and the 2-element combinations (non-decreasing) are:
  // {10, 10}, {10, 20}, {10, 40}, {20, 20}, {20, 40}, {40, 40}.
  // The required element 30 is appended to each combination.
  cse::ComboManager<std::vector<int>> manager(testVector, 3, 2, true);
  std::vector<std::vector<int>> correctCombos = {
      {10, 10, 30}, {10, 20, 30}, {10, 40, 30}, {20, 20, 30}, {20, 40, 30}, {40, 40, 30}
  };

  std::size_t index = 0;
  for (auto combo : manager) {
    REQUIRE(correctCombos.at(index) == combo);
    index++;
  }
  REQUIRE(index == correctCombos.size());
}

TEST_CASE("ComboManager: Binomial Coefficient for Repeating Elements", "[ComboManager]") {
  std::vector<int> numbers = {0, 1, 2, 3, 4}; // n = 5

  // Test 1: Combination size 3 with repeats allowed.
  // Expected total combinations = C(n+k-1, k) = C(5+3-1, 3) = C(7, 3) = 35.
  cse::ComboManager<std::vector<int>> cm1(numbers, 3, true);
  const std::size_t expectedTotal1 = 35;
  REQUIRE(cm1.TotalCombinations() == expectedTotal1);

  // Test 2: Combination size 2 with repeats allowed.
  // Expected total combinations = C(5+2-1, 2) = C(6, 2) = 15.
  cse::ComboManager<std::vector<int>> cm2(numbers, 2, true);
  const std::size_t expectedTotal2 = 15;
  REQUIRE(cm2.TotalCombinations() == expectedTotal2);
}
/**
 * Repeating determines whether individual elements can repeat within a
 * combination (determined by index) (AllowRepetition) Repeating count
 * determines how many times that individual element may repeat (max of the
 * length of combo) (MaxRepetitions)
 */
/*TEST_CASE("Combo Manager: Test Repeating Elements", "[ComboManager]") {
  bc = BinomialCoefficient(n = 5, k = 3, AllowRepetition = true,
                           MaxRepetitions = 2);
  expected = 7;
  REQUIRE(bc == expected);

  bc = BinomialCoefficient(n = 5, k = 2, AllowRepetition = false,
                           MaxRepetitions = None);
  expected = 10;
  REQUIRE(bc == expected);

  bc = BinomialCoefficient(n = 4, k = 3, AllowRepetition = true,
                           MaxRepetitions = None);
  expected = 20;
  REQUIRE(bc == expected);

  bc = BinomialCoefficient(n = 3, k = 3, AllowRepetition = true,
                           MaxRepetitions = 1);
  expected = 1;
  REQUIRE(bc == expected);
}*/