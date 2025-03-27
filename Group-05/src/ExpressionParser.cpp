/**
 * @file ExpressionParser.cpp
 *
 * @author Pedro Mitkiewicz
 */

#include "ExpressionParser.h"


namespace cse {
/**
 * @brief Parses the next key in the equation, returning that number as a double
 * 
 * @param expression The string representing the equation
 * @param index The position of the parser in the string
 * @return The first key found
 */
std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::ParseKey(const std::string &expression, size_t &index) {
  assert(index < expression.size());
  assert(std::any_of(expression.begin(), expression.end(), ::isdigit));

  while (index < expression.size() && expression.at(index)!='{') {
    index++; 
  }

  if (expression[index]=='{') {
    index++; 
    std::string result;

    
    while (expression[index]!='}') {
      assert(expression[index]!='{');
      assert(index<expression.size());
      result.push_back(expression[index]);
      index++;
    }



    return KeyLambda(result);
  }

  std::cerr << "Error: Expected string inside {}" << std::endl;
  return KeyLambda(""); 
}

/**
 * @brief Creates adding function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeAddFun(std::function<double(std::map<std::string,double> &)> left, std::function<double(std::map<std::string,double> &)> right) {

  //using map_t = std::map<std::string, double>;
  return [left, right](std::map<std::string,double> & symbols) {
    return left(symbols) + right(symbols);
  };
}

/**
 * @brief Creates subtracting function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeSubFun(std::function<double(std::map<std::string,double> &)> left, std::function<double(std::map<std::string,double> &)> right) {

  //using map_t = std::map<std::string, double>;
  return [left, right](std::map<std::string,double> & symbols) {
    return left(symbols) - right(symbols);
  };
}

/**
 * @brief Creates multiplication function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeMulFun(std::function<double(std::map<std::string,double> &)> left, std::function<double(std::map<std::string,double> &)> right) {

  //using map_t = std::map<std::string, double>;
  return [left, right](std::map<std::string,double> & symbols) {
    return left(symbols) * right(symbols);
  };
}
/**
 * @brief Creates cosine function that take a map with values to add
 *
 * @param key Key for first value
 * @return auto
 */
auto cse::ExpressionParser::MakeCosFun(const std::string &key) {
  using map_t = std::map<std::string, double>;
  return [key](map_t &m) {
    return cos(m[key]);
  };
}

/**
 * @brief Creates sine function that take a map with values to add
 *
 * @param key Key for first value
 * @return auto
 */
auto cse::ExpressionParser::MakeSinFun(const std::string &key) {
  using map_t = std::map<std::string, double>;
  return [key](map_t &m) {
    return sin(m[key]);
  };
}

/**
 * @brief Creates exponent function that take a map with values to add
 *
 * @param key Key for first value
 * @return auto
 */
auto cse::ExpressionParser::MakeExpoFun(const std::string &key1, const std::string &key2) {
  using map_t = std::map<std::string, double>;
  return [key1, key2](map_t &m) {
    return pow(m[key1], m[key2]);
  };
}

/**
 * @brief Creates division function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeDivFun(std::function<double(std::map<std::string,double> &)> left, std::function<double(std::map<std::string,double> &)> right) {

  //using map_t = std::map<std::string, double>;
  return [left, right](std::map<std::string,double> & symbols) {
    return left(symbols) / right(symbols);
  };
}



/**
 * @brief Evaluates equation represented by expression and returns simplified value as a double
 *
 * @param symbol_table_ Map with value and key that will be regerences in expression
 * @param expression Representing equation
 * @return double representing value of equation
 */
std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeFunc( const std::string &expression, int precedence, size_t &index) {

  std::function<double(std::map<std::string,double> &)> l1 = ParseKey(expression, index);  
  std::map<std::string, double> symbol_table;
  symbol_table["val1"] = -2;
  symbol_table["val2"] = 2;
  symbol_table["val3"] = -15;
  symbol_table["val4"] = 15;
  symbol_table["val5"] = 0;
  symbol_table["val6"] = 30;
  symbol_table["val7"] = 1;
  //double result = 0;

/*   if (expression.find("cos(") != std::string::npos){
    auto fun = MakeCosFun(first_key);
    return fun(symbol_table_);
  }
  if (expression.find("sin(") != std::string::npos){
    auto fun = MakeSinFun(first_key);
    return fun(symbol_table_);
  }
  if (expression.find("pow(") != std::string::npos){
    second_key = ParseKey(expression, index);
    assert(symbol_table_.find(second_key)!=symbol_table_.end());
    auto fun = MakeExpoFun(first_key, second_key);
    return fun(symbol_table_);
  } */
  while (index < expression.size()) {
    getToken(expression, index);
    if (index>=expression.size()){
      return l1;
    }
    char op = expression.at(index);  
    char comparison_char;
    if (op=='+' || op=='-' || op=='*' || op=='/') {
      addition(comparison_char);
      if (op==comparison_char) {
        if (precedence>=2){
          return l1;
        }
        l1 = MakeAddFun(l1, MakeFunc(expression, 2, index)); //Creates function
      }
      subtraction(comparison_char);
      if (op==comparison_char) {
        if (precedence>=2){
          return l1;
        }
        l1 = MakeSubFun(l1, MakeFunc(expression, 2, index)); //Creates function
      }
      multiplication(comparison_char);
      if (op==comparison_char) {
        if (precedence>=3){
          return l1;
        }
        l1 = MakeMulFun(l1, MakeFunc(expression, 3, index)); //Creates function
      }
      division(comparison_char);
      if (op==comparison_char) {
        if (precedence>=3){
          return l1;
        }
        l1 = MakeDivFun(l1, MakeFunc(expression, 3, index)); //Creates function
      }
      }
      if(index>=expression.size()){
        return l1;
      }
    }
    return l1;
  }


  size_t cse::ExpressionParser::getToken(const std::string& expression, size_t &index) {
    // Iterate over the string and find the next instance of '+', '-', '*', or '/'
    for (; index < expression.size(); index++) {
        char c = expression[index];
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            return index;  // Return the index of the found token
        }
    }
    
    // If no operator is found, return a special value indicating no operator
    return std::string::npos;  // std::string::npos is the largest possible value for size_t
}



}

