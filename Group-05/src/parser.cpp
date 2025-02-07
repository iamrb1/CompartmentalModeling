#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

// Boiler plate and general structure created using chatgpt, full implementation
// is original
class Parser {
 public:
  // Constructor
  Parser() {}
  // Member function to evaluate the expression
  int Evaluate(std::string expression) {
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

 private:
  // Helper function to parse a number inside curly braces
  int ParseNumber(std::string expression, size_t &index) {
    bool negative = false;
    while (index < expression.size() && expression[index] != '{') {
      index++;  // Skip whitespace
    }

    if (expression[index] == '{') {
      index++;  // Skip opening '{'
      int result = 0;

      // Extract the number inside the curly braces
      while (index < expression.size() && (std::isdigit(expression[index])) ||
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
};

int main() {
  // Will turn this into catch2 tests later.
  //  Right now decimal functionality is not implemented.
  // That would be the next step
  Parser parser;
  std::cout << (parser.Evaluate("{-2} + {2}") == 0) << "\n";
  std::cout << (parser.Evaluate("{-15} + {-15}") == -30) << "\n";
  std::cout << (parser.Evaluate("{15} + {15}") == 30) << "\n";
  std::cout << (parser.Evaluate("{-2} - {0}}") == -2) << "\n";
  std::cout << (parser.Evaluate("{15} - {-15}") == 30) << "\n";
  std::cout << (parser.Evaluate("{-15} - {15}") == -30) << "\n";
  std::cout << (parser.Evaluate("{30} - {15}") == 15) << "\n";
  std::cout << (parser.Evaluate("{-2} * {0}}") == 0) << "\n";
  std::cout << (parser.Evaluate("{-2} * {-2}") == 4) << "\n";
  std::cout << (parser.Evaluate("{2} * {2}") == 4) << "\n";
  std::cout << (parser.Evaluate("{-2} * {2}") == -4) << "\n";
  std::cout << (parser.Evaluate("{0} / {2}}") == 0) << "\n";
  std::cout << (parser.Evaluate("{-2} / {-2}") == 1) << "\n";
  std::cout << (parser.Evaluate("{2} / {2}") == 1) << "\n";
  std::cout << (parser.Evaluate("{-2} / {2}") == -1) << "\n";
  std::cout << (parser.Evaluate("{0.5} / {2}") == 0) << "\n";
  std::cout << (parser.Evaluate("{1} / {4}") == 0) << "\n";
  return 0;
}