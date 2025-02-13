/**
 * @file Datum.cpp
 *
 * @author Max Krawec
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "Datum.h"

namespace cse {

// TODO - May need to change value, I just picked a semi-random one
static const double kEpsilon = 0.0001;

/**
 * Returns the string value of the Datum if it contains a string.
 * @return An optional if the Datum contains a string, otherwise an empty
 * optional.
 */
std::optional<std::string> Datum::GetString() const {
  if (IsString()) {
    return std::get<std::string>(GetVariant());
  }
  return {};
}

/**
 * Returns the double value of the Datum if it contains a double.
 * @return An optional if the Datum contains a double, otherwise an empty
 * optional.
 */
std::optional<double> Datum::GetDouble() const {
  if (IsDouble()) {
    return std::get<double>(GetVariant());
  }
  return {};
}

/**
 * Converts the Datum into a string.
 * @warning If Datum is NaN, it converts into an empty string.
 */
void Datum::AsString() {
  if (GetDouble()) {
    assert(GetDouble().has_value());
    if (std::isnan(GetDouble().value())) {
      mValue = "";
    } else {
      std::string string_numeric_value = std::to_string(GetDouble().value());

      // CITE:
      // https://stackoverflow.com/questions/13686482/c11-stdto-stringdouble-no-trailing-zeros.
      // Used the above link to help remove trailing 0s and . when a double
      // converts into a string. find_last_not_of finds the last value of the
      // indicated value (0 and .). Need to add 1 to find the first 0 or .
      // string::npos indicates everything until the end of the string. So
      // remove everything after the last 0 or .
      string_numeric_value.erase(string_numeric_value.find_last_not_of('0') + 1,
                                 std::string::npos);
      string_numeric_value.erase(string_numeric_value.find_last_not_of('.') + 1,
                                 std::string::npos);
      mValue = string_numeric_value;
    }
  }
}

/**
 * Converts the Datum into a double
 * @warning If the Datum is an invalid double, it converts into NaN
 */
void Datum::AsDouble() {
  if (GetString()) {
    try {
      assert(GetString().has_value());
      // CITE: Used ChatGPT to write this code. There was a bug where, if the
      // string started with numbers, it
      //  wouldn't throw an invalid argument exception. Instead, it would take
      //  the valid numbers. For example, 123Hello --> 123. To fix this, stod
      //  can count the number of valid values that can become a double. If the
      //  value is less than the length of the string, we know there was a
      //  conversion error, and it throws an exception.
      std::size_t pos;
      double double_value = std::stod(GetString().value(), &pos);
      if (pos != GetString().value().length()) {
        throw std::invalid_argument("Invalid input: cannot convert to double");
      } else {
        mValue = double_value;
      }
    } catch (std::invalid_argument &e) {
      // CITE: Used
      // https://stackoverflow.com/questions/16691207/c-c-nan-constant-literal
      // for NaN
      mValue = std::numeric_limits<double>::quiet_NaN();
      std::cout << "Warning: Attempted to convert a string with no numeric "
                   "value. Replaced value with NaN."
                << std::endl;
      std::cout << e.what() << std::endl;
    }
  }
}

// TODO - May want to change the overloaded functions based on the specs
//  Need to figure out how we want to do comparisons with different types(>, >=,
//  <, <=)

// Used https://www.geeksforgeeks.org/operator-overloading-cpp/ to help me with
// operator overloading
/**
 * Sums two Datums if they both contain doubles.
 * Otherwise, returns NaN if one Datum is a string and the other is a double, or
 * if they are both strings.
 * @param datum the other Datum
 * @return A new Datum. If they are doubles, returns the sum of the Datums.
 * Otherwise, returns NaN
 */
Datum Datum::operator+(const Datum &datum) const {
  if (AreDatumsDouble(datum)) {
    assert(GetDouble().has_value() && datum.GetDouble().has_value());
    return {GetDouble().value() + datum.GetDouble().value()};
  }
  return {std::numeric_limits<double>::quiet_NaN()};
}

/**
 * Subtracts two Datums if they both contain doubles.
 * Otherwise, returns NaN if one Datum is a string and the other is a double, or
 * if they are both strings.
 * @param datum the other Datum
 * @return A new Datum. If they are doubles, returns the difference between the
 * Datums. Otherwise, returns NaN
 */
