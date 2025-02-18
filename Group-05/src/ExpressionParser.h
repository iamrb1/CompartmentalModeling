#pragma once

#include <string>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

namespace cse {

/**
 * @brief Class to parse string equations
 */
class ExpressionParser {
  private:
  /**
   * @brief Parses the next key in the equation, returning that number as a double
   *
   * @param expression The string representing the equation
   * @param index The position of the parser in the string
   * @return The first key found
   */
   std::string ParseKey(const std::string expression, size_t &index);

  /**
   * @brief Creates division function that take a map with values to add
   *
   * @param name1 Key for first value
   * @param name2 Key for second value
   * @return auto Lambda function to perform operation
   */
   auto MakeDivideFun(const std::string name1, const std::string name2);

  /**
   * @brief Creates adding function that take a map with values to add
   *
   * @param name1 Key for first value
   * @param name2 Key for second value
   * @return auto Lambda function to perform operation
   */
   auto MakeAddFun(const std::string name1, const std::string name2);

  /**
   * @brief Creates subtracting function that take a map with values to add
   *
   * @param name1 Key for first value
   * @param name2 Key for second value
   * @return auto Lambda function to perform operation
   */
   auto MakeSubtractFun(const std::string name1, const std::string name2);

  /**
   * @brief Creates multiplication function that take a map with values to add
   *
   * @param name1 Key for first value
   * @param name2 Key for second value
   * @return auto Lambda function to perform operation
   */
   auto MakeMultiplyFun(const std::string name1, const std::string name2);

 public:
  /**
   * @brief Default constructor for ExpressionParser object
   */
  ExpressionParser() = default;

  /**
   * @brief Evaluates equation represented by expression and returns simplified value as a double
   *
   * @param number_map Map with value and key that will be regerences in expression
   * @param expression Representing equation
   * @return double representing value of equation
   */
   double Evaluate(std::map<std::string, double> number_map, const std::string expression);
};

} // namespace cse
