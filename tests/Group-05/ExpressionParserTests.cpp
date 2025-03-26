#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../Group-05/src/ExpressionParser.cpp"
#include "../../Group-05/src/ExpressionParser.h"
#include <map>
#include <functional>

TEST_CASE("Parser Evaluations", "[parser]") {
  cse::ExpressionParser parser;
  parser.SetSymbolTable("val1", -2);
  parser.SetSymbolTable("val2", 2);
  parser.SetSymbolTable("val3", -15);
  parser.SetSymbolTable("val4", 15);
  parser.SetSymbolTable("val5", 0);
  parser.SetSymbolTable("val6", 30);
  parser.SetSymbolTable("val7", 1);
 
  static size_t default_index = 0;
  // Addition tests
  REQUIRE(parser.Evaluate("{val1} + {val2}", 0, default_index) == 0);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val3} + {val3}", 0, default_index) == -30);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val4} + {val4} + {val3}", 0, default_index) == 15);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val4} + {val4} * {val2}", 0, default_index) == 45);
  default_index = 0;
  // Subtraction tests
  REQUIRE(parser.Evaluate("{val1} - {val5}", 0, default_index) == -2);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val4} - {val3}", 0, default_index) == 30);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val3} - {val4}", 0, default_index) == -30);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val6} - {val4}", 0, default_index) == 15);
  default_index = 0;

  // Multiplication tests
  REQUIRE(parser.Evaluate("{val1} * {val4}", 0, default_index) == -30);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val1} * {val1}", 0, default_index) == 4);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val2} * {val2}", 0, default_index) == 4);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val1} * {val2} + {val4}", 0, default_index) == 11);
  default_index = 0;

  // Division tests
  REQUIRE(parser.Evaluate("{val5} / {val2}", 0, default_index) == 0);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val1} / {val1}", 0, default_index) == 1);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val2} / {val2}", 0, default_index) == 1);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val1} / {val2}", 0, default_index) == -1);
  default_index = 0;
  REQUIRE(parser.Evaluate("{val7} / {val2}", 0, default_index) == 0.5);
  default_index = 0;
  
  }

  /**
 * @brief Tests the compile-time addition using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprAdd", "[parser][constexpr]") {
  // This test will validate that the constexprAdd template function computes the sum at compile time.
  constexpr double result = cse::ExpressionParser::constexprAdd(2.0, 3.0);
  REQUIRE(result == 5.0);
}

  /**
 * @brief Tests the compile-time subtraction using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprSubtract", "[parser][constexpr]") {
  // This test will validate that the constexprSubtract template function computes the difference at compile time.
  constexpr double result1 = cse::ExpressionParser::constexprSubtract(8.0, 3.0);
  REQUIRE(result1 == 5.0);
  constexpr double result2 = cse::ExpressionParser::constexprSubtract(2.0, 3.0);
  REQUIRE(result2 == -1.0);
}

  /**
 * @brief Tests the compile-time multiplication using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprMultiply", "[parser][constexpr]") {
  // This test will validate that the constexprMultiply template function computes the product at compile time.
  constexpr double result1 = cse::ExpressionParser::constexprMultiply(8.0, 3.0);
  REQUIRE(result1 == 24.0);
  constexpr double result2 = cse::ExpressionParser::constexprMultiply(-2.0, 3.0);
  REQUIRE(result2 == -6.0);
  constexpr double result3 = cse::ExpressionParser::constexprMultiply(-2.0, -3.0);
  REQUIRE(result3 == 6.0);
}

  /**
 * @brief Tests the compile-time division using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprDivide", "[parser][constexpr]") {
  // This test will validate that the constexprDivide template function computes the quotient at compile time.
  constexpr double result1 = cse::ExpressionParser::constexprDivide(8.0, 2.0);
  REQUIRE(result1 == 4.0);
  constexpr double result2 = cse::ExpressionParser::constexprDivide(-4.0, 2.0);
  REQUIRE(result2 == -2.0);
  constexpr double result3 = cse::ExpressionParser::constexprDivide(-4.0, -2.0);
  REQUIRE(result3 == 2.0);
  REQUIRE_THROWS_AS(cse::ExpressionParser::constexprDivide(4.0, 0.0), std::logic_error);
}

  /**
 * @brief Tests the compile-time power function using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprPower", "[parser][constexpr]") {
  // This test will validate that the constexprPower template function computes the power at compile time.
  // Test positive exponents
  constexpr double result1 = cse::ExpressionParser::constexprPower(2.0, 3);
  REQUIRE(result1 == 8.0);
  constexpr double result2 = cse::ExpressionParser::constexprPower(-2.0, 3);
  REQUIRE(result2 == -8.0);
  // Test exponent of zero
  constexpr double result3 = cse::ExpressionParser::constexprPower(5.0, 0);
  REQUIRE(result3 == 1.0);
  constexpr double result4 = cse::ExpressionParser::constexprPower(-5.0, 0);
  REQUIRE(result4 == -1.0);
  // Test negative exponents
  constexpr double result5 = cse::ExpressionParser::constexprPower(2.0, -2);
  REQUIRE(result5 == 0.25);
  constexpr double result6 = cse::ExpressionParser::constexprPower(-2.0, -2);
  REQUIRE(result6 == 0.25);
  constexpr double result7 = cse::ExpressionParser::constexprPower(-2.0, -3);
  REQUIRE(result7 == -0.125);
}

  /**
 * @brief Tests the compile-time comparison using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprLessThan", "[parser][constexpr]") {
  // This test will validate that the constexprLessThan template function computes the < comparison at compile time.
  constexpr bool result1 = cse::ExpressionParser::constexprLessThan(8.0, 3.0);
  REQUIRE(result1 == false);
  constexpr bool result2 = cse::ExpressionParser::constexprLessThan(2.0, 3.0);
  REQUIRE(result2 == true);
  constexpr bool result3 = cse::ExpressionParser::constexprLessThan(-2.0, 2.0);
  REQUIRE(result3 == true);
  constexpr bool result4 = cse::ExpressionParser::constexprLessThan(2.0, 2.0);
  REQUIRE(result4 == false);
}

  /**
 * @brief Tests the compile-time comparison using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprEqualTo", "[parser][constexpr]") {
  // This test will validate that the constexprEqualTo template function computes the == comparison at compile time.
  constexpr bool result1 = cse::ExpressionParser::constexprEqualTo(8.0, 3.0);
  REQUIRE(result1 == false);
  constexpr bool result2 = cse::ExpressionParser::constexprEqualTo(-2.0, 2.0);
  REQUIRE(result2 == false);
  constexpr bool result3 = cse::ExpressionParser::constexprEqualTo(2.0, 2.0);
  REQUIRE(result3 == true);
}

/* Verifies that the template function correctly creates a lambda that performs the binary operation 
  (addition and subtraction) at compile time 
*/
TEST_CASE("ExpressionParser MakeBinaryFun - Compile-time Binary Operation", "[parser][template]") {
  // Test compile-time addition:
  constexpr auto addFun = cse::ExpressionParser::MakeBinaryFun(2.0, 3.0, std::plus<>{});
  static_assert(addFun() == 5.0, "Compile-time addition failed");
  REQUIRE(addFun() == 5.0);

  // Test compile-time subtraction:
  constexpr auto subFun = cse::ExpressionParser::MakeBinaryFun(10.0, 4.0, std::minus<>{});
  static_assert(subFun() == 6.0, "Compile-time subtraction failed");
  REQUIRE(subFun() == 6.0);
}

