/**
 * @file CseStringTest.hpp
 * @brief Tests for cse::String class
 *
 * This file contains a series of tests to verify the functionality
 * of the cse::String class defined in CseString.hpp. It uses assert
 * statements and simple printed status messages to confirm expected behavior.
 */

#pragma once

#include <cassert>
#include <iostream>

#include "CseString.hpp" 

// Helper function to print test results
inline void print_test_result(const char* test_name, bool passed) {
  std::cout << test_name << ": " << (passed ? "PASSED" : "FAILED") << std::endl;
}

/**
 * @brief Test basic construction and assignment operations for cse::String.
 */
inline void test_basic_operations() {
  // Test default construction
  cse::String s1;
  assert(s1.empty());
  
  // Test construction from std::string
  std::string std_str = "Hello";
  cse::String s2(std_str);
  assert(s2.size() == 5);
  assert(s2 == "Hello");

  // Test assignment from std::string
  cse::String s3;
  s3 = std_str;
  assert(s3 == "Hello");

  // Test assignment from another cse::String
  cse::String s4;
  s4 = s2;  // uses cse::String &operator=(const std::string&)
  assert(s4 == "Hello");

  print_test_result("Basic operations", true);
}

/**
 * @brief Test safe character access using at() and operator[].
 *
 * Note: In debug mode, out-of-range access should trigger a dbg_assert failure.
 *       In release mode, out-of-range access with at() would throw std::out_of_range.
 */
inline void test_safe_access() {
  cse::String s("Test");

  // operator[] and at() return same character for valid indices
  assert(s[0] == 'T');
  assert(s.at(1) == 'e');
  assert(s.at(2) == 's');
  assert(s[3] == 't');

  // Attempting out-of-range access in debug mode will trigger dbg_assert.
  // The following lines are commented out to avoid halting the test suite.
  // s[4];       // Should fail in debug
  // s.at(10);   // Should throw std::out_of_range or trigger dbg_assert

  print_test_result("Safe character access", true);
}

/**
 * @brief Test substring extraction using substr().
 */
inline void test_substring_extraction() {
  cse::String s("Hello, World!");

  // Extract "Hello"
  cse::String sub1 = s.substr(0, 5);
  assert(sub1 == "Hello");

  // Extract ", World!"
  cse::String sub2 = s.substr(5, 8);
  assert(sub2 == ", World!");

  // Extract the whole string (count = npos)
  cse::String sub3 = s.substr(0);
  assert(sub3 == "Hello, World!");

  // Test out-of-range start in debug mode triggers dbg_assert
  // cse::String sub4 = s.substr(20); // Not valid; would fail in debug mode

  print_test_result("Substring extraction", true);
}

/**
 * @brief Test insertion functionality in cse::String.
 */
inline void test_insertion() {
  cse::String s("ABC");
  
  // Insert at beginning
  s.insert(0, "Start-");
  assert(s == "Start-ABC");

  // Insert in the middle
  s.insert(6, "MID-");
  assert(s == "Start-MID-ABC");

  // Insert at the end
  s.insert(s.size(), "-END");
  assert(s == "Start-MID-ABC-END");

  // Uncomment to test out-of-range behavior in debug mode:
  // s.insert(s.size() + 5, "OutOfRange"); // should trigger dbg_assert

  print_test_result("Insertion", true);
}

/**
 * @brief Test erasure functionality in cse::String.
 */
inline void test_erasure() {
  cse::String s("Hello World!");

  // Erase from the middle
  s.erase(5, 1); // remove the space
  assert(s == "HelloWorld!");

  // Erase a portion from the end
  s.erase(5, 5); // remove 'World'
  assert(s == "Hello!");

  // Erase everything
  s.erase();
  assert(s.empty());

  // Uncomment to test out-of-range behavior:
  // cse::String s2("Short");
  // s2.erase(10); // should trigger dbg_assert if pos > size()

  print_test_result("Erasure", true);
}

/**
 * @brief Test replacement functionality in cse::String.
 */
inline void test_replacement() {
  cse::String s("12345");
  
  // Replace the middle part
  s.replace(1, 3, "ABC"); // replaces '234' with 'ABC'
  assert(s == "1ABC5");

  // Replace at the end
  s.replace(4, 1, "XYZ"); // replaces '5' with 'XYZ'
  assert(s == "1ABCXYZ");

  // Replace entire string
  s.replace(0, s.size(), "NewString");
  assert(s == "NewString");

  // Uncomment to test out-of-range behavior:
  // s.replace(s.size() + 1, 2, "Oops"); // should trigger dbg_assert

  print_test_result("Replacement", true);
}

/**
 * @brief Additional tests for more advanced usage and boundary conditions.
 */
inline void test_advanced_operations() {
  {
    // Boundary checks using operator[] and at()
    cse::String s("Edge");
    // Accessing the last valid index
    assert(s[s.size() - 1] == 'e');
    assert(s.at(s.size() - 1) == 'e');

    // Uncomment to test boundary overrun:
    // s[s.size()];      // should trigger dbg_assert
    // s.at(s.size());   // should throw std::out_of_range or dbg_assert
  }

  {
    // Substring edge cases
    cse::String s("Hello");
    cse::String sub_end = s.substr(4, 1);  // last character
    assert(sub_end == "o");

    // Starting exactly at size() => yields empty substring
    cse::String sub_empty = s.substr(5);
    assert(sub_empty.empty());
  }

  print_test_result("Advanced operations", true);
}

/**
 * @brief Main entry point for cse::String tests.
 * @return 0 on success, non-zero otherwise.
 *
 * Call this function from your main program or test runner
 * to verify that all cse::String functionality works as expected.
 */
inline int cse_string_test() {
  std::cout << "Running cse::String tests...\n\n";

  test_basic_operations();
  test_safe_access();
  test_substring_extraction();
  test_insertion();
  test_erasure();
  test_replacement();
  test_advanced_operations();

  std::cout << "\nAll tests for cse::String completed!\n";
  return 0;
}
