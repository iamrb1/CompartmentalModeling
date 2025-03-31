#include <iostream>
#include <numeric>

#include "../../Group-10/Classes/BruteForceOptimizer.hpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

/// Tests for BruteForceOptimizer functionality
TEST_CASE("BruteForceOptimizer basic tests", "[BruteForceOptimizer]") {
  SECTION("No items") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetCapacity(10.0);
    optimizer.SetItems({});
    auto result = optimizer.FindOptimalSolution();
    REQUIRE(result.second.empty());
    REQUIRE(result.first == 0.0);
  }

  SECTION("Single item, fits capacity") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetItems({{"Item1", 5.0, 5.0}});
    optimizer.SetCapacity(10.0);
    auto result = optimizer.FindOptimalSolution();
    REQUIRE(result.first == 5.0);
    REQUIRE(result.second.size() == 1);
    REQUIRE(result.second[0].weight == 5.0);
    REQUIRE(result.second[0].value == 5.0);
  }

  SECTION("Single item, does not fit capacity") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetItems({{"Item1", 12.0, 12.0}});
    optimizer.SetCapacity(10.0);
    auto result = optimizer.FindOptimalSolution();
    REQUIRE(result.second.empty());
    REQUIRE(result.first == 0.0);
  }

  SECTION("Multiple items, easy example") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetItems({{"A", 1.0, 1.0},
                        {"B", 2.0, 2.0},
                        {"C", 3.0, 3.0},
                        {"D", 4.0, 4.0},
                        {"E", 5.0, 5.0}});
    optimizer.SetCapacity(5.0);
    auto result = optimizer.FindOptimalSolution();
    REQUIRE(result.first == 5.0);
    double sumWeight = 0.0;
    for (auto const &item : result.second) sumWeight += item.weight;
    REQUIRE(sumWeight == 5.0);
  }

  SECTION("Multiple items, capacity < smallest item") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetItems({{"A", 3.0, 3.0}, {"B", 4.0, 4.0}, {"C", 5.0, 5.0}});
    optimizer.SetCapacity(2.0);
    auto result = optimizer.FindOptimalSolution();
    REQUIRE(result.second.empty());
    REQUIRE(result.first == 0.0);
  }

  SECTION("Mixed example, capacity=6") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetItems(
        {{"A", 2.0, 2.0}, {"B", 2.0, 2.0}, {"C", 6.0, 6.0}, {"D", 3.0, 3.0}});
    optimizer.SetCapacity(6.0);
    auto result = optimizer.FindOptimalSolution();
    REQUIRE(result.first == 6.0);
    double sumWeight = 0.0;
    for (auto const &item : result.second) sumWeight += item.weight;
    REQUIRE(sumWeight == 6.0);
  }

  SECTION("Mixed example, selecting specific items (Gold and Silver)") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetItems(
        {{"Silver", 2.0, 10.0}, {"Gold", 5.0, 20.0}, {"Copper", 2.0, 5.0}});
    optimizer.SetCapacity(7.0);
    auto result = optimizer.FindOptimalSolution();
    // Best solution should be Silver + Gold: weight = 7.0, value = 30.0
    REQUIRE(result.first == 30.0);
    REQUIRE(result.second.size() == 2);

    bool foundSilver = false, foundGold = false;
    for (auto const &item : result.second) {
      if (item.name == "Silver") foundSilver = true;
      if (item.name == "Gold") foundGold = true;
    }
    REQUIRE(foundSilver);
    REQUIRE(foundGold);
  }
}

/**
 * Advanced Test Section
 */

// Optimization sections

// Utility function to measure execution time
template <typename Func>
double measureTime(Func &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>(end - start).count();
}

/**
 * Current Optimization:
 * Focus around bounding based on item weights, halting individual branches once
 * they become infeasible compared to the capacity
 */
TEST_CASE("BruteForceOptimizer: Optimization Settings",
          "[BruteForceOptimizer]") {
  cse::BruteForceOptimizer optimizer;

  std::vector<cse::Item> initialItems = {
      {"A", 2.0, 2.0}, {"B", 2.0, 2.0}, {"C", 6.0, 6.0}, {"D", 3.0, 3.0},
      {"E", 2.2, 4.6}, {"F", 1.0, 1.0}, {"G", 1.0, 2.1}, {"H", 5.5, 6.5},
      {"I", 5.9, 6.0}, {"J", 0.1, 0.4}, {"K", 1.0, 1.1}, {"L", 1.3, 4.0},
      {"M", 3.3, 3.0}};
  optimizer.SetItems(initialItems);
  optimizer.SetCapacity(9.0);

  double unoptimizedTime =
      measureTime([&]() { optimizer.FindOptimalSolution(); });

  auto unoptimizedScore = optimizer.FindOptimalSolution().first;

  optimizer.setOptimizer(true);

  double optimizedTime =
      measureTime([&]() { optimizer.FindOptimalSolution(); });

  auto optimizedScore = optimizer.FindOptimalSolution().first;
  std::cout << "\n13 Item Test:\n";
  std::cout << "Unoptimized Time: " << unoptimizedTime << std::endl;
  std::cout << "Optimized Time: " << optimizedTime << std::endl;
  std::cout << "Speedup: "
            << ((unoptimizedTime - optimizedTime) / unoptimizedTime) * 100
            << "%\n";
  REQUIRE(unoptimizedTime > optimizedTime);
  REQUIRE(unoptimizedScore == optimizedScore);

  cse::BruteForceOptimizer optimizer2;

  optimizer2.SetItems(
      {{"A", 2.0, 2.0}, {"B", 2.0, 2.0}, {"C", 6.0, 6.0}, {"D", 3.0, 3.0},
       {"E", 2.2, 4.6}, {"F", 1.0, 1.0}, {"G", 1.0, 2.1}, {"H", 5.5, 6.5},
       {"I", 5.9, 6.0}, {"J", 0.1, 0.4}, {"K", 1.0, 1.1}, {"L", 1.3, 4.0},
       {"M", 3.3, 3.0}, {"N", 3.0, 3.0}, {"O", 5.2, 3.6}, {"P", 1.4, 5.0},
       {"Q", 2.7, 2.1}, {"R", 1.5, 4.5}, {"S", 9.9, 6.2}, {"T", 0.3, 1.4},
       {"U", 1.8, 1.1}, {"V", 1.3, 2.0}, {"W", 3.9, 3.1}, {"X", 1.9, 4.0},
       {"Y", 1.0, 2.1}, {"Z", 6.5, 3.5}});
  optimizer2.SetCapacity(9.0);

  double unoptimizedTime2 =
      measureTime([&]() { optimizer2.FindOptimalSolution(); });

  auto unoptimizedScore2 = optimizer2.FindOptimalSolution().first;

  optimizer2.setOptimizer(true);

  double optimizedTime2 =
      measureTime([&]() { optimizer2.FindOptimalSolution(); });

  auto optimizedScore2 = optimizer2.FindOptimalSolution().first;
  std::cout << "\n26 Item Test:\n";
  std::cout << "Unoptimized Time: " << unoptimizedTime2 << std::endl;
  std::cout << "Optimized Time: " << optimizedTime2 << std::endl;
  std::cout << "Speedup: "
            << ((unoptimizedTime2 - optimizedTime2) / unoptimizedTime2) * 100
            << "%\n";
  REQUIRE(unoptimizedTime2 > optimizedTime2);
  REQUIRE(unoptimizedScore2 == optimizedScore2);
}

