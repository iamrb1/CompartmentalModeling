/**
 * @file ExpressionParser.h
 *
 * @author Pedro Mitkiewicz Calen Green and Muhammad Asif Masood
 *
 * A class providing a framework for parsing and evaluating mathematical expressions
 */

#pragma once

#include <string>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
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
   * @brief set char to subtraction operator
   * 
   * @param sub_char comparison char
   */
  constexpr void subtraction(char& sub_char){
    sub_char='-';
  }

  /**
   * @brief Get the Token object
   * 
   * @param expression string representing equation
   * @param index representing position in expression
   * @return size_t index of next token
   */
  size_t getToken(const std::string& expression, size_t &index);

  /**
   * @brief set char to addition operator
   * 
   * @param add_char comparison char
   */
  constexpr void addition(char& add_char){
    add_char='+';
  }

  /**
   * @brief set char to division operator
   * 
   * @param div_char comparison char
   */
  constexpr void division(char& div_char){
    div_char='/';
  }

    /**
   * @brief set char to division operator
   * 
   * @param div_char comparison char
   */
  constexpr void exponent(char& exp_char){
    exp_char='^';
  }


  /**
   * @brief set char to multiplication operator
   * 
   * @param mult_char comparison char
   */
  constexpr void multiplication(char& mult_char){
    mult_char='*';
  }

  /**
   * @brief Get the content inside Parenthesis
   * 
   * @param expression string representing equation
   * @param index position in string
   * @return std::string content inside parenthesis
   */
  std::string GetParenthContent(const std::string& expression, size_t index);

  std::map<std::string, double> symbol_table_;
  /**
   * @brief Parses the next key in the equation, returning that number as a double
   *
   * @param expression The string representing the equation
   * @param index The position of the parser in the string
   * @return Function returning next key
   */
  std::function<double(std::map<std::string,double> &)> ParseKey(const std::string &expression, size_t &index);

  /**
   * @brief Creates multiplication function that take a map with values to add
   *
   * @param left Lambda which returns the left value
   * @param right Lambda which returns the right value
   * @return auto Lambda function to perform operation
   */
  std::function<double(std::map<std::string,double> &)> MakeDivFun(std::function<double(std::map<std::string,double> &left)>, std::function<double(std::map<std::string,double> &right)>);


  /**
   * @brief Creates multiplication function that take a map with values to add
   *
   * @param left Lambda which returns the left value
   * @param right Lambda which returns the right value
   * @return auto Lambda function to perform operation
   */
  std::function<double(std::map<std::string,double> &)> MakeAddFun(std::function<double(std::map<std::string,double> &left)>, std::function<double(std::map<std::string,double> &right)>);

  /**
   * @brief Creates multiplication function that take a map with values to add
   *
   * @param left Lambda which returns the left value
   * @param right Lambda which returns the right value
   * @return auto Lambda function to perform operation
   */
  std::function<double(std::map<std::string,double> &)> MakeSubFun(std::function<double(std::map<std::string,double> &)>, std::function<double(std::map<std::string,double> &right)>);


  /**
   * @brief Creates multiplication function that take a map with values to add
   *
   * @param left Lambda which returns the left value
   * @param right Lambda which returns the right value
   * @return auto Lambda function to perform operation
   */
  std::function<double(std::map<std::string,double> &)> MakeMulFun(std::function<double(std::map<std::string,double> & left)>, std::function<double(std::map<std::string,double> & right)>);


    /**
   * @brief Creates cosine function that takes a map with values
   *
   * @param expression_lambda lambda returning value inside parenthesis
   * @return auto Lambda function to perform operation
   */
  std::function<double(std::map<std::string,double> &)> MakeCosFun(std::function<double(std::map<std::string,double> &)> expression_lambda);

      /**
   * @brief Creates parenthesis function that takes a map with values
   *
   * @param expression_lambda lambda returning value inside parenthesis
   * @return auto Lambda function to perform operation
   */
  std::function<double(std::map<std::string,double> &)> MakeParenthFun(std::function<double(std::map<std::string,double> &)> expression_lambda);

    /**
   * @brief Creates sine function that takes a map with values
   *
   * @param expression_lambda lambda returning value inside parenthesis
   * @return auto Lambda function to perform operation
   */
  std::function<double(std::map<std::string,double> &)> MakeSinFun(std::function<double(std::map<std::string,double> &)> expression_lambda);


  /**
   * @brief Creates power function that take a map with values to perform operation
   *
   * @param left Lambda which returns the left value
   * @param right Lambda which returns the right value
   * @return auto Lambda function to perform operation
   */
  std::function<double(std::map<std::string,double> &)> MakeExpoFun(std::function<double(std::map<std::string,double> &left)>, std::function<double(std::map<std::string,double> &right)>);

 public:
  /**
   * @brief Default constructor for ExpressionParser object
   */
  ExpressionParser() = default;

  /**
 * @brief Adds two values at compile time.
 *
 * @tparam T A type that supports the + operator.
 * @param a First value.
 * @param b Second value.
 * @return The sum of a and b.
 */
  template <typename T>
  static constexpr T constexprAdd(T a, T b) {
    return a + b;
}

