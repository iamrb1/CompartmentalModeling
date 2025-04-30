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

TEST_CASE("BruteForceOptimizer: Optimized and unoptimized return the same value with new items",
  "[BruteForceOptimizer]") 
{
    cse::BruteForceOptimizer optimizer;

    std::vector<cse::Item> initialItems = {
        {"Alpha", 1.0, 2.0}, {"Beta", 3.0, 4.0}, {"Gamma", 4.0, 5.0},
        {"Delta", 2.5, 3.5}, {"Epsilon", 1.5, 2.5}, {"Zeta", 2.0, 3.0},
        {"Eta", 5.0, 8.0}, {"Theta", 0.5, 1.5}
    };
    double BestScore = 12.0;

    optimizer.SetItems(initialItems);
    optimizer.SetCapacity(7.0);

    auto unoptimizedScore = optimizer.FindOptimalSolution().first;

    optimizer.SetOptimizer(true);

    auto optimizedScore = optimizer.FindOptimalSolution().first;

    REQUIRE(unoptimizedScore == Approx(optimizedScore));

    REQUIRE(unoptimizedScore == Approx(BestScore)); 
    REQUIRE(optimizedScore == Approx(BestScore));
}

TEST_CASE("BruteForceOptimizer: Optimized and unoptimized return same value with a larger new item set",
  "[BruteForceOptimizer]") 
{
    cse::BruteForceOptimizer optimizer2;

    std::vector<cse::Item> moreItems = {
        {"Alpha", 1.0, 2.0}, {"Beta", 3.0, 4.0}, {"Gamma", 4.0, 5.0},
        {"Delta", 2.5, 3.5}, {"Epsilon", 1.5, 2.5}, {"Zeta", 2.0, 3.0},
        {"Eta", 5.0, 8.0}, {"Theta", 0.5, 1.5}, {"Iota", 3.2, 3.6},
        {"Kappa", 1.1, 2.2}, {"Lambda", 2.8, 3.9}, {"Mu", 4.3, 5.5},
        {"Nu", 1.7, 2.7}, {"Xi", 3.5, 5.0}
    };

    double BestScore = 15.1;

    optimizer2.SetItems(moreItems);
    optimizer2.SetCapacity(9.0);

    auto unoptimizedScore2 = optimizer2.FindOptimalSolution().first;

    optimizer2.SetOptimizer(true);

    auto optimizedScore2 = optimizer2.FindOptimalSolution().first;
    REQUIRE(optimizedScore2 == Approx(unoptimizedScore2));
    REQUIRE(optimizedScore2 == Approx(BestScore));
    REQUIRE(unoptimizedScore2 == Approx(BestScore));


}