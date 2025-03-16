/**
 * @file CseString.cpp
 * @brief Tests for cse::String class
 *
 * This file contains a series of tests to verify the functionality
 * of the cse::String class defined in CseString.hpp. It uses REQUIRE
 * statements and simple printed status messages to confirm expected behavior.
 */

#include "catch.hpp"

#define TEST_CSE_ASSERT
#include "CseAssert.hpp"
#include "CseString.hpp"

/**
 * @brief Test basic construction and assignment operations for cse::String.
 */
TEST_CASE("Test basic operations", "[csestring]") {
  // Test default construction
  cse::String s1;
  REQUIRE(s1.empty());

  // Test construction from std::string
  std::string std_str = "Hello";
  cse::String s2(std_str);
  REQUIRE(s2.size() == 5);
  REQUIRE(s2 == "Hello");

  // Test assignment from std::string
  cse::String s3;
  s3 = std_str;
  REQUIRE(s3 == "Hello");

  // Now test reassigning s3 to ensure it changes correctly
  s3 = "World";
  REQUIRE(s3 == "World");

  // Test assignment from another cse::String
  cse::String s4;
  s4 = s2;  // uses cse::String &operator=(const std::string&)
  REQUIRE(s4 == "Hello");
}

/**
 * @brief Test safe character access using at() and operator[].
 *
 * Note: In debug mode, out-of-range access should trigger a dbg_assert
 * failure. In release mode, out-of-range access with at() would throw
 * std::out_of_range.
 */
TEST_CASE("Test safe access", "[csestring]") {
  cse::String s("Test");

  // operator[] and at() return same character for valid indices
  REQUIRE(s[0] == 'T');
  REQUIRE(s.at(1) == 'e');
  REQUIRE(s.at(2) == 's');
  REQUIRE(s.at(3) == 't');

  // Attempting out-of-range access in debug mode will trigger dbg_assert
  // (or throw std::out_of_range in release).
  REQUIRE_ASSERT(s[4]);      // operator[] with index 4
  REQUIRE_ASSERT(s[10]);     // operator[] with index 10
  REQUIRE_ASSERT(s.at(4));   // at() with index 4
  REQUIRE_ASSERT(s.at(10));  // at() with index 10
}

/**
 * @brief Test substring extraction using substr().
 */
TEST_CASE("Test substring extraction", "[csestring]") {
  cse::String s("Hello, World!");

  // Extract "Hello"
  cse::String sub1 = s.substr(0, 5);
  REQUIRE(sub1 == "Hello");

  // Extract ", World!"
  cse::String sub2 = s.substr(5, 8);
  REQUIRE(sub2 == ", World!");

  // Extract the whole string (count = npos)
  cse::String sub3 = s.substr(0);
  REQUIRE(sub3 == "Hello, World!");

  // Test out-of-range start in debug mode triggers dbg_assert
  cse::String sub4;
  REQUIRE_ASSERT(sub4 = s.substr(20));
}

/**
 * @brief Test insertion functionality in cse::String.
 */
TEST_CASE("Test insertion", "[csestring]") {
  cse::String s("ABC");

  // Insert at beginning
  s.insert(0, "Start-");
  REQUIRE(s == "Start-ABC");

  // Insert in the middle
  s.insert(6, "MID-");
  REQUIRE(s == "Start-MID-ABC");

  // Insert at the end
  s.insert(s.size(), "-END");
  REQUIRE(s == "Start-MID-ABC-END");

  // Uncomment to test out-of-range behavior in debug mode:
  REQUIRE_ASSERT(
      s.insert(s.size() + 5, "OutOfRange"));  // should trigger dbg_assert
}

/**
 * @brief Test erasure functionality in cse::String.
 */
TEST_CASE("Test erasure", "[csestring]") {
  cse::String s("Hello World!");

  // Erase from the middle
  s.erase(5, 1);  // remove the space
  REQUIRE(s == "HelloWorld!");

  // Erase a portion from the end
  s.erase(5, 5);  // remove 'World'
  REQUIRE(s == "Hello!");

  // Erase everything
  s.erase();
  REQUIRE(s.empty());

  // Uncomment to test out-of-range behavior:
  cse::String s2("Short");
  REQUIRE_ASSERT(s2.erase(10));  // should trigger dbg_assert if pos > size()
}

