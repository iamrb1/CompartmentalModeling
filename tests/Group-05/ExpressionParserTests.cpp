#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../Group-05/src/ExpressionParser.cpp"
#include "../../Group-05/src/ExpressionParser.h"
#include "../../Group-05/src/DataGrid.cpp"
#include "../../Group-05/src/Datum.cpp"
#include <map>
#include <functional>

constexpr double kPI = 3.14159265358979323846;

TEST_CASE("Datum Parser Evaluations", "[parser]") {
  cse::ExpressionParser<std::vector<Datum>> parser;
  std::vector<std::vector<Datum>> symbol_table(3, std::vector<Datum>(5));
  symbol_table[0][0] = Datum(5.0);
  symbol_table[0][1] = Datum(3.5);
  symbol_table[0][2] = Datum(10);
  symbol_table[0][3] = Datum(-15);
  symbol_table[0][4] = Datum(3);

  symbol_table[1][0] = Datum("string");
  symbol_table[1][1] = Datum("alsostring");
  symbol_table[1][2] = Datum("stringystring");
  symbol_table[1][3] = Datum("verystring");
  symbol_table[1][4] = Datum("superstring");


 
  static size_t default_index = 0;

  auto func=parser.MakeFunc("{0} + {1}", 0, default_index);
  REQUIRE(func(symbol_table[0]) == 8.5);
  REQUIRE(std::isnan(func(symbol_table[1]).AsDouble()));
  default_index = 0;
  func=parser.MakeFunc("{0} + {0} * {0}", 0, default_index);
  REQUIRE(func(symbol_table[0]) == 30);
  default_index = 0;
  func=parser.MakeFunc("{0} + {3} * {2} ^ ({0} - {4})", 0, default_index);
  REQUIRE(func(symbol_table[0]).AsDouble() == -1495);
  REQUIRE(std::isnan(func(symbol_table[1]).AsDouble()));
  default_index = 0;
  func=parser.MakeFunc("{2} / {0}", 0, default_index);
  REQUIRE(func(symbol_table[0]) == 2);
  REQUIRE(std::isnan(func(symbol_table[1]).AsDouble()));
  default_index = 0;
  func=parser.MakeFunc("{2} - {0}", 0, default_index);
  REQUIRE(func(symbol_table[0]) == 5);
  REQUIRE(std::isnan(func(symbol_table[1]).AsDouble()));

  
  }

TEST_CASE("Advanced Parser Funcitonality", "[parser]") {
  cse::ExpressionParser<std::map<std::string, double>> parser;
  std::map<std::string, double> symbol_table;
  symbol_table["val1"] = 1;
  symbol_table["val2"]= kPI;
  symbol_table["val3"] = 4;
  symbol_table["val4"] = kPI/2;
  symbol_table["val5"] = kPI/3;
  symbol_table["val6"] = 8;
  symbol_table["val7"] = 9;
  symbol_table["val8"] = 27;
  static size_t default_index = 0;

  auto func = parser.MakeFunc("cos({val2})", 0, default_index);
  REQUIRE(func(symbol_table) == -1);
  default_index = 0;
  func = parser.MakeFunc("sin({val2})", 0, default_index);
  REQUIRE(func(symbol_table) == sin(kPI));
  default_index = 0;
  func = parser.MakeFunc("cos({val5})", 0, default_index);
  REQUIRE(func(symbol_table) == cos(kPI/3));
  default_index = 0;
  func = parser.MakeFunc("sin({val1})", 0, default_index);
  REQUIRE(func(symbol_table) == sin(1));
  default_index = 0;
  func = parser.MakeFunc("sin({val4} + {val4})", 0, default_index);
  REQUIRE(func(symbol_table) == sin(kPI));
  default_index = 0;
  func = parser.MakeFunc("sin({val2} / 2)", 0, default_index);
  REQUIRE(func(symbol_table) == sin(kPI/2));
  default_index = 0;
  func = parser.MakeFunc("sin({val2} / 2)", 0, default_index);
  REQUIRE(func(symbol_table) == sin(kPI/2));

} 