/* 
  TEST_CASE("Advanced Parser Funcitonality", "[parser]") {
    cse::ExpressionParser parser;
    parser.SetSymbolTable("val1", -2);
    parser.SetSymbolTable("val2", 2);
    parser.SetSymbolTable("val3", -15);
    parser.SetSymbolTable("val4", 15);
    parser.SetSymbolTable("val5", 0);
    parser.SetSymbolTable("val6", 30);
    parser.SetSymbolTable("val7", 1);
    parser.SetSymbolTable("val8", M_PI);
    parser.SetSymbolTable("val9", 4);
    parser.SetSymbolTable("val10", M_PI/2);
    parser.SetSymbolTable("val11", M_PI/3);
    parser.SetSymbolTable("val12", 8);
    parser.SetSymbolTable("val13", 9);
    parser.SetSymbolTable("val14", 27);

  
    REQUIRE(parser.Evaluate("cos({val8})") == -1);
    REQUIRE(parser.Evaluate("sin({val8}))") == sin(M_PI));
    REQUIRE(parser.Evaluate("cos({val11})") == cos(M_PI/3));
    REQUIRE(parser.Evaluate("sin({val10})") == 1);

    REQUIRE(parser.Evaluate("pow({val2}, {val2})") == 4);
    REQUIRE(parser.Evaluate("pow({val1}, {val2})") == 4);
    REQUIRE(parser.Evaluate("pow({val2}, {val1})") == 0.25);

} */
