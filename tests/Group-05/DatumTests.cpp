/**
 * @file DatumTests.cpp
 *
 * @author Max Krawec
 *
 * Tests for the Datum class.
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include <string>

#include "../../Group-05/src/Datum.h"
#include "../../Group-05/src/Datum.cpp"

// CITE: Used https://github.com/catchorg/Catch2/blob/devel/docs/comparing-floating-point-numbers.md
//   to learn about REQUIRE_THAT and WithinAbs for testing doubles

// TODO - May need to change value, I just picked a random one
static const double g_epsilon = 0.0001;

TEST_CASE("GetString() functionality", "[datum]")
{
  SECTION("When Datum contains a string") {
    cse::Datum string_test("test");
    REQUIRE(string_test.GetString().has_value());
    REQUIRE(string_test.GetString().value()=="test");
  }

  SECTION("When Datum contains a double") {
    cse::Datum double_test(123.456);
    REQUIRE(!double_test.GetString().has_value());
  }
}

TEST_CASE("GetDouble() functionality", "[datum]")
{
  SECTION("When Datum contains a double") {
    cse::Datum double_test(999.777);
    REQUIRE(double_test.GetDouble().has_value());
    REQUIRE_THAT(double_test.GetDouble().value(), Catch::WithinAbs(999.777, g_epsilon));
  }

  SECTION("When Datum contains a string") {
    cse::Datum string_test("test2");
    REQUIRE(!string_test.GetDouble().has_value());
  }
}

TEST_CASE("AsString() functionality", "[datum]")
{
  SECTION("When Datum contains a string") {
    cse::Datum string_test("test3");
    string_test.AsString();
    REQUIRE(string_test.GetString().value()=="test3");
  }

  SECTION("When Datum contains a double") {

    SECTION("When the value is NOT nan") {
      cse::Datum double_test(123.456);
      double_test.AsString();
      REQUIRE(double_test.GetString().value()=="123.456");
    }

    SECTION("When the value IS nan") {
      cse::Datum double_test(std::numeric_limits<double>::quiet_NaN());
      double_test.AsString();
      REQUIRE(double_test.GetString().value()=="");
    }
  }
}

TEST_CASE("AsDouble() functionality", "[datum]")
{
  SECTION("When the Datum is a double") {

    SECTION("When Datum contains a string that CAN be converted") {
      cse::Datum string_test(123.123);
      string_test.AsDouble();
      REQUIRE_THAT(string_test.GetDouble().value(), Catch::WithinAbs(123.123, g_epsilon));
    }

    SECTION("When Datum contains a string that CANNOT be converted") {
      cse::Datum string_test("123hello456");
      string_test.AsDouble();
      REQUIRE(std::isnan(string_test.GetDouble().value()));
    }
  }

  SECTION("When the Datum is a string") {
    cse::Datum string_test("987.654");
    string_test.AsDouble();
    REQUIRE_THAT(string_test.GetDouble().value(), Catch::WithinAbs(987.654, g_epsilon));
  }
}

TEST_CASE("Addition functionality", "[datum]") {
  cse::Datum datum_double1(43.26);
  cse::Datum datum_string1("123ABC");

  SECTION("Adding two doubles") {
    cse::Datum datum_double2(23.89);
    cse::Datum result = datum_double1 + datum_double2;
    REQUIRE_THAT(result.GetDouble().value(), Catch::WithinAbs(67.15, g_epsilon));
  }

  SECTION("Adding one string and one double") {
    cse::Datum result = datum_string1 + datum_double1;
    REQUIRE(std::isnan(result.GetDouble().value()));
  }

  SECTION("Adding two strings") {
    cse::Datum datum_string2("test");
    cse::Datum result = datum_string1 + datum_string2;
    REQUIRE(std::isnan(result.GetDouble().value()));
  }
}

TEST_CASE("Subtraction functionality", "[datum]") {
  cse::Datum datum_double1(43.26);
  cse::Datum datum_string1("123ABC");

  SECTION("Subtracting two doubles") {
    cse::Datum datum_double2(23.89);
    cse::Datum result = datum_double1 - datum_double2;
    REQUIRE_THAT(result.GetDouble().value(), Catch::WithinAbs(19.37, g_epsilon));
  }

  SECTION("Subtracting one string and one double") {
    cse::Datum result = datum_string1 - datum_double1;
    REQUIRE(std::isnan(result.GetDouble().value()));
  }

  SECTION("Subtracting two strings") {
    cse::Datum datum_string2("test");
    cse::Datum result = datum_string1 - datum_string2;
    REQUIRE(std::isnan(result.GetDouble().value()));
  }
}

TEST_CASE("Multiplication functionality", "[datum]") {
  cse::Datum datum_double1(43.26);
  cse::Datum datum_string1("123ABC");

  SECTION("Multiplying two doubles") {
    cse::Datum datum_double2(23.89);
    cse::Datum result = datum_double1*datum_double2;
    REQUIRE_THAT(result.GetDouble().value(), Catch::WithinAbs(1033.4814, g_epsilon));
  }

  SECTION("Multiplying one string and one double") {
    cse::Datum result = datum_string1*datum_double1;
    REQUIRE(std::isnan(result.GetDouble().value()));
  }

  SECTION("Multiplying two strings") {
    cse::Datum datum_string2("test");
    cse::Datum result = datum_string1*datum_string2;
    REQUIRE(std::isnan(result.GetDouble().value()));
  }
}

TEST_CASE("Division functionality", "[datum]") {
  cse::Datum datum_double1(43.26);
  cse::Datum datum_string1("123ABC");

  SECTION("Dividing two doubles") {

    SECTION("Denominator is NOT 0") {
      cse::Datum datum_double2(23.8874);
      cse::Datum result = datum_double1/datum_double2;
      REQUIRE_THAT(result.GetDouble().value(), Catch::WithinAbs(1.811, g_epsilon));
    }

    SECTION("Denominator IS 0") {
      cse::Datum datum_double2(0.000);
      cse::Datum result = datum_double1/datum_double2;
      REQUIRE(std::isnan(result.GetDouble().value()));
    }
  }

  SECTION("Dividing one string and one double") {
    cse::Datum result = datum_string1/datum_double1;
    REQUIRE(std::isnan(result.GetDouble().value()));
  }

  SECTION("Dividing two strings") {
    cse::Datum datum_string2("test");
    cse::Datum result = datum_string1/datum_string2;
    REQUIRE(std::isnan(result.GetDouble().value()));
  }
}

TEST_CASE("Equal Functionality", "[datum]") {
  cse::Datum datum_string("test");
  cse::Datum datum_string1("test");
  cse::Datum datum_string2("test2");
  cse::Datum datum_double(123.123);
  cse::Datum datum_double1(123.123);
  cse::Datum datum_double2(123.12369);

  SECTION("Testing two strings") {
    REQUIRE(datum_string==datum_string1);
    REQUIRE(datum_string!=datum_string2);
  }

  SECTION("Testing two doubles") {
    REQUIRE(datum_double==datum_double1);
    REQUIRE(datum_double!=datum_double2);
  }

  SECTION("Testing one string and one double") {
    REQUIRE(datum_double!=datum_string);
    REQUIRE(datum_string!=datum_double);
  }
}

TEST_CASE("Assignment functionality", "[datum]") {

  SECTION("Datum assignment") {

    SECTION("Datum assignment") {
      cse::Datum datum_double(123.123);
      cse::Datum datum_double1 = datum_double;
      REQUIRE(datum_double==datum_double1);
    }

    SECTION("Datum self assignment") {
      cse::Datum datum_double(123.123);
      datum_double = datum_double;
      REQUIRE_THAT(datum_double.GetDouble().value(), Catch::WithinAbs(123.123, g_epsilon));
    }
  }

  SECTION("Double assignment") {
    cse::Datum datum_double(123.123);
    datum_double = 420.420;
    REQUIRE_THAT(datum_double.GetDouble().value(), Catch::WithinAbs(420.420, g_epsilon));
  }

  SECTION("String assignment") {
    cse::Datum datum_string("test");
    datum_string = "testUpdate";
    REQUIRE(datum_string.GetString().value()=="testUpdate");
  }
}