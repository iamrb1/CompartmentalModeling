// These tests were generated with the assistance of GitHub Copilot

#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <sstream>
#include <random>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-06/StringSet/StringSet.hpp"
#include "../../Group-06/StaticString/StaticString.hpp"

using namespace cse;

TEST_CASE("Test for is_subset", "[is_subset]") {
	StringSet set1, set2;
	
	SECTION("Empty test is a subset of empty set and itself") {
		REQUIRE(set1.is_subset(set2));
		REQUIRE(set1.is_subset(set1));
	}
	
	SECTION("Non-empty set is a subset of itself") {
		set1.insert("cat");
		set1.insert("dog");
		REQUIRE(set1.is_subset(set1));
	}
	
	SECTION("Non-empty set is a subset of another larger non-empty set") {
		set1.insert("cat");
		set2.insert("cat");
		set1.insert("dog");
		set2.insert("dog");
		
		set2.insert("ivan");
		set2.insert("monkey");
		
		REQUIRE(set1.is_subset(set2));
		
	}
	
	SECTION("Superset is not a subset") {
		set1.insert("cat");
		set2.insert("cat");
		set1.insert("dog");
		set2.insert("dog");
		
		set1.insert("monkey");
		
		REQUIRE_FALSE(set1.is_subset(set2));
	}


    StringSet<StaticString<10>> set3, set4;
    
    SECTION("Empty test is a subset of empty set and itself") {
        REQUIRE(set3.is_subset(set4));
        REQUIRE(set3.is_subset(set3));
    }
    
    SECTION("Non-empty set is a subset of itself") {
        set3.insert("cat");
        set3.insert("dog");
        REQUIRE(set3.is_subset(set3));
    }
    
    SECTION("Non-empty set is a subset of another larger non-empty set") {
        set3.insert("cat");
        set4.insert("cat");
        set3.insert("dog");
        set4.insert("dog");
        
        set4.insert("ivan");
        set4.insert("monkey");
        
        REQUIRE(set3.is_subset(set4));
        
    }
    
    SECTION("Superset is not a subset") {
        set3.insert("cat");
        set4.insert("cat");
        set3.insert("dog");
        set4.insert("dog");
        
        set3.insert("monkey");
        
        REQUIRE_FALSE(set3.is_subset(set4));
    }
}

TEST_CASE("Test for substring_filter", "[substring_filter]") {
	StringSet set;
	
    SECTION("Long repetetive words") {
        set.insert("abababababababababababababab");
        REQUIRE(set.size() == 1);
        set.substring_filter("ababababababab");
        REQUIRE(set.size() == 0);
    }
	SECTION("Filtering from an empty set should not cause errors") {
		set.substring_filter("something");
		REQUIRE(set.size() == 0);
	}
	
	SECTION("Filtering with non-occuring substring should not change the set") {
		set.insert("apple");
		set.insert("banana");
		set.insert("orange");
		set.substring_filter("something");
		
		REQUIRE(set.count("apple") == 1);
		REQUIRE(set.count("banana") == 1);
		REQUIRE(set.count("orange") == 1);
		REQUIRE(set.count("something") == 0);
	}
	
	SECTION("Using a substring that occurs in all elements") {
		set.insert("apple");
		set.insert("banana");
		set.insert("orange");
		set.substring_filter("a");
		REQUIRE(set.size() == 0);
	}
	
	SECTION("Using a substring that occurs in some elements") {
		set.insert("apple");
		set.insert("banana");
		set.insert("orange");
		set.insert("dog");
		REQUIRE(set.size() == 4);
		set.substring_filter("o");
		REQUIRE(set.size() == 2);
		REQUIRE(set.count("apple") == 1);
		REQUIRE(set.count("banana") == 1);
		REQUIRE(set.count("orange") == 0);
		REQUIRE(set.count("dog") == 0);
	}

    //StaticString
    StringSet<StaticString<15>> set2;
	
	SECTION("Filtering from an empty set should not cause errors") {
		set2.substring_filter("something");
		REQUIRE(set2.size() == 0);
	}
	
	SECTION("Filtering with non-occuring substring should not change the set") {
		set2.insert("apple");
		set2.insert("banana");
		set2.insert("orange");
		set2.substring_filter("something");
		
		REQUIRE(set2.count("apple") == 1);
		REQUIRE(set2.count("banana") == 1);
		REQUIRE(set2.count("orange") == 1);
		REQUIRE(set2.count("something") == 0);
	}
	
	SECTION("Using a substring that occurs in all elements") {
		set2.insert("apple");
		set2.insert("banana");
		set2.insert("orange");
		set2.substring_filter("a");
		REQUIRE(set2.size() == 0);
	}
	
	SECTION("Using a substring that occurs in some elements") {
		set2.insert("apple");
		set2.insert("banana");
		set2.insert("orange");
		set2.insert("dog");
		REQUIRE(set2.size() == 4);
		set2.substring_filter("o");
		REQUIRE(set2.size() == 2);
		REQUIRE(set2.count("apple") == 1);
		REQUIRE(set2.count("banana") == 1);
		REQUIRE(set2.count("orange") == 0);
		REQUIRE(set2.count("dog") == 0);
	}
}


