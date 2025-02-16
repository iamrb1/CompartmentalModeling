#ifndef PARSER_H
#define PARSER_H

#include <string>

/**
 * @brief Class to parse string equations
 */
class Parser {
public:
  // Constructor
  Parser();

  // Member function to evaluate the expression
  double Evaluate(std::string expression);

private:
  // Helper function to parse a number inside curly braces
  double ParseNumber(std::string expression, size_t &index);
  auto MakeDivideFun(std::string name1, std::string name2);
  auto MakeAddFun(std::string name1, std::string name2);
  auto MakeSubtractFun(std::string name1, std::string name2);
  auto MakeMultiplyFun(std::string name1, std::string name2);
};

#endif // PARSER_H