TEST_CASE("Parser Evaluations", "[parser]") {
  cse::ExpressionParser<std::map<std::string, double>> parser;
  std::map<std::string, double> symbol_table;
  symbol_table["val1"] = -2;
  symbol_table["val2"] = 2;
  symbol_table["val3"] = -15;
  symbol_table["val4"] = 15;
  symbol_table["val5"] = 0;
  symbol_table["val6"] = 30;
  symbol_table["val7"] = 1;
 
  static size_t default_index = 0;

  auto func=parser.MakeFunc("{val1} + {val2}", 0, default_index);
  func(symbol_table);
  // Addition tests
  REQUIRE(func(symbol_table) == 0);
  default_index = 0;
  func=parser.MakeFunc("{val3} + {val3} * {val2}", 0, default_index);
  REQUIRE(func(symbol_table) == -45);
  default_index = 0;
  func=parser.MakeFunc("{val3} + {val3} * {val2} ^ ({val2} + {val2})", 0, default_index);
  REQUIRE(func(symbol_table) == -255);
  default_index = 0;
  func=parser.MakeFunc("{val4} + {val4} + {val3}", 0, default_index);
  REQUIRE(func(symbol_table) == 15);
  default_index = 0;
  func=parser.MakeFunc("{val4} + {val4} * {val2}", 0, default_index);
  REQUIRE(func(symbol_table)== 45);
  default_index = 0;
  func=parser.MakeFunc("{val4} + 2 * {val2}", 0, default_index);
  REQUIRE(func(symbol_table)== 19);
  default_index = 0;
  func=parser.MakeFunc("{val1} - {val5}", 0, default_index);
  // Subtraction tests
  REQUIRE(func(symbol_table) == -2);
  default_index = 0;
  func=parser.MakeFunc("{val4} - {val3}", 0, default_index);
  REQUIRE(func(symbol_table) == 30);
  default_index = 0;
  func=parser.MakeFunc("{val3} - {val4}", 0, default_index);
  REQUIRE(func(symbol_table) == -30);
  default_index = 0;
  func=parser.MakeFunc("{val6} - {val4}", 0, default_index);
  REQUIRE(func(symbol_table) == 15);
  default_index = 0;
  func=parser.MakeFunc("{val4} + {val4} * {val2} ^ {val2}", 0, default_index);
  REQUIRE(func(symbol_table)== 75);
  default_index = 0;
  func=parser.MakeFunc("{val1} ^ {val2}", 0, default_index);
  REQUIRE(func(symbol_table)== 4);
  default_index = 0;
  func=parser.MakeFunc("{val2} ^ {val2} ^ {val2}", 0, default_index);
  REQUIRE(func(symbol_table)== 16);
  default_index = 0;
  // Multiplication tests
  func=parser.MakeFunc("{val1} * {val4}", 0, default_index);
  REQUIRE(func(symbol_table) == -30);
  default_index = 0;
  func=parser.MakeFunc("{val1} * {val1}", 0, default_index);
  REQUIRE(func(symbol_table)==4);
  default_index = 0;
  func=parser.MakeFunc("{val2} * {val2}", 0, default_index);
  REQUIRE(func(symbol_table)== 4);
  default_index = 0;
  func=parser.MakeFunc("{val1} * {val2} + {val4}", 0, default_index);
  REQUIRE(func(symbol_table) == 11);
  default_index = 0;

  // Division tests
  func=parser.MakeFunc("{val5} / {val2}", 0, default_index);
  REQUIRE(func(symbol_table) == 0);
  default_index = 0;
  func=parser.MakeFunc("{val1} / {val1}", 0, default_index);
  REQUIRE(func(symbol_table) == 1);
  default_index = 0;
  func=parser.MakeFunc("{val2} / {val2}", 0, default_index);
  REQUIRE(func(symbol_table) == 1);
  default_index = 0;
  func=parser.MakeFunc("{val1} / {val2}", 0, default_index);
  REQUIRE(func(symbol_table) == -1);
  default_index = 0;
  func=parser.MakeFunc("{val7} / {val1}", 0, default_index);
  REQUIRE(func(symbol_table) == -0.5);
  default_index = 0;
  
  }

  /**
 * @brief Tests the compile-time addition using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprAdd", "[parser][constexpr]") {
  // This test will validate that the constexprAdd template function computes the sum at compile time.
  constexpr double result = cse::ExpressionParser<std::map<std::string, double>>::constexprAdd(2.0, 3.0);
  REQUIRE(result == 5.0);
}

  /**
 * @brief Tests the compile-time subtraction using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprSubtract", "[parser][constexpr]") {
  // This test will validate that the constexprSubtract template function computes the difference at compile time.
  constexpr double result1 = cse::ExpressionParser<std::map<std::string, double>>::constexprSubtract(8.0, 3.0);
  REQUIRE(result1 == 5.0);
  constexpr double result2 = cse::ExpressionParser<std::map<std::string, double>>::constexprSubtract(2.0, 3.0);
  REQUIRE(result2 == -1.0);
}

  /**
 * @brief Tests the compile-time multiplication using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprMultiply", "[parser][constexpr]") {
  // This test will validate that the constexprMultiply template function computes the product at compile time.
  constexpr double result1 = cse::ExpressionParser<std::map<std::string, double>>::constexprMultiply(8.0, 3.0);
  REQUIRE(result1 == 24.0);
  constexpr double result2 = cse::ExpressionParser<std::map<std::string, double>>::constexprMultiply(-2.0, 3.0);
  REQUIRE(result2 == -6.0);
  constexpr double result3 = cse::ExpressionParser<std::map<std::string, double>>::constexprMultiply(-2.0, -3.0);
  REQUIRE(result3 == 6.0);
}

  /**
 * @brief Tests the compile-time division using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprDivide", "[parser][constexpr]") {
  // This test will validate that the constexprDivide template function computes the quotient at compile time.
  constexpr double result1 = cse::ExpressionParser<std::map<std::string, double>>::constexprDivide(8.0, 2.0);
  REQUIRE(result1 == 4.0);
  constexpr double result2 = cse::ExpressionParser<std::map<std::string, double>>::constexprDivide(-4.0, 2.0);
  REQUIRE(result2 == -2.0);
  constexpr double result3 = cse::ExpressionParser<std::map<std::string, double>>::constexprDivide(-4.0, -2.0);
  REQUIRE(result3 == 2.0);
  REQUIRE_THROWS_AS(
    (cse::ExpressionParser<std::map<std::string, double>>::constexprDivide(4.0, 0.0)), std::logic_error);
}

  /**
 * @brief Tests the compile-time power function using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprPower", "[parser][constexpr]") {
  // This test will validate that the constexprPower template function computes the power at compile time.
  // Test positive exponents
  constexpr double result1 = cse::ExpressionParser<std::map<std::string, double>>::constexprPower(2.0, 3);
  REQUIRE(result1 == 8.0);
  constexpr double result2 = cse::ExpressionParser<std::map<std::string, double>>::constexprPower(-2.0, 3);
  REQUIRE(result2 == -8.0);
  // Test exponent of zero
  constexpr double result3 = cse::ExpressionParser<std::map<std::string, double>>::constexprPower(5.0, 0);
  REQUIRE(result3 == 1.0);
  constexpr double result4 = cse::ExpressionParser<std::map<std::string, double>>::constexprPower(-5.0, 0);
  REQUIRE(result4 == -1.0);
  // Test negative exponents
  constexpr double result5 = cse::ExpressionParser<std::map<std::string, double>>::constexprPower(2.0, -2);
  REQUIRE(result5 == 0.25);
  constexpr double result6 = cse::ExpressionParser<std::map<std::string, double>>::constexprPower(-2.0, -2);
  REQUIRE(result6 == 0.25);
  constexpr double result7 = cse::ExpressionParser<std::map<std::string, double>>::constexprPower(-2.0, -3);
  REQUIRE(result7 == -0.125);
}

  /**
 * @brief Tests the compile-time comparison using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprLessThan", "[parser][constexpr]") {
  // This test will validate that the constexprLessThan template function computes the < comparison at compile time.
  constexpr bool result1 = cse::ExpressionParser<std::map<std::string, double>>::constexprLessThan(8.0, 3.0);
  REQUIRE(result1 == false);
  constexpr bool result2 = cse::ExpressionParser<std::map<std::string, double>>::constexprLessThan(2.0, 3.0);
  REQUIRE(result2 == true);
  constexpr bool result3 = cse::ExpressionParser<std::map<std::string, double>>::constexprLessThan(-2.0, 2.0);
  REQUIRE(result3 == true);
  constexpr bool result4 = cse::ExpressionParser<std::map<std::string, double>>::constexprLessThan(2.0, 2.0);
  REQUIRE(result4 == false);
}

  /**
 * @brief Tests the compile-time comparison using constexpr lambda.
 */
