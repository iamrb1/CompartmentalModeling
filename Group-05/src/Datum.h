/**
 * @file Datum.h
 *
 * @author Max Krawec
 *
 * A data class that can be either a string or double.
 */

#pragma once

#include <optional>
#include <string>
#include <variant>

namespace cse {

/**
 * A data class that can be either a string or double.
 */
class Datum {
private:
  /// Stores the string or double value.
  std::variant<std::string, double> mValue = 0.0;

  bool AreDatumsDouble(const Datum &datum) const;

  bool AreDatumsStrings(const Datum &datum) const;

public:
  /**
   * Constructor for a string value.
   * @param value The string value
   */
  Datum(std::string value) { mValue = value; }

  /**
   * Constructor for a double value.
   * @param value The double value
   */
  Datum(double value = 0) { mValue = value; }

  // CITE: Used https://www.geeksforgeeks.org/rule-of-five-in-cpp/ for the
  // default constructors

  /**
   * Default copy constructor
   * @param datum The other Datum
   */
  Datum(const Datum &datum) = default;

  /**
   * Default destructor
   */
  ~Datum() = default;

  /**
   * Default move constructor
   */
  Datum(Datum &&) noexcept = default;

  /**
   * Default move assignment opeartor
   * @param datum The other Datum
   * @return A reference to this Datum
   */
  Datum &operator=(Datum &&datum) noexcept = default;

  /**
   * Checks if the current type of the Datum is a string.
   * @return true if the type is a string, false otherwise.
   */
  bool IsString() const { return std::holds_alternative<std::string>(mValue); }

  /**
   * Checks if the current type of the Datum is a double.
   * @return true if the type is a double, false otherwise.
   */
  bool IsDouble() const { return std::holds_alternative<double>(mValue); }

  /**
   * Returns the Datum as a variant.
   * @return the variant value.
   */
  std::variant<std::string, double> GetVariant() const { return mValue; };

  std::optional<std::string> GetString() const;

  std::optional<double> GetDouble() const;

  void AsString();

  void AsDouble();

  Datum operator+(const Datum &datum) const;

  Datum operator-(const Datum &datum) const;

  Datum operator*(const Datum &datum) const;

  Datum operator/(const Datum &datum) const;

  bool operator==(const Datum &datum) const;

  Datum &operator=(const Datum &datum);

  Datum &operator=(const double &double_value);

  Datum &operator=(const std::string &string_value);
};

} // namespace cse