/**
 * This is similar, but tests with different capacities
 */
TEST_CASE("BruteForceOptimizer: Optimization Settings, higher capacities",
  "[BruteForceOptimizer]") {
  cse::BruteForceOptimizer optimizer;
  
  std::vector<cse::Item> initialItems = {
    {"A", 2.0, 2.0}, {"B", 2.0, 2.0}, {"C", 6.0, 6.0}, {"D", 3.0, 3.0},
    {"E", 2.2, 4.6}, {"F", 1.0, 1.0}, {"G", 11.0, 2.1}, {"H", 5.5, 6.5},
    {"I", 5.9, 6.0}, {"J", 0.1, 0.4}, {"K", 1.0, 1.1}, {"L", 1.3, 4.0},
    {"M", 3.3, 3.0}, {"O", 0.3, 0.4}, {"P", 9.3, 1.2}, {"Q", 3.3, 7.9}};
  optimizer.SetItems(initialItems);
  optimizer.SetCapacity(30.0);

  double unoptimizedTime =
  measureTime([&]() { optimizer.FindOptimalSolution(); });

  auto unoptimizedScore = optimizer.FindOptimalSolution().first;

  optimizer.setOptimizer(true);

  double optimizedTime =
  measureTime([&]() { optimizer.FindOptimalSolution(); });

  auto optimizedScore = optimizer.FindOptimalSolution().first;
  std::cout << "\n16 Item Test, 30.0 Capacity:\n";
  std::cout << "Unoptimized Time: " << unoptimizedTime << std::endl;
  std::cout << "Optimized Time: " << optimizedTime << std::endl;
  std::cout << "Speedup: "
      << ((unoptimizedTime - optimizedTime) / unoptimizedTime) * 100
      << "%\n";

  REQUIRE(unoptimizedTime > optimizedTime);
  REQUIRE(unoptimizedScore == optimizedScore);

  cse::BruteForceOptimizer optimizer2;
  initialItems = {
    {"A", 2.0, 2.0}, {"B", 2.0, 2.0}, {"C", 6.0, 6.0}, {"D", 3.0, 3.0},
    {"E", 2.2, 4.6}, {"F", 1.0, 1.0}, {"G", 11.0, 2.1}, {"H", 5.5, 6.5},
    {"I", 5.9, 6.0}, {"J", 0.1, 0.4}, {"K", 1.0, 1.1}, {"L", 1.3, 4.0},
    {"M", 3.3, 3.0}, {"O", 0.3, 0.4}, {"P", 9.3, 1.2}, {"Q", 3.3, 7.9},
    {"R", 5.5, 19.0}, {"S", 2.1, 3.4}, {"T", 9.0, 9.4}, {"U", 4.4, 8.7},
    {"V", 3.5, 4.0}, {"W", 12.2, 15.0}, {"X", 1.1, 0.2}, {"Y", 10.0, 11.0},
    {"Z", 7.7, 7.9}};
  optimizer2.SetItems(initialItems);
  optimizer2.SetCapacity(50.0);
  unoptimizedTime = measureTime([&]() { 
    unoptimizedScore = optimizer2.FindOptimalSolution().first;
  });

  optimizer2.setOptimizer(true);

  optimizedTime = measureTime([&]() { 
    optimizedScore = optimizer2.FindOptimalSolution().first;
  });

  std::cout << "\n26 Item Test, 50.0 Capacity:\n";
  std::cout << "Unoptimized Time: " << unoptimizedTime << std::endl;
  std::cout << "Optimized Time: " << optimizedTime << std::endl;
  std::cout << "Speedup: "
      << ((unoptimizedTime - optimizedTime) / unoptimizedTime) * 100
      << "%\n";

  REQUIRE(unoptimizedTime > optimizedTime);
  REQUIRE(unoptimizedScore == optimizedScore);
}