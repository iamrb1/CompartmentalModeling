#include "ExpressionParser.h"


namespace cse {
/**
 * @brief Parses the next key in the equation, returning that number as a double
 * 
 * @param expression The string representing the equation
 * @param index The position of the parser in the string
 * @return The first key found
 */
double cse::ExpressionParser::ParseKey(const std::string &expression, size_t &index) {
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

    if (expression[index]=='}') {
      index++; 
    }

    assert(symbol_table_.find(result)!=symbol_table_.end());
    return symbol_table_.at(result);
  }

  std::cerr << "Error: Expected string inside {}" << std::endl;
  return 0; 
}

/**
 * @brief Creates adding function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
auto cse::ExpressionParser::MakeAddFun(double left, const double right) {
  //using map_t = std::map<std::string, double>;
  return [left, right]() {
    return left+right;
  };
}

/**
 * @brief Creates subtracting function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
auto cse::ExpressionParser::MakeSubFun(const double left, const double right) {
  //using map_t = std::map<std::string, double>;
  return [left, right]() {
    return left-right;
  };
}

/**
 * @brief Creates multiplication function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
auto cse::ExpressionParser::MakeMulFun(const double left, const double right) {
  //using map_t = std::map<std::string, double>;
  return [left, right]() {
    return left*right;
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
auto cse::ExpressionParser::MakeDivFun(const double left, const double right) {
  //using map_t = std::map<std::string, double>;
  return [left, right]() {
    return left/right;
  };
}



/**
 * @brief Evaluates equation represented by expression and returns simplified value as a double
 *
 * @param symbol_table_ Map with value and key that will be regerences in expression
 * @param expression Representing equation
 * @return double representing value of equation
 */
double cse::ExpressionParser::Evaluate( const std::string &expression, int precedence, size_t &index) {
  double left;
  std::cout<<precedence<<'\n';

  left = ParseKey(expression, index);  
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
    if (index>=expression.size()){
      return left;
    }
    getToken(expression, index);
    char op = expression.at(index);  
    char comparison_char;
    if (op=='+' || op=='-' || op=='*' || op=='/') {
      addition(comparison_char);
      if (op==comparison_char) {
        if (precedence>=2){
          return left;
        }
        std::cout<<"black"<<'\n';
        auto fun = MakeAddFun(left, Evaluate(expression, 2, index)); //Creates function
        left = fun(); 
        std::cout<<"result:"<<left<<'\n';
      }
      subtraction(comparison_char);
      if (op==comparison_char) {
        std::cout<<"blue";
        if (precedence>=2){
          return left;
        }
        auto fun = MakeSubFun(left, Evaluate(expression, 2, index)); //Creates function
        left = fun(); 
      }
      multiplication(comparison_char);
      if (op==comparison_char) {
        std::cout<<"red";
        if (precedence>=3){
          return left;
        }
        auto fun = MakeMulFun(left, Evaluate(expression, 3, index)); //Creates function
        left = fun(); 
      }
      division(comparison_char);
      if (op==comparison_char) {
        std::cout<<"green";
        if (precedence>=3){
          return left;
        }
        auto fun = MakeDivFun(left, Evaluate(expression, 3, index)); //Creates function
        left = fun(); 
      }
      }
      if(index>=expression.size()){
        return left;
      }
    }
    return left;
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

