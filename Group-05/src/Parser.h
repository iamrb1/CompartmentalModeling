#ifndef PARSER_H
#define PARSER_H

#include <string>

class Parser {
 public:
  // Constructor
  Parser();

  // Member function to evaluate the expression
  int Evaluate(std::string expression);

 private:
  // Helper function to parse a number inside curly braces
  int ParseNumber(std::string expression, size_t &index);
};

#endif  // PARSER_H