// GitHub copilot helped write some initial code for this function
TEST_CASE("Test for search", "[search]") {
    // Test with std::string
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    SECTION("Long repetetive word") {
        set.insert("abababababababababababababababababaabababababababbababababababababab");
        StringSet mySet =  set.search("ababababababa?abababa*");
        REQUIRE(mySet.size() == 1);
    }

    // Perform a search with a pattern and check if the result matches the expected output
    StringSet result = set.search("a?c*");
    REQUIRE(result.size() == 3);
    REQUIRE(result.count("aXcY") == 1);
    REQUIRE(result.count("aXcZ") == 1);
    
    // Test with StaticString
    StringSet<StaticString<4>> staticSet;
    staticSet.insert("abc");
    staticSet.insert("abcd");
    staticSet.insert("aXcY");
    staticSet.insert("aXcZ");
    staticSet.insert("xyz");

    // Perform a search with a pattern and check if the result matches the expected output
    StringSet<StaticString<4>> staticResult = staticSet.search("a?c*");
    REQUIRE(staticResult.size() == 3);
    REQUIRE(staticResult.count("aXcY") == 1);
    REQUIRE(staticResult.count("aXcZ") == 1);
}
// GitHub copilot helped write some initial code for this function
TEST_CASE("Test for Count_occurrence", "[Count_occurrence]") {
    // Test with normal strings
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    // Check the occurrence count of specific characters in the set
    REQUIRE(set.count_occurrence("a") == 4);
    REQUIRE(set.count_occurrence("X") == 2);
    REQUIRE(set.count_occurrence("z") == 1);
    
    // Test with StaticString
    StringSet<StaticString<4>> staticSet;
    StaticString<4> str1("abc");
    StaticString<4> str2("abcd");
    StaticString<4> str3("aXcY");
    StaticString<4> str4("aXcZ");
    StaticString<4> str5("xyz");
    staticSet.insert(str1);
    staticSet.insert(str2);
    staticSet.insert(str3);
    staticSet.insert(str4);
    staticSet.insert(str5);

    // Check the occurrence count of specific characters in the set
    REQUIRE(staticSet.count_occurrence("a") == 4);
    REQUIRE(staticSet.count_occurrence("X") == 2);
    REQUIRE(staticSet.count_occurrence("z") == 1);
}

