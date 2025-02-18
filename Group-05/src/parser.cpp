#include "Parser.h"



/**
 * @brief Default constructor for Parser object
 */
Parser::Parser() {}

/**
 * @brief Parses the next key in the equation, returning that number as a double
 * 
 * @param expression The string representing the equation
 * @param index The position of the parser in the string
 * @return The first key found
 */
const std::string Parser::ParseKey(const std::string expression, size_t &index) {
  assert(index < expression.size());
  assert(std::any_of(expression.begin(), expression.end(), ::isdigit));

  while (index < expression.size() && expression[index] != '{') {
    index++; // Skip whitespace
  }

  if (expression[index] == '{') {
    index++; // Skip opening '{'
    std::string result;

    // Extract the string inside the curly braces
    while (index < expression.size() && expression[index] != '}') {
      result.push_back(expression[index]);
      index++;
    }

    if (expression[index] == '}') {
      index++; // Skip closing '}'
    }

    return result;
  }

  std::cerr << "Error: Expected string inside {}" << std::endl;
  return ""; // Return a default error value (empty string)
}


  /**
   * @brief Creates adding function that take a map with values to add
   * 
   * @param name1 Key for first value
   * @param name2 Key for second value
   * @return auto 
   */
  const auto Parser::MakeAddFun(const std::string name1, const std::string name2) {
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
  const auto Parser::MakeSubtractFun(const std::string name1, const std::string name2) {
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
  const auto Parser::MakeMultiplyFun(const std::string name1, const std::string name2) {
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
  const auto Parser::MakeDivideFun(const std::string name1, const std::string name2) {
    using map_t = std::map<std::string, double>;
    return [name1,name2](map_t & m){
      return m[name1] / m[name2];
    };
  }

  /**
   * @brief Evaluates equation represented by expression and returns simplified value as a double
   * 
   * @param number_map Map with value and key that will be regerences in expression
   * @param expression Representing equation
   * @return double representing value of equation
   */
  const double Parser::Evaluate(std::map<std::string, double> number_map, const std::string expression) {
    assert(std::any_of(expression.begin(), expression.end(), ::isdigit));
    assert(std::any_of(expression.begin(), expression.end(), [](char c) {
    return c == '+' || c == '-' || c == '/' || c == '*';
    }));
    size_t index = 0;
    std::string first_key;
    std::string second_key;
    first_key = ParseKey(expression, index);  // Put the first number in map
    double result=0;
    assert(number_map.find(first_key)!=number_map.end());

    while (index < expression.size()) {
      index++;
      char op = expression[index];  // Get the operator
      if (op == '+' || op == '-' || op == '*' || op == '/') {
        index++;  // Skip the operator
        second_key = ParseKey(expression, index);
        assert(number_map.find(second_key)!=number_map.end());
        if (op == '+'){
            auto fun = MakeAddFun(first_key, second_key); //Creates function
            result = fun(number_map); //Evauluates operator
            break;
        }
        else if (op == '-'){
            auto fun = MakeSubtractFun(first_key, second_key);
            result = fun(number_map);
            break;
          }
        else if (op == '*'){
            auto fun = MakeMultiplyFun(first_key, second_key);
            result = fun(number_map);
            break;
          }
        else if (op == '/'){
            auto fun = MakeDivideFun(first_key, second_key);
            assert(number_map[second_key]!=0);{
            result = fun(number_map);
            break;
        }
      }
    }

  }
  std::cout<<result<<'\n';
  return result;

  }