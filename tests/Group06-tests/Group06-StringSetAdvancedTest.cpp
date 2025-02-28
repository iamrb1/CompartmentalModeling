#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-06/StringSet/StringSet.cpp"

using namespace cse;

TEST_CASE("Test for Search", "[Search]") {
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    auto result = set.Search("a?c*");
    std::vector<std::string> expected = {"abc", "aXcY", "aXcZ"};
    REQUIRE(result == expected);
}

TEST_CASE("Test for Count_occurrence", "[Count_occurrence]") {
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    REQUIRE(set.Count_occurrence("a") == 4);
    REQUIRE(set.Count_occurrence("X") == 2);
    REQUIRE(set.Count_occurrence("z") == 1);
}

TEST_CASE("Test for random_sample", "[random_sample]") {
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    auto samples = set.random_sample(2);
    REQUIRE(samples.size() == 2);
    for (const auto& sample : samples) {
        REQUIRE(set.count(sample) == 1);
    }
}

TEST_CASE("Test for Statistic", "[Statistic]") {
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    auto stats = set.Statistic();
    REQUIRE(stats.num_strings == 5);
    REQUIRE(stats.avg_length == 4);
    REQUIRE(stats.longest_string == "abcd");
    REQUIRE(stats.shortest_string == "abc");
}