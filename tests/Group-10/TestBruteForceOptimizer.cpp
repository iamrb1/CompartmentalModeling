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

TEST_CASE("BruteForceOptimizer: item whose weight exceeds its value",
  "[BruteForceOptimizer]") 
//Never selecting an item whose weight exceeds its value when a strictly better alternative exists.
//Selecting that same inefficient item only when it’s the sole item that fits.
{
  SECTION("Inefficient item not chosen when a better fit exists") {
    // Here the 'Efficient' item (weight=4,val=8) should be picked over the
    // 'Inefficient' item (weight=5,val=1), even though both individually fit.
    cse::BruteForceOptimizer opt;
    opt.SetItems({{"Efficient", 4.0,  8.0},
                  {"Inefficient", 5.0, 1.0}});
    opt.SetCapacity(6.0);

    auto result = opt.FindOptimalSolution();
    REQUIRE(result.first == 8.0);
    REQUIRE(result.second.size() == 1);
    REQUIRE(result.second[0].name == "Efficient");
  }

  SECTION("Inefficient item chosen when it's the only option") {
    // Now only the 'Inefficient' item fits under capacity, so it must be selected.
    cse::BruteForceOptimizer opt;
    opt.SetItems({{"TooHeavy",     6.0, 12.0},
                  {"Inefficient",  5.0,  1.0}});
    opt.SetCapacity(5.0);

    auto result = opt.FindOptimalSolution();
    REQUIRE(result.first == 1.0); 
    REQUIRE(result.second.size() == 1);
    REQUIRE(result.second[0].name == "Inefficient");
  }
}

TEST_CASE("BruteForceOptimizer: Item constructor rejects negative or zero weights", "[BruteForceOptimizer]"){
  SECTION("Negative Weight"){
    REQUIRE_THROWS_AS(cse::Item("NegativeWeight", -1.0, 5.0), std::invalid_argument);
  }
  SECTION("Negative Value"){
    REQUIRE_THROWS_AS(cse::Item("NegativeWeight", 1.0, -5.0), std::invalid_argument);
  }
  SECTION("Both Negative"){
    REQUIRE_THROWS_AS(cse::Item("NegativeWeight", -1.0, -5.0), std::invalid_argument);
  }
  SECTION("Zero Weight"){
    REQUIRE_THROWS_AS(cse::Item("NegativeWeight", 0.0, 5.0), std::invalid_argument);
  }
  SECTION("Zero Value"){
    REQUIRE_THROWS_AS(cse::Item("NegativeWeight", 1.0, 0.0), std::invalid_argument);
  }
  SECTION("Both Zero"){
    REQUIRE_THROWS_AS(cse::Item("NegativeWeight", 0.0, 0.0), std::invalid_argument);
  }
}

TEST_CASE("Varied weight/value combinations", "[BruteForceOptimizer]") {
  
  SECTION("Random weights and values") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetItems({
      {"A",    1.5,  2.0},
      {"B",    2.2,  3.3},
      {"C",    3.7,  4.1},
      {"D",    4.4,  5.5},
      {"E",    0.9,  1.2},
      {"F",    2.5,  2.8},
      {"G",    3.3,  3.9},
      {"H",    1.1,  1.7},
      {"I",    5.2,  6.0},
      {"J",    4.8,  5.0},
      {"K",    2.9,  3.5},
      {"L",    1.7,  2.2},
      {"M",    3.1,  3.0},
      {"N",    4.2,  5.1},
      {"O",    0.6,  0.9},
      {"P",    3.8,  4.4},
      {"Q",    2.1,  2.7},
      {"R",    4.9,  5.2},
      {"S",    1.3,  1.8},
      {"T",    0.4,  0.6}
    });
    optimizer.SetCapacity(10.0);
    auto result = optimizer.FindOptimalSolution();
    REQUIRE(result.first == 13.8);

    double sumW = 0.0;
    for (auto &it : result.second) sumW += it.weight;
    REQUIRE(sumW <= Approx(10.0).margin(1e-6));
  }

  SECTION("High-value decoys vs many smalls") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetItems({
      {"Decoy1", 12.0, 15.0},
      {"Decoy2",  9.9,  9.9},
      {"Tiny1",   0.5,  0.8},
      {"Tiny2",   0.4,  0.6},
      {"X1",      2.0,  3.0},
      {"X2",      2.0,  3.0},
      {"X3",      2.0,  3.0},
      {"X4",      2.0,  3.0},
      {"Mini",    1.0,  1.5},
      {"Small",   1.5,  2.2},
      {"Med",     3.0,  4.0}
    });
    optimizer.SetCapacity(8.0);

    // Expect to pick the many smalls rather than decoys
    auto result = optimizer.FindOptimalSolution();
    REQUIRE(result.first == 12.0);

    double sumW = 0.0;
    for (auto &it : result.second) sumW += it.weight;
    REQUIRE(sumW <= 8.0);

    REQUIRE(result.second.size() > 1);
  }

  SECTION("Mix of tiny and huge items") {
    cse::BruteForceOptimizer optimizer;
    optimizer.SetItems({
      {"Tiny",       0.1,  0.05},
      {"Mini",       0.3,  0.25},
      {"Small",      1.0,  0.80},
      {"Medium",     5.0,  4.00},
      {"Moderate",   3.0,  3.50},
      {"Large",      7.5,  7.0},
      {"Huge",      20.0, 25.0},
      {"XL",        10.0, 12.0},
      {"MidTiny",    0.7,  0.9},
      {"SemiHuge",  12.0, 14.0}
    });
    optimizer.SetCapacity(15.0);

    auto result = optimizer.FindOptimalSolution();
    REQUIRE(result.first == 17.5);

    double sumW = 0.0;
    for (auto &it : result.second) sumW += it.weight;
    REQUIRE(sumW <= 15.0);
  }

}