TEST_CASE("ExpressionParser constexprEqualTo", "[parser][constexpr]") {
  // This test will validate that the constexprEqualTo template function computes the == comparison at compile time.
  constexpr bool result1 = cse::ExpressionParser<std::map<std::string, double>>::constexprEqualTo(8.0, 3.0);
  REQUIRE(result1 == false);
  constexpr bool result2 = cse::ExpressionParser<std::map<std::string, double>>::constexprEqualTo(-2.0, 2.0);
  REQUIRE(result2 == false);
  constexpr bool result3 = cse::ExpressionParser<std::map<std::string, double>>::constexprEqualTo(2.0, 2.0);
  REQUIRE(result3 == true);
}

/* Verifies that the template function correctly creates a lambda that performs the binary operation 
  (addition and subtraction) at compile time 
*/
TEST_CASE("ExpressionParser MakeBinaryFun - Compile-time Binary Operation", "[parser][template]") {
  // Test compile-time addition:
  constexpr auto addFun = cse::ExpressionParser<std::map<std::string, double>>::MakeBinaryFun(2.0, 3.0, std::plus<>{});
  static_assert(addFun() == 5.0, "Compile-time addition failed");
  REQUIRE(addFun() == 5.0);

  // Test compile-time subtraction:
  constexpr auto subFun = cse::ExpressionParser<std::map<std::string, double>>::MakeBinaryFun(10.0, 4.0, std::minus<>{});
  static_assert(subFun() == 6.0, "Compile-time subtraction failed");
  REQUIRE(subFun() == 6.0);
}





