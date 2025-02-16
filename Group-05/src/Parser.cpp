#include "Parser.h"
#include <assert.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

// Constructor
Parser::Parser() {}

// Helper function to parse a number inside curly braces
int Parser::ParseNumber(std::string expression, size_t &index) {
  assert(index < expression.size());
  assert(std::any_of(expression.begin(), expression.end(), ::isdigit));
  bool negative = false;
  while (index < expression.size() && expression[index] != '{') {
    index++;  // Skip whitespace
  }

  if (expression[index] == '{') {
    index++;  // Skip opening '{'
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
      index++;  // Skip closing '}'
    }
    if (negative == true) {
      result *= -1;
    }
    return result;
  }

  std::cerr << "Error: Expected number inside {}" << std::endl;
  return 0;  // Return a default error value
}


  // Member function to evaluate the expression
  auto Parser::MakeAddFun(std::string name1, std::string name2) {
    using map_t = std::map<std::string, double>;
    return [name1,name2](map_t & m){
      return m[name1] + m[name2];
    };
  }

  auto Parser::MakeSubtractFun(std::string name1, std::string name2) {
    using map_t = std::map<std::string, double>;
    return [name1,name2](map_t & m){
      return m[name1] - m[name2];
    };
  }

  auto Parser::MakeMultiplyFun(std::string name1, std::string name2) {
    using map_t = std::map<std::string, double>;
    return [name1,name2](map_t & m){
      return m[name1] * m[name2];
    };
  }

  auto Parser::MakeDivideFun(std::string name1, std::string name2) {
    using map_t = std::map<std::string, double>;
    return [name1,name2](map_t & m){
      return m[name1] / m[name2];
    };
  }

  int Parser::Evaluate(std::string expression) {
    assert(std::any_of(expression.begin(), expression.end(), ::isdigit));
    assert(std::any_of(expression.begin(), expression.end(), [](char c) {
    return c == '+' || c == '-' || c == '/' || c == '*';
    }));
    size_t index = 0;
    std::map<std::string, double> number_map;
    number_map["val1"] = ParseNumber(expression, index);  // Get the first number
    double result=0;
    while (index < expression.size()) {
      index++;
      char op = expression[index];  // Get the operator

      if (op == '+' || op == '-' || op == '*' || op == '/') {
        index++;  // Skip the operator
        double next_number = ParseNumber(expression, index);
        number_map["val2"] = next_number;
        if (op == '+'){
            auto fun = MakeAddFun("val1", "val2");
            result = fun(number_map);
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

