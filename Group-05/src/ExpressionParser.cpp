/**
 * @file ExpressionParser.cpp
 *
 * @author Pedro Mitkiewicz
 */

#include "ExpressionParser.h"
#include "Datum.h"


namespace cse {

    /**
   * @brief Get the content inside Parenthesis
   * 
   * @param expression string representing equation
   * @param index position in string
   * @return std::string content inside parenthesis
   */
  template <typename SymbolTableType>
  std::string cse::ExpressionParser<SymbolTableType>::GetParenthContent(const std::string& expression, size_t index) {
    // Ensure that the index is within the string length and points to '('
    if (index >= expression.size() || expression[index] != '(') {
        throw std::invalid_argument("Index must point to an opening parenthesis.");
    }

    // Find the corresponding closing parenthesis ')'
    size_t closingIndex = expression.find(')', index);
    if (closingIndex == std::string::npos) {
        throw std::invalid_argument("Matching closing parenthesis ')' not found.");
    }

    // Extract and return the content between '(' and ')'

    return expression.substr(index + 1, closingIndex - index - 1);
  }

/**
 * @brief Parses the next key in the equation, returning that number as a double
 * 
 * @param expression The string representing the equation
 * @param index The position of the parser in the string
 * @return The first key found
 */
template <typename SymbolTableType>
std::function<cse::Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::ParseKey(const std::string &expression, size_t &index) {
  std::vector<std::vector<Datum>> symbol_table(3, std::vector<Datum>(5));
  symbol_table[0][0] = Datum(5.0);
  symbol_table[0][1] = Datum(3.5);
  symbol_table[0][2] = Datum(10);
  symbol_table[0][3] = Datum(-15);
  symbol_table[0][4] = Datum(3);

 

  assert(index <= expression.size());
  assert(std::any_of(expression.begin(), expression.end(), ::isdigit));
  std::string num_str;
  std::string result;
  while (index < expression.size() && expression.at(index)!='{' && isdigit(expression.at(index))==false && expression.at(index)!='(') {

    index++; 
  }
  
  if (expression[index]=='(') {
    if(isalpha(expression[index-1])){

      index=index-3;
      return KeyLambda(Datum(""));
    }
    else{
      return KeyLambda(Datum(""));
    }

  }

  if (expression[index]=='{') {
    index++; 
  }
  if(isdigit(expression.at(index))){
    while (index < expression.size() && isdigit(expression[index])) {
      num_str.push_back(expression[index]);
      index++;
    }
  }
  else{
    while (expression[index]!='}' && index<=expression.size()) {
      assert(expression[index]!='{');
      assert(index<=expression.size());
      result.push_back(expression[index]);
      index++;
    }
  }


    if(expression[index]!='}'){
      return NumLambda(stoi(num_str));
    }
    else{
      if(result==""){
        result=num_str;
      }
      auto func= KeyLambda(result);
      return KeyLambda(result);
    }

}

/**
 * @brief Creates adding function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::MakeAddFun(std::function<Datum(SymbolTableType &)> left, std::function<Datum(SymbolTableType &)> right) {

  //using map_t = std::map<std::string, double>;
  return [left, right](SymbolTableType & symbols) {
    if(left(symbols).IsString() || right(symbols).IsString() || std::isnan(left(symbols).AsDouble()) || std::isnan(right(symbols).AsDouble())){
      return std::nan("");
    }
    return left(symbols).AsDouble() + right(symbols).AsDouble();
  };
}

/**
 * @brief Creates subtracting function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::MakeSubFun(std::function<Datum(SymbolTableType &)> left, std::function<Datum(SymbolTableType &)> right) {

  //using map_t = std::map<std::string, double>;
  return [left, right](SymbolTableType & symbols) {
    if(left(symbols).IsString() || right(symbols).IsString() || std::isnan(left(symbols).AsDouble()) || std::isnan(right(symbols).AsDouble())){
      return std::nan("");
    }
    return left(symbols).AsDouble() - right(symbols).AsDouble();
  };
}

/**
 * @brief Creates multiplication function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::MakeMulFun(std::function<Datum(SymbolTableType &)> left, std::function<Datum(SymbolTableType &)> right) {

  //using map_t = std::map<std::string, double>;
  return [left, right](SymbolTableType & symbols) {
    if(left(symbols).IsString() || right(symbols).IsString() || std::isnan(left(symbols).AsDouble()) || std::isnan(right(symbols).AsDouble())){
      return std::nan("");
    }
    return left(symbols).AsDouble() * right(symbols).AsDouble();
  };
}
/**
 * @brief Creates cosine function that take a map with values to add
 *
 * @param key Key for first value
 * @return auto
 */
template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::MakeCosFun(std::function<Datum(SymbolTableType &)> expression_lambda) {
  return [expression_lambda](SymbolTableType &symbols) {
    return cos(expression_lambda(symbols).AsDouble());
  };
}

/**
 * @brief Creates sine function that take a map with values to add
 *
 * @param key Key for first value
 * @return auto
 */
template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::MakeSinFun(std::function<Datum(SymbolTableType &)> expression_lambda) {
  return [expression_lambda](SymbolTableType &symbols) {
    return sin(expression_lambda(symbols).AsDouble());
  };
}

/**
 * @brief Creates sine function that take a map with values to add
 *
 * @param key Key for first value
 * @return auto
 */
template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::MakeParenthFun(std::function<Datum(SymbolTableType &)> expression_lambda) {
  return [expression_lambda](SymbolTableType &symbols) {
    return expression_lambda(symbols).AsDouble();
  };
}