/**
 * @brief Subtracts two values at compile time.
 *
 * @tparam T A type that supports the - operator.
 * @param a First value (minuend).
 * @param b Second value (subtrahend).
 * @return The result of a - b.
 */
  template <typename T>
  static constexpr T constexprSubtract(T a, T b) {
    return a - b;
}

/**
 * @brief Multiplies two values at compile time.
 *
 * @tparam T A type that supports the * operator.
 * @param a First value.
 * @param b Second value.
 * @return The product of a and b.
 */
  template <typename T>
  static constexpr T constexprMultiply(T a, T b) {
    return a * b;
}

/**
 * @brief Divides two values at compile time.
 *
 * @tparam T A type that supports the / operator.
 * @param a Numerator.
 * @param b Denominator (must not be zero).
 * @return The quotient of a divided by b.
 */
  template <typename T>
  static constexpr T constexprDivide(T a, T b) { 
    return b != 0 ? a / b : throw std::logic_error("Division by zero");
}

/**
 * @brief Raises a base to an exponent at compile time.
 *
 * @tparam T A type that supports multiplication.
 * @param base The base value.
 * @param exp The exponent (must be non-negative).
 * @return The result of base^exp.
 */
  template <typename T>
  static constexpr T constexprPower(T base, int exp) {
    if (exp == 0) {
        // check sign of base number
        return base < 0 ? -1 : 1;
    }
    if (exp < 0) {
        return 1 / constexprPower(base, -exp);  // Handle negative exponents
    }
    T halfPower = constexprPower(base, exp / 2); // for explicitly handling -base to an odd exp
    if (exp % 2 == 0) {
        return halfPower * halfPower;  // Even exponent
    } else {
        return base * halfPower * halfPower;  // Odd exponent
    }
}

/**
 * @brief Compares two values at compile time.
 *
 * @tparam T A type that supports comparison operators.
 * @param a First value.
 * @param b Second value.
 * @return True if a < b, False otherwise.
 */
  template <typename T>
  static constexpr bool constexprLessThan(T a, T b) {
    return a < b;
}

/**
 * @brief Compares two values at compile time to determine if they are equal.
 *
 * @tparam T A type that supports comparison operators.
 * @param a The first value.
 * @param b The second value.
 * @return true if a is equal to b, false otherwise.
 */
  template <typename T>
  static constexpr bool constexprEqualTo(T a, T b) {
    return a == b;
}

std::function<double(std::map<std::string,double> & symbols)> MakeFunc( const std::string &expression, int precedence, size_t &index);

/**
 * @brief Creates a binary operation function at compile time.
 *
 * @tparam Operation A callable type that performs a binary operation.
 * @param left The left-hand operand.
 * @param right The right-hand operand.
 * @param op The binary operation.
 * @return A constexpr lambda that returns the result of op(left, right).
 */
  template <typename Operation>
  static constexpr auto MakeBinaryFun(double left, double right, Operation op) {
      return [=]() constexpr { return op(left, right); };
}

  /**
   * @brief Evaluates equation represented by expression and returns simplified value as a double
   *
   * @param number_map Map with value and key that will be regerences in expression
   * @param expression Representing equation
   * @return double representing value of equation
   */
  double Evaluate(const std::string &expression, int precedence, size_t &index);

  auto KeyLambda(std::string key){
    return [key](std::map<std::string,double> & symbols){ return symbols[key]; 
    };
  }

  auto NumLambda(double num){
   return [num](std::map<std::string,double> &){ return num;}; 
    };

   /**
    * @brief Get the Symbol Table object
    * 
    * @return std::map<std::string, double> symbol table
    */
   std::map<std::string, double>& GetSymbolTable(){
    return symbol_table_;
   }

   /**
    * @brief Set a pair in the Symbol Table object
    * 
    * @param key string key for symbol table
    * @param num double value for symbol table
    */
    void SetSymbolTable(const std::string &key, const double &num){
      symbol_table_[key] = num;
    }
  };

  /**
   * @brief Set index to the end of the parenthesis
   * 
   * @param index position inside expression
   * @param expression string representing equation
   */
  void SkipParenthesis(size_t &index, const std::string expression) {
    // Increment index until we find the '(' character
    while (index < expression.size() && expression[index] != ')') {
        index++;
    }
    index++;
}
}






