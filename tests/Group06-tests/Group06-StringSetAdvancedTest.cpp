// These tests were generated with the assistance of GitHub Copilot

#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-06/StringSet/StringSet.cpp"

using namespace cse;
// GitHub copilot helped write some initial code for this function
TEST_CASE("Test for Search", "[Search]") {
    // Create a StringSet and insert some strings
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    // Perform a search with a pattern and check if the result matches the expected output
    auto result = set.Search("a?c*");
    std::vector<std::string> expected = {"abc", "aXcY", "aXcZ"};
    REQUIRE(result == expected);
}
// GitHub copilot helped write some initial code for this function
TEST_CASE("Test for Count_occurrence", "[Count_occurrence]") {
    // Create a StringSet and insert some strings
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    // Check the occurrence count of specific characters in the set
    REQUIRE(set.Count_occurrence("a") == 4);
    REQUIRE(set.Count_occurrence("X") == 2);
    REQUIRE(set.Count_occurrence("z") == 1);
}

TEST_CASE("Test for random_sample", "[random_sample]") {
    // Create a StringSet and insert some strings
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    // Get a random sample of 2 strings from the set and check if the sample size is correct
    auto samples = set.random_sample(2);
    REQUIRE(samples.size() == 2);
    // Ensure each sample exists in the set
    for (const auto& sample : samples) {
        REQUIRE(set.count(sample) == 1);
    }
}

TEST_CASE("Test for Statistic", "[Statistic]") {
    // Create a StringSet and insert some strings
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    // Get statistics from the set and check if they match the expected values
    auto stats = set.Statistic();
    REQUIRE(stats.num_strings == 5);
    REQUIRE(stats.avg_length == 4);
    REQUIRE(stats.longest_string == "abcd");
    REQUIRE(stats.shortest_string == "abc");
}