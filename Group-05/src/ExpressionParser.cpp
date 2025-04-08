/**
 * @file ExpressionParser.cpp
 *
 * @author Pedro Mitkiewicz
 */

#include "ExpressionParser.h"


namespace cse {

  std::string cse::ExpressionParser::GetParenthesisContent(const std::string& expression, size_t index) {
    // Ensure that the index is within the string length and points to '('
    assert((index < expression.size() && expression[index] == '('));
    // Find the corresponding closing parenthesis ')'
    size_t closingIndex = expression.find(')', index);
    assert(closingIndex != std::string::npos);

    // Extract and return the content between '(' and ')'
    return expression.substr(index + 1, closingIndex - index - 1);
  }


std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::ParseKey(const std::string &expression, size_t &index) {
  assert(index <= expression.size());
  assert(std::any_of(expression.begin(), expression.end(), ::isdigit));
  std::string num_str;
  std::string key_str;
  while (index < expression.size() && expression.at(index)!='{' && isdigit(expression.at(index))==false && expression.at(index)!='(') {

    index++; 
  }
  //if parenthesis is found, check if it is preceded by an alpha
  if (expression[index]=='(') {
    if(isalpha(expression[index-1])){  //if preceded by an alpha, set index to first char of function and return empty str
      index=index-3;
      return KeyLambda(""); 
    }
    else{
      return KeyLambda(""); //if not, keep index to alpha and return empty string
    }

  }

  //if opening curly brace is found, skip it
  if (expression[index]=='{') {
    index++; 
  }
  if(isdigit(expression.at(index))){
    while (index < expression.size() && isdigit(expression[index])) {
      num_str.push_back(expression[index]); //if number, build number string
      index++;
    }
  }
  else{
    while (expression[index]!='}' && index<=expression.size()) {
      assert(expression[index]!='{');
      key_str.push_back(expression[index]); //if not number, build key string
      index++;
    }
  }


  //if closing brace, it is a key, return key. Otherwise, return number
    if(expression[index]!='}'){
      return NumLambda(stoi(num_str));
    }
    else{
      return KeyLambda(key_str);
    }

}

std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeAddFun(std::function<double(std::map<std::string,double> &)> left, std::function<double(std::map<std::string,double> &)> right) {
  return [left, right](std::map<std::string,double> & symbols) {
    return left(symbols) + right(symbols);
  };
}

std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeSubFun(std::function<double(std::map<std::string,double> &)> left, std::function<double(std::map<std::string,double> &)> right) {
  return [left, right](std::map<std::string,double> & symbols) {
    return left(symbols) - right(symbols);
  };
}

std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeMulFun(std::function<double(std::map<std::string,double> &)> left, std::function<double(std::map<std::string,double> &)> right) {
  return [left, right](std::map<std::string,double> & symbols) {
    return left(symbols) * right(symbols);
  };
}

std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeCosFun(std::function<double(std::map<std::string,double> &)> expression_lambda) {
  return [expression_lambda](std::map<std::string,double> &symbols) {
    return cos(expression_lambda(symbols));
  };
}


std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeSinFun(std::function<double(std::map<std::string,double> &)> expression_lambda) {
  return [expression_lambda](std::map<std::string,double> &symbols) {
    return sin(expression_lambda(symbols));
  };
}

std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeParenthFun(std::function<double(std::map<std::string,double> &)> expression_lambda) {
  return [expression_lambda](std::map<std::string,double> &symbols) {
    return expression_lambda(symbols);
  };
}

std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeExpoFun(std::function<double(std::map<std::string,double> &)> left, std::function<double(std::map<std::string,double> &)> right) {
  return [left, right](std::map<std::string,double> & symbols) {
    return pow(left(symbols), right(symbols));
  };
}

std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeDivFun(std::function<double(std::map<std::string,double> &)> left, std::function<double(std::map<std::string,double> &)> right) {
  return [left, right](std::map<std::string,double> & symbols) {
    assert(right!=0);
    return left(symbols) / right(symbols);
  };
}

//This function uses recursive calls to perform pratt parsing
//Pratt parser explanation: https://www.youtube.com/watch?v=2l1Si4gSb9A
std::function<double(std::map<std::string,double> &)> cse::ExpressionParser::MakeFunc( const std::string &expression, int precedence, size_t &index) {
  std::function<double(std::map<std::string,double> &)> l1 = ParseKey(expression, index);  //Get location next first key or number of equation
  static size_t default_index = 0;

  while (index < expression.size()) {
    GetToken(expression, index); //get next operator token index
    if (index>=expression.size()){
      return l1; //if token not found, exist function
    }
    char op = expression.at(index);  
    char comparison_char;
    //switch for each operator, not an actual switch in order to use const expressions
    if (operators_.find(op)!=std::string::npos) {
      addition(comparison_char);
      if (op==comparison_char) {
        if (precedence>=2){
          return l1;
        }
        l1 = MakeAddFun(l1, MakeFunc(expression, 2, index)); //Creates function, make func called recursively in order to perform pratt parsing

      }
      subtraction(comparison_char);
      if (op==comparison_char) {
        if (precedence>=2){
          return l1;
        }
        l1 = MakeSubFun(l1, MakeFunc(expression, 2, index)); 
      }
      multiplication(comparison_char);
      if (op==comparison_char) {
        if (precedence>=3){
          return l1;
        }
        l1 = MakeMulFun(l1, MakeFunc(expression, 3, index)); 
      }
      division(comparison_char);
      if (op==comparison_char) {
        if (precedence>=3){
          return l1;
        }
        l1 = MakeDivFun(l1, MakeFunc(expression, 3, index)); 
      }
      exponent(comparison_char);
      if (op==comparison_char) {
        if (precedence>=4){
          return l1;
        }
        l1 = MakeExpoFun(l1, MakeFunc(expression, 4, index)); 
      }
    
      if (op=='('){
        if(expression.substr(index-3,4)=="cos("){
          default_index=0;
          l1= MakeCosFun(MakeFunc(GetParenthesisContent(expression, index), 0, default_index));
           //Make func called recursively with priority xero, parenthesis treated like new function
          SkipParenthesis(index, expression); //find index at the end of parenthesis
        }
        else if(expression.substr(index-3,4)=="sin("){
          default_index=0;
          l1= MakeSinFun(MakeFunc(GetParenthesisContent(expression, index), 0, default_index));
          SkipParenthesis(index, expression);
        }
        else{
          default_index=0;
          l1= MakeParenthFun(MakeFunc(GetParenthesisContent(expression, index), 0, default_index));
          SkipParenthesis(index, expression);
        }
      }
    }
    
      //If overrun the string, exit function
      if(index>=expression.size()){
        return l1;
      }
    }
    return l1;
  }

  size_t cse::ExpressionParser::GetToken(const std::string& expression, size_t &index) {
    index=expression.find_first_of(operators_, index);
    return index;  // std::string::npos is the largest possible value for size_t
}




}