TEST_CASE("Test for random_sample", "[random_sample]") {
    // Test with normal strings
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

    // Get a random sample of 2 strings from the set with a filter and check if the sample size is correct
    auto filteredSamples = set.random_sample(2, [](const std::string& str) {
        return str.find('a') != std::string::npos;
    });
    REQUIRE(filteredSamples.size() == 2);
    // Ensure each sample exists in the set and matches the filter
    for (const auto& sample : filteredSamples) {
        REQUIRE(set.count(sample) == 1);
        REQUIRE(sample.find('a') != std::string::npos);
    }

    // Get a random sample of 3 strings from the set with a filter and check if the sample size is correct
    auto filteredSamples2 = set.random_sample(3, [](const std::string& str) {
        return str.find('X') != std::string::npos;
    });
    REQUIRE(filteredSamples2.size() == 2); // Only 2 strings match the filter
    // Ensure each sample exists in the set and matches the filter
    for (const auto& sample : filteredSamples2) {
        REQUIRE(set.count(sample) == 1);
        REQUIRE(sample.find('X') != std::string::npos);
    }

    // Get a random sample of 3 strings from the set without a filter and check if the sample size is correct
    auto samplesWithoutFilter = set.random_sample(3);
    REQUIRE(samplesWithoutFilter.size() == 3);
    // Ensure each sample exists in the set
    for (const auto& sample : samplesWithoutFilter) {
        REQUIRE(set.count(sample) == 1);
    }
    
    // Test with StaticString
    StringSet<StaticString<4>> staticSet;
    StaticString<4> str1("abc");
    StaticString<4> str2("abcd");
    StaticString<4> str3("aXcY");
    StaticString<4> str4("aXcZ");
    StaticString<4> str5("xyz");
    staticSet.insert(str1);
    staticSet.insert(str2);
    staticSet.insert(str3);
    staticSet.insert(str4);
    staticSet.insert(str5);

    // Get a random sample of 2 strings from the set and check if the sample size is correct
    auto staticSamples = staticSet.random_sample(2);
    REQUIRE(staticSamples.size() == 2);
    // Ensure each sample exists in the set
    for (const auto& sample : staticSamples) {
        REQUIRE(staticSet.count(sample) == 1);
    }

    // Get a random sample of 2 strings from the set with a filter and check if the sample size is correct
    auto staticFilteredSamples = staticSet.random_sample(2, [](const StaticString<4>& str) {
        return str.find('a') != cse::StaticString<4>::npos;
    });
    REQUIRE(staticFilteredSamples.size() == 2);
    // Ensure each sample exists in the set and matches the filter
    for (const auto& sample : staticFilteredSamples) {
        REQUIRE(staticSet.count(sample) == 1);
        REQUIRE(sample.find('a') != cse::StaticString<4>::npos);
    }

    // Get a random sample of 3 strings from the set with a filter and check if the sample size is correct
    auto staticFilteredSamples2 = staticSet.random_sample(3, [](const StaticString<4>& str) {
        return str.find('X') != cse::StaticString<4>::npos;
    });
    REQUIRE(staticFilteredSamples2.size() == 2); // Only 2 strings match the filter
    // Ensure each sample exists in the set and matches the filter
    for (const auto& sample : staticFilteredSamples2) {
        REQUIRE(staticSet.count(sample) == 1);
        REQUIRE(sample.find('X') != cse::StaticString<4>::npos);
    }

    // Get a random sample of 3 strings from the set without a filter and check if the sample size is correct
    auto staticSamplesWithoutFilter = staticSet.random_sample(3);
    REQUIRE(staticSamplesWithoutFilter.size() == 3);
    // Ensure each sample exists in the set
    for (const auto& sample : staticSamplesWithoutFilter) {
        REQUIRE(staticSet.count(sample) == 1);
    }
}

TEST_CASE("Test for Statistic", "[Statistic]") {
    // Test with std::string
    StringSet set;
    set.insert("abc");
    set.insert("abcd");
    set.insert("aXcY");
    set.insert("aXcZ");
    set.insert("xyz");

    // Get statistics from the set and check if they match the expected values
    auto stats = set.statistics();
    REQUIRE(std::get<0>(stats) == 5); // Number of strings
    REQUIRE(std::get<1>(stats) == Approx(3.6)); // Average length
    REQUIRE(std::get<2>(stats).size() == 4); // Longest string
    REQUIRE(std::get<3>(stats).size() == 3); // Shortest string

    // Test with StaticString
    StringSet<StaticString<4>> staticSet;
    staticSet.insert("abc");
    staticSet.insert("abcd");
    staticSet.insert("aXcY");
    staticSet.insert("aXcZ");
    staticSet.insert("xyz");

    // Get statistics from the set and check if they match the expected values
    auto staticStats = staticSet.statistics();
    REQUIRE(std::get<0>(staticStats) == 5); // Number of strings
    REQUIRE(std::get<1>(staticStats) == Approx(3.6)); // Average length
    REQUIRE(std::get<2>(staticStats).size() == 4); // Longest string
    REQUIRE(std::get<3>(staticStats).size() == 4); // Shortest string
}