  /**
   * @brief Creates power function that take a map with values to perform operation
   *
   * @param left Lambda which returns the left value
   * @param right Lambda which returns the right value
   * @return auto Lambda function to perform operation
   */
  template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::MakeExpoFun(std::function<Datum(SymbolTableType &)> left, std::function<Datum(SymbolTableType &)> right) {
  //using map_t = std::map<std::string, double>;
  return [left, right](SymbolTableType & symbols) {
    if(left(symbols).IsString() || right(symbols).IsString() || std::isnan(left(symbols).AsDouble()) || std::isnan(right(symbols).AsDouble())){
      return std::nan("");
    }

    return pow(left(symbols).AsDouble(), right(symbols).AsDouble());
  };
}

/**
 * @brief Creates division function that take a map with values to add
 *
 * @param key1 Key for first value
 * @param key2 Key for second value
 * @return auto
 */
template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::MakeDivFun(std::function<Datum(SymbolTableType &)> left, std::function<Datum(SymbolTableType &)> right) {
  return [left, right](SymbolTableType & symbols) {
    if(left(symbols).IsString() || right(symbols).IsString() || std::isnan(left(symbols).AsDouble()) || std::isnan(right(symbols).AsDouble())){
      return std::nan("");
    }
    return left(symbols).AsDouble() / right(symbols).AsDouble();
  };
}



/**
 * @brief Evaluates equation represented by expression and returns simplified value as a double
 *
 * @param symbol_table_ Map with value and key that will be regerences in expression
 * @param expression Representing equation
 * @return double representing value of equation
 */
template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::MakeFunc( const std::string &expression, int precedence, size_t &index) {

  std::function<Datum(SymbolTableType &)> l1 = ParseKey(expression, index);  
  static size_t default_index = 0;

  //double result = 0;


  while (index < expression.size()) {
    getToken(expression, index);
    if (index>=expression.size()){
      return l1;
    }
    char op = expression.at(index);  
    char comparison_char;
    if (op=='+' || op=='-' || op=='*' || op=='/' || op=='c' || op=='s' || op=='^' || op=='(') {
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
      exponent(comparison_char);
      if (op==comparison_char) {
        if (precedence>=4){
          return l1;
        }
        l1 = MakeExpoFun(l1, MakeFunc(expression, 4, index)); //Creates function
      }
    }
      if (op=='c'){

        default_index=0;
        l1= MakeCosFun(MakeFunc(GetParenthContent(expression, index+3), 0, default_index));
        SkipParenthesis(index, expression);
      }

      if (op=='s'){
        default_index=0;
        l1= MakeSinFun(MakeFunc(GetParenthContent(expression, index+3), 0, default_index));
        SkipParenthesis(index, expression);
      }

      if (op=='('){
        default_index=0;
        l1= MakeParenthFun(MakeFunc(GetParenthContent(expression, index), 0, default_index));
        SkipParenthesis(index, expression);
      }
    
      if(index>=expression.size()){
        return l1;
      }
    }
    return l1;
  }

  template <typename SymbolTableType>
  size_t cse::ExpressionParser<SymbolTableType>::getToken(const std::string& expression, size_t &index) {
    // Iterate over the string and find the next instance of '+', '-', '*', or '/'
    for (; index < expression.size(); index++) {
        char c = expression[index];
        if (c == '+' || c == '-' || c == '*' || c == '/' || c=='^' || c=='(') {
            return index;  // Return the index of the found token
        }
        if (expression.substr(index, 4) == "cos(" || expression.substr(index, 4) == "sin(") {
          return index;  // Return the index where 'cos(' or 'sin(' starts
        }
        
    }
    
    // If no operator is found, return a special value indicating no operator
    return std::string::npos;  // std::string::npos is the largest possible value for size_t
}

template <typename SymbolTableType>
std::function<Datum(SymbolTableType &)> cse::ExpressionParser<SymbolTableType>::KeyLambda(const Datum &key) {
    if constexpr (requires(SymbolTableType &s) { s[int{}]; }) {
        // Index with a number (e.g., for vector)
        return [key](SymbolTableType &symbols) {
            return symbols[key.AsDouble()];
        };
    } else {
        // Index with a string — check if it's supported
        if constexpr (requires(SymbolTableType &s) { s[std::string{}]; }) {
            return [key](SymbolTableType &symbols) {
                return symbols[key.GetString()];
            };
        } else {
            return [key](SymbolTableType &) -> Datum {
                throw std::runtime_error("SymbolTableType does not support indexing");
            };
        }
    }
}


template <typename SymbolTableType>
void cse::ExpressionParser<SymbolTableType>::SetSymbolTable(const std::string &key, const double &num){
  symbol_table_[key] = num;
}

}


