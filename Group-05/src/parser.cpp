#include "Parser.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

/**
 * @brief Default constructor for Parser object
 */
Parser::Parser() {}

/**
 * @brief Parses the next number in the equation, returning that number as a double
 * 
 * @param expression The string representing the equation
 * @param index The position of the parser in the string
 * @return The double result of the equation
 */
double Parser::ParseNumber(std::string expression, size_t &index) {
  assert(index < expression.size());
  assert(std::any_of(expression.begin(), expression.end(), ::isdigit));
  bool negative = false;
  while (index < expression.size() && expression[index] != '{') {
    index++; // Skip whitespace
  }

  if (expression[index] == '{') {
    index++; // Skip opening '{'
    int result = 0;

    // Extract the number inside the curly braces
    while (((index < expression.size()) && (std::isdigit(expression[index]))) ||
           expression[index] == '-') {
      if (expression[index] == 45) {
        negative = true;
        index++;
        continue;
      }
      result = result * 10 + (expression[index] - '0');
      index++;
    }

    if (expression[index] == '}') {
      index++; // Skip closing '}'
    }
    if (negative == true) {
      result *= -1;
    }
    return result;
  }

  std::cerr << "Error: Expected number inside {}" << std::endl;
  return 0; // Return a default error value
}


  /**
   * @brief Creates adding function that take a map with values to add
   * 
   * @param name1 Key for first value
   * @param name2 Key for second value
   * @return auto 
   */
  auto Parser::MakeAddFun(std::string name1, std::string name2) {
    using map_t = std::map<std::string, double>;
    return [name1,name2](map_t & m){
      return m[name1] + m[name2];
    };
  }

  /**
   * @brief Creates subtracting function that take a map with values to add
   * 
   * @param name1 Key for first value
   * @param name2 Key for second value
   * @return auto 
   */
  auto Parser::MakeSubtractFun(std::string name1, std::string name2) {
    using map_t = std::map<std::string, double>;
    return [name1,name2](map_t & m){
      return m[name1] - m[name2];
    };
  }

  /**
   * @brief Creates multiplication function that take a map with values to add
   * 
   * @param name1 Key for first value
   * @param name2 Key for second value
   * @return auto 
   */
  auto Parser::MakeMultiplyFun(std::string name1, std::string name2) {
    using map_t = std::map<std::string, double>;
    return [name1,name2](map_t & m){
      return m[name1] * m[name2];
    };
  }

  /**
   * @brief Creates dividation function that take a map with values to add
   * 
   * @param name1 Key for first value
   * @param name2 Key for second value
   * @return auto 
   */
  auto Parser::MakeDivideFun(std::string name1, std::string name2) {
    using map_t = std::map<std::string, double>;
    return [name1,name2](map_t & m){
      return m[name1] / m[name2];
    };
  }

  /**
   * @brief Evaluates equation represented by expression and returns simplified value as a double
   * 
   * @param expression Representing equation
   * @return double representing value of equation
   */
  double Parser::Evaluate(std::string expression) {
    assert(std::any_of(expression.begin(), expression.end(), ::isdigit));
    assert(std::any_of(expression.begin(), expression.end(), [](char c) {
    return c == '+' || c == '-' || c == '/' || c == '*';
    }));
    size_t index = 0;
    std::map<std::string, double> number_map;
    number_map["val1"] = ParseNumber(expression, index);  // Put the first number in map
    double result=0;
    while (index < expression.size()) {
      index++;
      char op = expression[index];  // Get the operator

      if (op == '+' || op == '-' || op == '*' || op == '/') {
        index++;  // Skip the operator
        double next_number = ParseNumber(expression, index);
        number_map["val2"] = next_number; //Put second number in map
        if (op == '+'){
            auto fun = MakeAddFun("val1", "val2"); //Creates function
            result = fun(number_map); //Evauluates operator
            break;
        }
        else if (op == '-'){
            auto fun = MakeSubtractFun("val1", "val2");
            result = fun(number_map);
            break;
          }
        else if (op == '*'){
            auto fun = MakeMultiplyFun("val1", "val2");
            result = fun(number_map);
            break;
          }
        else if (op == '/'){
            auto fun = MakeDivideFun("val1", "val2");
            if (next_number != 0) {
              result = fun(number_map);
            } else {
              std::cerr << "Error: Division by zero!" << std::endl;
              return 0;  // or handle error as needed
            }
            break;
        }
      }
    }

    return result;
  }