TEST_CASE("Tests for size_filter", "[size_filter]") {
	StringSet set;
	
	SECTION("Filtering an empty set does not cause an error") {
		set.size_filter(100);
		REQUIRE(set.size() == 0);
	}
	
	SECTION("Filtering a set with same string lengths should not change it") {
		set.insert({"applee", "banana", "orange"});
		set.size_filter(6);
		
		REQUIRE(set.size() == 3);
	}
	
	SECTION("Filtering a set with multiple different elements") {
		set.insert({"apple", "banana", "orange", "ivan", "goose"});
		set.size_filter(5);
		
		REQUIRE(set.size() == 2);
		REQUIRE(set.count("apple") == 1);
		REQUIRE(set.count("goose") == 1);
	}

    // Test for StaticString
    StringSet<StaticString<10>> set2;
	
	SECTION("Filtering an empty set does not cause an error") {
		set2.size_filter(100);
		REQUIRE(set2.size() == 0);
	}
	
	SECTION("Filtering a set with same string lengths should not change it") {
		set2.insert({"applee", "banana", "orange"});
		set2.size_filter(6);
		
		REQUIRE(set2.size() == 3);
	}
	
	SECTION("Filtering a set with multiple different elements") {
		set2.insert({"apple", "banana", "orange", "ivan", "goose"});
		set2.size_filter(5);
		
		REQUIRE(set2.size() == 2);
		REQUIRE(set2.count("apple") == 1);
		REQUIRE(set2.count("goose") == 1);
	}
}


TEST_CASE("Test for union_with", "[union_with]"){
    StringSet set1;
    StringSet set2;

    // Not empty sets
    set1.insert("black");
    set1.insert("blue");
    set1.insert("brown");

    set2.insert("purple");
    set2.insert("white");
    set2.insert("yellow");

    StringSet unionSet = set1.union_with(set2, [](const std::string& str){
        return str.find('l') != std::string::npos;
    });
    REQUIRE(unionSet.size() == 4);
    REQUIRE(unionSet.count("black") == 1);
    REQUIRE(unionSet.count("blue") == 1);
    REQUIRE(unionSet.count("purple") == 1);
    REQUIRE(unionSet.count("yellow") == 1);

    // One empty set

    set2.clear();
    unionSet.clear();

    unionSet = set1.union_with(set2, [](const std::string& str){
        return str.find('l') != std::string::npos;
    });

    REQUIRE(unionSet.size() == 2);
    REQUIRE(unionSet.count("black") == 1);
    REQUIRE(unionSet.count("blue") == 1);

    // Two empty sets

    set1.clear();
    unionSet.clear();

    unionSet = set1.union_with(set2, [](const std::string& str){
        return str.find('l') != std::string::npos;
    });

    REQUIRE(unionSet.size() == 0);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset1;
    StringSet<StaticString<10>> Sset2;

    // Not empty sets
    Sset1.insert(StaticString<10>("black"));
    Sset1.insert(StaticString<10>("blue"));
    Sset1.insert(StaticString<10>("brown"));

    Sset2.insert(StaticString<10>("purple"));
    Sset2.insert(StaticString<10>("white"));
    Sset2.insert(StaticString<10>("yellow"));

    StringSet<StaticString<10>> SunionSet = Sset1.union_with(Sset2, [](const StaticString<10>& str){
        return str.find('l') != StaticString<10>::npos;
    });
    REQUIRE(SunionSet.size() == 4);
    REQUIRE(SunionSet.count(StaticString<10>("black")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("blue")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("purple")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("yellow")) == 1);

    // One empty set

    Sset2.clear();
    SunionSet.clear();

    SunionSet = Sset1.union_with(Sset2, [](const StaticString<10>& str){
        return str.find('l') != StaticString<10>::npos;
    });

    REQUIRE(SunionSet.size() == 2);
    REQUIRE(SunionSet.count(StaticString<10>("black")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("blue")) == 1);

    // Two empty sets

    Sset1.clear();
    SunionSet.clear();

    SunionSet = Sset1.union_with(Sset2, [](const StaticString<10>& str){
        return str.find('l') != StaticString<10>::npos;
    });

    REQUIRE(SunionSet.size() == 0);
}

