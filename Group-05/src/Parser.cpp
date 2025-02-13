#include "Parser.h"
#include <assert.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

// Constructor
Parser::Parser() {}

// Member function to evaluate the expression
int Parser::Evaluate(std::string expression) {
  assert(std::any_of(expression.begin(), expression.end(), ::isdigit));
  assert(std::any_of(expression.begin(), expression.end(), [](char c) {
    return c == '+' || c == '-' || c == '/' || c == '*';
}));
  size_t index = 0;
  double result = ParseNumber(expression, index);  // Get the first number

  while (index < expression.size()) {
    index++;
    char op = expression[index];  // Get the operator

    if (op == '+' || op == '-' || op == '*' || op == '/') {
      index++;  // Skip the operator
      double next_number = ParseNumber(expression, index);
      switch (op) {
        case '+':
          result += next_number;
          break;
        case '-':
          result -= next_number;
          break;
        case '*':
          result *= next_number;
          break;
        case '/':
          if (next_number != 0) {
            result /= next_number;
          } else {
            std::cerr << "Error: Division by zero!" << std::endl;
            return 0;  // or handle error as needed
          }
          break;
      }
    } else {
      break;  // No more valid operators
    }
  }

  return result;
}

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