/**
 * @brief Test replacement functionality in cse::String.
 */
TEST_CASE("Test replacement", "[csestring]") {
  cse::String s("12345");

  // Replace the middle part
  s.replace(1, 3, "ABC");  // replaces '234' with 'ABC'
  REQUIRE(s == "1ABC5");

  // Replace at the end
  s.replace(4, 1, "XYZ");  // replaces '5' with 'XYZ'
  REQUIRE(s == "1ABCXYZ");

  // Replace entire string
  s.replace(0, s.size(), "NewString");
  REQUIRE(s == "NewString");

  // Uncomment to test out-of-range behavior:
  REQUIRE_ASSERT(
      s.replace(s.size() + 1, 2, "Oops"));  // should trigger dbg_assert
}

/**
 * @brief Additional tests for more advanced usage and boundary conditions.
 */
TEST_CASE("Test advanced operations", "[csestring]") {
  {
    // Boundary checks using operator[] and at()
    cse::String s("Edge");
    // Accessing the last valid index
    REQUIRE(s[s.size() - 1] == 'e');
    REQUIRE(s.at(s.size() - 1) == 'e');

    // Uncomment to test boundary overrun:
    REQUIRE_ASSERT(s[s.size()]);  // should trigger dbg_assert
    REQUIRE_ASSERT(
        s.at(s.size()));  // should throw std::out_of_range or dbg_assert
  }

  {
    // Substring edge cases
    cse::String s("Hello");
    cse::String sub_end = s.substr(4, 1);  // last character
    REQUIRE(sub_end == "o");

    // Starting exactly at size() => yields empty substring
    cse::String sub_empty = s.substr(5);
    REQUIRE(sub_empty.empty());
  }
}

/**
 * @brief Test removing the first occurrence of a substring.
 */
TEST_CASE("Test remove first instance of substring", "[csestring]") {
  cse::String s("Hello world");

  // 1) Find the start of "world"
  auto pos = s.find("world");
  REQUIRE(pos != cse::String::npos);  // Should find substring at index 6

  // 2) Erase the substring "world"
  s.erase(pos, 5);

  // 3) Also remove the preceding space if it exists
  if (pos > 0 && s[pos - 1] == ' ') {
    s.erase(pos - 1, 1);
  }

  REQUIRE(s == "Hello");
}

/**
 * @brief Test a combination of multiple operations in a single flow.
 */
TEST_CASE("Test combined usage", "[csestring]") {
  // 1. Construct and verify
  cse::String s("abcXYZ123");
  REQUIRE(s.size() == 9);

  // 2. Insert into the middle
  s.insert(3, "-MID-");  // "abc-MID-XYZ123"
  REQUIRE(s == "abc-MID-XYZ123");

  // 3. Erase a portion
  s.erase(3, 5);  // Remove "-MID-", results in "abcXYZ123"
  REQUIRE(s == "abcXYZ123");

  // 4. Replace some characters
  s.replace(3, 3, "HELLO");  // "abcHELLO123"
  REQUIRE(s == "abcHELLO123");

  // 5. Substring extraction
  auto sub = s.substr(3, 5);  // "HELLO"
  REQUIRE(sub == "HELLO");

  // 6. Safe access boundary check
  REQUIRE(s[s.size() - 1] == '3');
  // next line ensures we do not cause a test suite halt, but demonstrates
  // boundary logic
  REQUIRE_ASSERT(s[s.size()]);  // out-of-range in debug, "should fail"

  // 7. Remove "HELLO" by find+erase to finalize
  auto pos = s.find("HELLO");
  REQUIRE(pos != cse::String::npos);
  s.erase(pos, 5);  // remove the "HELLO"
  REQUIRE(s == "abc123");
}