TEST_CASE("Test for symmetric_difference", "[symmetric_difference]"){
    StringSet set1;
    StringSet set2;

    // Empty sets
    StringSet symmetricSet = set1.symmetric_difference(set2);

    REQUIRE(symmetricSet.size() == 0);

    // One empty set
    set1.insert("black");
    set1.insert("blue");
    set1.insert("green");

    symmetricSet = set1.symmetric_difference(set2);
    REQUIRE(symmetricSet.size() == 3);
    REQUIRE(symmetricSet.count("black") == 1);
    REQUIRE(symmetricSet.count("blue") == 1);
    REQUIRE(symmetricSet.count("green") == 1);

    // Not empty sets
    symmetricSet.clear();
    set2.insert("black");
    set2.insert("yellow");
    set2.insert("white");

    symmetricSet = set1.symmetric_difference(set2);
    REQUIRE(symmetricSet.size() == 4);
    REQUIRE(symmetricSet.count("blue") == 1);
    REQUIRE(symmetricSet.count("green") == 1);
    REQUIRE(symmetricSet.count("yellow") == 1);
    REQUIRE(symmetricSet.count("white") == 1);

    // Try same set
    symmetricSet.clear();
    symmetricSet = set1.symmetric_difference(set1);
    REQUIRE(symmetricSet.size() == 0);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset1;
    StringSet<StaticString<10>> Sset2;

    // Empty sets
    StringSet<StaticString<10>> SsymmetricSet = Sset1.symmetric_difference(Sset2);

    REQUIRE(SsymmetricSet.size() == 0);

    // One empty set
    Sset1.insert(StaticString<10>("black"));
    Sset1.insert(StaticString<10>("blue"));
    Sset1.insert(StaticString<10>("green"));

    SsymmetricSet = Sset1.symmetric_difference(Sset2);
    REQUIRE(SsymmetricSet.size() == 3);
    REQUIRE(SsymmetricSet.count(StaticString<10>("black")) == 1);
    REQUIRE(SsymmetricSet.count(StaticString<10>("blue")) == 1);
    REQUIRE(SsymmetricSet.count(StaticString<10>("green")) == 1);

    // Not empty sets
    SsymmetricSet.clear();
    Sset2.insert(StaticString<10>("black"));
    Sset2.insert(StaticString<10>("yellow"));
    Sset2.insert(StaticString<10>("white"));

    SsymmetricSet = Sset1.symmetric_difference(Sset2);
    REQUIRE(SsymmetricSet.size() == 4);
    REQUIRE(SsymmetricSet.count(StaticString<10>("blue")) == 1);
    REQUIRE(SsymmetricSet.count(StaticString<10>("green")) == 1);
    REQUIRE(SsymmetricSet.count(StaticString<10>("yellow")) == 1);
    REQUIRE(SsymmetricSet.count(StaticString<10>("white")) == 1);

    // Try same set
    SsymmetricSet.clear();
    SsymmetricSet = Sset1.symmetric_difference(Sset1);
    REQUIRE(SsymmetricSet.size() == 0);
}

TEST_CASE("Test for to_vector", "[to_vector]"){
    StringSet set1;

    set1.insert("black");
    set1.insert("blue");
    set1.insert("green");
    set1.insert("yellow");

    std::vector<std::string> vec1 = set1.to_vector();

    REQUIRE(vec1.size() == 4);

    // Empty set

    StringSet set2;

    std::vector<std::string> vec2 = set2.to_vector();
    REQUIRE(vec2.size() == 0);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset1;

    Sset1.insert(StaticString<10>("black"));
    Sset1.insert(StaticString<10>("blue"));
    Sset1.insert(StaticString<10>("green"));
    Sset1.insert(StaticString<10>("yellow"));

    std::vector<StaticString<10>> Svec1 = Sset1.to_vector();

    REQUIRE(Svec1.size() == 4);

    // Empty set

    StringSet<StaticString<10>> Sset2;

    std::vector<StaticString<10>> Svec2 = Sset2.to_vector();
    REQUIRE(Svec2.size() == 0);
}

TEST_CASE("Test for sort", "[sort]"){
    StringSet set;

    // Empty set

    std::set<std::string> sortedSet = set.sort([](const std::string& str1, const std::string& str2){
        return str1.size() < str2.size();
    });

    std::set<std::string> expected = {};

    REQUIRE(sortedSet == expected);

    set.insert("black");
    set.insert("red");
    set.insert("purple");

    sortedSet = set.sort([](const std::string& str1, const std::string& str2){
        return str1.size() < str2.size();
    });

    expected = {"red", "black", "purple"};

    REQUIRE(sortedSet == expected);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;

    // Empty set

    std::set<StaticString<10>> SsortedSet = Sset.sort([](const StaticString<10>& s1, const StaticString<10>& s2){
        return s1.length() < s2.length();
    });

    std::set<StaticString<10>> STexpected = {};

    REQUIRE(SsortedSet == STexpected);

    Sset.insert(StaticString<10>("black"));
    Sset.insert(StaticString<10>("red"));
    Sset.insert(StaticString<10>("purple"));

    SsortedSet = Sset.sort([](const StaticString<10>& s1, const StaticString<10>& s2){
        return s1.length() < s2.length();
    });

    STexpected = {StaticString<10>("red"), StaticString<10>("black"), StaticString<10>("purple")};

    REQUIRE(SsortedSet == STexpected);

}

