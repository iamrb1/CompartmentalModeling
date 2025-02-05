/**
 * @file Datum.cpp
 *
 * @author Max Krawec
 */

#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "Datum.h"

namespace cse {

// TODO - May need to change value, I just picked a random one
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
    if (std::isnan(GetDouble().value())) {
      SetStringValue("");
    } else {
      std::string string_numeric_value = std::to_string(GetDouble().value());

      // https://stackoverflow.com/questions/13686482/c11-stdto-stringdouble-no-trailing-zeros.
      // Used the above link to help remove trailing 0s and . when a double converts into a string.
      // find_last_not_of finds the last value of the indicated value (0 and .). Need to add 1 to find the first 0 or .
      // string::npos indicates everything until the end of the string. So remove everything after the last 0 or .
      string_numeric_value.erase(string_numeric_value.find_last_not_of('0') + 1, std::string::npos);
      string_numeric_value.erase(string_numeric_value.find_last_not_of('.') + 1, std::string::npos);
      SetStringValue(string_numeric_value);
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
      SetDoubleValue(std::stod(GetString().value()));
    } catch (std::invalid_argument &e) {
      // Citation: Used
      // https://stackoverflow.com/questions/16691207/c-c-nan-constant-literal
      // for NaN
      SetDoubleValue(std::numeric_limits<double>::quiet_NaN());
      std::cout << "Warning: Attempted to convert a string with no numeric "
                   "value. Replaced value with NaN."
                << std::endl;
      std::cout << e.what() << std::endl;
    }
  }
}


// TODO - May want to change the overloaded functions based on the specs
//  Need to figure out how we want to do comparisons with different types(>, >=, <, <=)
//  Also, need to test more (Will probably wait until we get a testing framework)

// Used https://www.geeksforgeeks.org/operator-overloading-cpp/ to help me with operator overloading
/**
 * Sums two Datums if they both contain doubles.
 * Otherwise, returns NaN if one Datum is a string and the other is a double, or if they are both strings.
 * @param datum the other Datum
 * @return A new Datum. If they are doubles, returns the sum of the Datums. Otherwise, returns NaN
 */
Datum Datum::operator+(const Datum &datum) const {
  if (AreDatumsDouble(datum)) {
    return {GetDouble().value() + datum.GetDouble().value()};
  }
  return {std::numeric_limits<double>::quiet_NaN()};
}

/**
 * Subtracts two Datums if they both contain doubles.
 * Otherwise, returns NaN if one Datum is a string and the other is a double, or if they are both strings.
 * @param datum the other Datum
 * @return A new Datum. If they are doubles, returns the difference between the Datums. Otherwise, returns NaN
 */
Datum Datum::operator-(const Datum &datum) const {
  if (AreDatumsDouble(datum)) {
    return {GetDouble().value() - datum.GetDouble().value()};
  }
  return {std::numeric_limits<double>::quiet_NaN()};
}

/**
 * Multiplies two Datums if they both contain doubles.
 * Otherwise, returns NaN if one Datum is a string and the other is a double, or if they are both strings.
 * @param datum the other Datum
 * @return A new Datum. If they are doubles, returns the products of the Datums. Otherwise, returns NaN
 */
Datum Datum::operator*(const Datum &datum) const {
  if (AreDatumsDouble(datum)) {
    return {GetDouble().value()*datum.GetDouble().value()};
  }
  return {std::numeric_limits<double>::quiet_NaN()};
}

/**
 * Divides two Datums if they both contain doubles.
 * Otherwise, returns NaN if one Datum is a string and the other is a double, or if they are both strings.
 * @param datum the other Datum
 * @return A new Datum. If they are doubles, returns the quotient of the Datums. Otherwise, returns NaN
 */
Datum Datum::operator/(const Datum &datum) const {
  if (AreDatumsDouble(datum)) {
    return {GetDouble().value()*datum.GetDouble().value()};
  }
  return {std::numeric_limits<double>::quiet_NaN()};
}

/**
 * Checks if two double or string Datums are equal.
 * Otherwise, returns false if one Datum is a string and the other is a double.
 * @param datum the other Datum
 * @return True if the Datums are the same type and equal each other. Otherwise, returns false
 */
bool Datum::operator==(const Datum &datum) const {
  if (AreDatumsDouble(datum)) {
    return std::abs(GetDouble().value() - datum.GetDouble().value()) < kEpsilon;
  } else if (AreDatumsStrings(datum)) {
    return GetString().value()==datum.GetString().value();
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
    if (GetString().has_value() && datum.GetString().has_value()) {
      return true;
    }
  }
  return false;
}

}  // namespace cse