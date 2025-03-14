/**
 * @file DatumTests.cpp
 *
 * @author Max Krawec
 *
 * Tests for the Datum class.
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include <string>

#include "../../Group-05/src/Datum.cpp"

// CITE: Used https://github.com/catchorg/Catch2/blob/devel/docs/comparing-floating-point-numbers.md
// to learn about REQUIRE_THAT (Used CHECK_THAT instead) and WithinAbs for testing doubles

static const double kEpsilon = 0.0001;

TEST_CASE("GetString() functionality", "[datum]") {
  // When a Datum contains a string
  cse::Datum string_test("test");
  CHECK(string_test.GetString() == "test");

  // When a Datum contains a double
  cse::Datum double_test(123.123);
  CHECK_THROWS(double_test.GetString());
}

TEST_CASE("GetDouble() functionality", "[datum]") {
  // When Datum contains a double
  cse::Datum double_test(999.777);
  CHECK_THAT(double_test.GetDouble(),
               Catch::WithinAbs(999.777, kEpsilon));

  // When Datum contains a string
  cse::Datum string_test("hello");
  CHECK_THROWS(string_test.GetDouble());
}

TEST_CASE("AsString() functionality", "[datum]") {
  // When Datum contains a double

  // When the value is NOT NaN
  cse::Datum double_test(123.456);
  double_test.AsString();
  CHECK(double_test.GetString() == "123.456");

  // When the value is NaN
  cse::Datum double_test1(std::numeric_limits<double>::quiet_NaN());
  double_test1.AsString();
  CHECK(double_test1.GetString() == "");


  // When Datum contains a double

  // When Datum contains a string
  cse::Datum string_test("hello");
  string_test.AsString();
  CHECK(string_test.GetString() == "hello");
}

TEST_CASE("AsDouble() functionality", "[datum]") {
  // When the Datum is a string

  // The string contains all valid numbers
  cse::Datum string_test("987.654");
  string_test.AsDouble();
  CHECK_THAT(string_test.GetDouble(),
             Catch::WithinAbs(987.654, kEpsilon));

  // The string contains valid numbers at the start
  cse::Datum string_test1("123Hello");
  string_test1.AsDouble();
  CHECK(std::isnan(string_test1.GetDouble()));

  // The string contains invalid numbers
  cse::Datum string_test2("test");
  string_test2.AsDouble();
  CHECK(std::isnan(string_test2.GetDouble()));

  // When the Datum is a double
  cse::Datum double_test(123.123);
  double_test.AsDouble();
  CHECK(double_test.GetDouble() == 123.123);
}

TEST_CASE("Addition functionality", "[datum]") {
  cse::Datum datum_double1(43.26);
  cse::Datum datum_string1("123ABC");

  // Adding two doubles
  cse::Datum datum_double2(23.89);
  cse::Datum result = datum_double1 + datum_double2;
  CHECK_THAT(result.GetDouble(),
               Catch::WithinAbs(67.15, kEpsilon));

  // Adding two strings
  cse::Datum datum_string2("test");
  cse::Datum result1 = datum_string1 + datum_string2;
  CHECK(result1.GetString() == "123ABCtest");

  // Adding 1 double datum and 1 string datum
  CHECK_THROWS(datum_double1 + datum_string1);

  // Adding 1 string datum and 1 double datum
  CHECK_THROWS(datum_string1 + datum_double1);
}

TEST_CASE("Subtraction functionality", "[datum]") {
  cse::Datum datum_double1(43.26);
  cse::Datum datum_string1("hello");

  // Subtracting two doubles
  cse::Datum datum_double2(23.89);
  cse::Datum result = datum_double1 - datum_double2;
  CHECK_THAT(result.GetDouble(),
               Catch::WithinAbs(19.37, kEpsilon));

  // Subtracting 1 double datum and 1 string datum
  CHECK_THROWS(datum_double1 - datum_string1);

  // Subtracting 1 string datum and 1 double datum
  CHECK_THROWS(datum_string1 - datum_double1);

  // Subtracting two string datums
  cse::Datum datum_string2("test");
  CHECK_THROWS(datum_string1 - datum_string2);
}

TEST_CASE("Multiplication functionality", "[datum]") {
  cse::Datum datum_double1(43.26);
  cse::Datum datum_string1("123ABC");

  // Multiplying two doubles
  cse::Datum datum_double2(23.89);
  cse::Datum result = datum_double1 * datum_double2;
  CHECK_THAT(result.GetDouble(),
               Catch::WithinAbs(1033.4814, kEpsilon));

  // Multiplying 1 double datum and 1 string datum
  CHECK_THROWS(datum_double1 * datum_string1);

  // Multiplying 1 string datum and 1 double datum
  CHECK_THROWS(datum_string1 * datum_double1);

  // Multiplying two string datums
  cse::Datum datum_string2("test");
  CHECK_THROWS(datum_string1 * datum_string2);
}

TEST_CASE("Division functionality", "[datum]") {
  cse::Datum datum_double1(43.26);
  cse::Datum datum_string1("123ABC");

  // Dividing two doubles

  // Denominator is NOT 0
  cse::Datum datum_double2(23.8874);
  cse::Datum result = datum_double1 / datum_double2;
  CHECK_THAT(result.GetDouble(),
               Catch::WithinAbs(1.811, kEpsilon));

  // Denominator IS 0
  cse::Datum datum_double3(0.000);
  CHECK_THROWS(datum_double1 / datum_double3);

  // Dividing 1 double datum and 1 string datum
  CHECK_THROWS(datum_double1 / datum_string1);

  // Dividing 1 string datum and 1 double datum
  CHECK_THROWS(datum_string1 / datum_double1);

  // Dividing 2 string datums
  cse::Datum datum_string2("test");
  CHECK_THROWS(datum_string1 / datum_string2);
}

TEST_CASE("Equal Functionality", "[datum]") {
  // Testing two strings
  cse::Datum datum_string("test");
  cse::Datum datum_string1("test");
  cse::Datum datum_string2("test2");
  cse::Datum datum_double(123.123);
  cse::Datum datum_double1(123.123);
  cse::Datum datum_double2(123.12369);

  // Testing two strings
  CHECK(datum_string == datum_string1);
  CHECK(datum_string != datum_string2);

  // Testing two doubles
  CHECK(datum_double == datum_double1);
  CHECK(datum_double != datum_double2);

  // Testing on double and one string
  CHECK(datum_double != datum_string);
}

TEST_CASE("Assignment functionality", "[datum]") {
  // Datum Assignment
  cse::Datum datum_double(123.123);
  cse::Datum datum_double1 = datum_double;
  CHECK_THAT(datum_double.GetDouble(),
             Catch::WithinAbs(123.123, kEpsilon));

  // Double assignment
  datum_double = 420.420;
  CHECK_THAT(datum_double.GetDouble(),
               Catch::WithinAbs(420.420, kEpsilon));

  // String assignment
  cse::Datum datum_string("test");
  datum_string = "testUpdate";
  CHECK(datum_string.GetString() == "testUpdate");
 }