TEST_CASE("operator<<", "[operator<<]"){
    StringSet set;
    set.insert("blue");
    set.insert("green");
    set.insert("black");

    std::ostringstream oss;
    oss << set;

    std::string output = oss.str();

    REQUIRE(output.find("blue") != std::string::npos);
    REQUIRE(output.find("green") != std::string::npos);
    REQUIRE(output.find("black") != std::string::npos);

    // empty set
    set.clear();
    std::ostringstream oss2;
    oss2 << set;

    REQUIRE(oss2.str() == "");

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;
    Sset.insert(StaticString<10>("blue"));
    Sset.insert(StaticString<10>("green"));
    Sset.insert(StaticString<10>("black"));

    std::ostringstream Soss;
    Soss << Sset;

    std::string Soutput = Soss.str();

    REQUIRE(Soutput.find("blue") != std::string::npos);
    REQUIRE(Soutput.find("green") != std::string::npos);
    REQUIRE(Soutput.find("black") != std::string::npos);

    // empty set
    Sset.clear();
    std::ostringstream Soss2;
    Soss2 << Sset;

    REQUIRE(Soss2.str() == "");
}

TEST_CASE("Swap", "[Swap]"){
    StringSet set1;
    StringSet set2;

    set1.insert("blue");
    set1.insert("green");
    set1.insert("black");
    REQUIRE(set1.count("blue") == 1);
    REQUIRE(set1.count("green") == 1);
    REQUIRE(set1.count("black") == 1);

    set2.insert("yellow");
    set2.insert("white");
    set2.insert("purple");
    REQUIRE(set2.count("yellow") == 1);
    REQUIRE(set2.count("white") == 1);
    REQUIRE(set2.count("purple") == 1);

    set1.Swap(set2);

    REQUIRE(set1.count("yellow") == 1);
    REQUIRE(set1.count("white") == 1);
    REQUIRE(set1.count("purple") == 1);
    REQUIRE(set1.count("blue") == 0);
    REQUIRE(set1.count("green") == 0);
    REQUIRE(set1.count("black") == 0);

    REQUIRE(set2.count("blue") == 1);
    REQUIRE(set2.count("green") == 1);
    REQUIRE(set2.count("black") == 1);
    REQUIRE(set2.count("yellow") == 0);
    REQUIRE(set2.count("white") == 0);
    REQUIRE(set2.count("purple") == 0);

    // empty set
    set1.clear();
    set2.clear();

    set1.Swap(set2);
    REQUIRE(set1.empty());
    REQUIRE(set2.empty());


    // TEST FOR STATICSTRING


    StringSet<StaticString<10>> Sset1;
    StringSet<StaticString<10>> Sset2;

    Sset1.insert(StaticString<10>("blue"));
    Sset1.insert(StaticString<10>("green"));
    Sset1.insert(StaticString<10>("black"));
    REQUIRE(Sset1.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("green")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("black")) == 1);

    Sset2.insert(StaticString<10>("yellow"));
    Sset2.insert(StaticString<10>("white"));
    Sset2.insert(StaticString<10>("purple"));
    REQUIRE(Sset2.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("white")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("purple")) == 1);

    Sset1.Swap(Sset2);

    REQUIRE(Sset1.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("white")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("purple")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("blue")) == 0);
    REQUIRE(Sset1.count(StaticString<10>("green")) == 0);
    REQUIRE(Sset1.count(StaticString<10>("black")) == 0);

    REQUIRE(Sset2.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("green")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("black")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("yellow")) == 0);
    REQUIRE(Sset2.count(StaticString<10>("white")) == 0);
    REQUIRE(Sset2.count(StaticString<10>("purple")) == 0);

    // empty set
    Sset1.clear();
    Sset2.clear();

    Sset1.Swap(Sset2);
    REQUIRE(Sset1.empty());
    REQUIRE(Sset2.empty());

}