Datum Datum::operator-(const Datum &datum) const {
  if (AreDatumsDouble(datum)) {
    assert(GetDouble().has_value() && datum.GetDouble().has_value());
    return {GetDouble().value() - datum.GetDouble().value()};
  }
  return {std::numeric_limits<double>::quiet_NaN()};
}

/**
 * Multiplies two Datums if they both contain doubles.
 * Otherwise, returns NaN if one Datum is a string and the other is a double, or
 * if they are both strings.
 * @param datum the other Datum
 * @return A new Datum. If they are doubles, returns the products of the Datums.
 * Otherwise, returns NaN
 */
Datum Datum::operator*(const Datum &datum) const {
  if (AreDatumsDouble(datum)) {
    assert(GetDouble().has_value() && datum.GetDouble().has_value());
    return {GetDouble().value() * datum.GetDouble().value()};
  }
  return {std::numeric_limits<double>::quiet_NaN()};
}

/**
 * Divides two Datums if they both contain doubles.
 * Otherwise, returns NaN if one Datum is a string and the other is a double, or
 * if they are both strings.
 * @param datum the other Datum
 * @return A new Datum. If they are doubles, returns the quotient of the Datums.
 * Otherwise, returns NaN
 */
Datum Datum::operator/(const Datum &datum) const {
  if (AreDatumsDouble(datum) && datum.GetDouble().value() != 0.0) {
    assert(GetDouble().has_value() && datum.GetDouble().has_value());
    assert(datum.GetDouble().value() != 0.0);
    return {GetDouble().value() / datum.GetDouble().value()};
  }
  return {std::numeric_limits<double>::quiet_NaN()};
}

/**
 * Checks if two double or string Datums are equal.
 * Otherwise, returns false if one Datum is a string and the other is a double.
 * @param datum the other Datum
 * @return True if the Datums are the same type and equal each other. Otherwise,
 * returns false
 */
bool Datum::operator==(const Datum &datum) const {
  if (AreDatumsDouble(datum)) {
    assert(GetDouble().has_value() && datum.GetDouble().has_value());
    return std::abs(GetDouble().value() - datum.GetDouble().value()) < kEpsilon;
  } else if (AreDatumsStrings(datum)) {
    assert(GetString().has_value() && datum.GetString().has_value());
    return GetString().value() == datum.GetString().value();
  } else {
    return false;
  }
}

/**
 * Helper method to determine if two Datums are both doubles
 * @param datum The other Datum
 * @return True if both Datums are doubles. Otherwise, returns false
 */
bool Datum::AreDatumsDouble(const Datum &datum) const {
  if (IsDouble() && datum.IsDouble()) {
    assert(GetDouble().has_value() && datum.GetDouble().has_value());
    if (GetDouble().has_value() && datum.GetDouble().has_value()) {
      return true;
    }
  }
  return false;
}

/**
 * Helper method to determine if two Datums are both strings
 * @param datum The other Datum
 * @return True if both Datums are strings. Otherwise, returns false
 */
bool Datum::AreDatumsStrings(const Datum &datum) const {
  if (IsString() && datum.IsString()) {
    assert(GetString().has_value() && datum.GetString().has_value());
    if (GetString().has_value() && datum.GetString().has_value()) {
      return true;
    }
  }
  return false;
}

// Used https://www.geeksforgeeks.org/cpp-assignment-operator-overloading/ to
// help with assignment overloading Used ChatGPT to help with the assignment
// comments
/**
 * Assigns a value from a different Datum into this Datum.
 * @param datum The Datum to copy from.
 * @return A reference to this Datum after assignment.
 */
Datum &Datum::operator=(const Datum &datum) {
  // Checks if the object is not assigned to itself (From GeeksForGeeks)
  if (this != &datum) {
    mValue = datum.mValue;
  }
  return *this;
}

/**
 * Assigns a value from a double into this Datum.
 * @param double_value The double
 * @return  A reference to this Datum after assignment.
 */
Datum &Datum::operator=(const double &double_value) {
  mValue = double_value;
  return *this;
}

/**
 * Assigns a value from a string into this Datum.
 * @param string_value The string
 * @return  A reference to this Datum after assignment.
 */
Datum &Datum::operator=(const std::string &string_value) {
  mValue = string_value;
  return *this;
}
} // namespace cse