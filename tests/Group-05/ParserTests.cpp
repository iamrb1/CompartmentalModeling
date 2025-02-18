#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../Group-05/src/Parser.cpp"
#include "../../Group-05/src/Parser.h"
#include <map>

TEST_CASE("Parser Evaluations", "[parser]") {
  Parser parser;
  std::map<std::string, double> symbolTable;
  symbolTable["val1"]=-2;
  symbolTable["val2"]=2;
  symbolTable["val3"]=-15;
  symbolTable["val4"]=15;
  symbolTable["val5"]=0;
  symbolTable["val6"]=30;
  symbolTable["val7"]=1;
 

  // Addition tests
  REQUIRE(parser.Evaluate(symbolTable, "{val1} + {val2}") == 0);
  REQUIRE(parser.Evaluate(symbolTable, "{val3} + {val3}") == -30);
  REQUIRE(parser.Evaluate(symbolTable, "{val4} + {val4}") == 30);

  // Subtraction tests
  REQUIRE(parser.Evaluate(symbolTable, "{val1} - {val5}") == -2);
  REQUIRE(parser.Evaluate(symbolTable, "{val4} - {val3}") == 30);
  REQUIRE(parser.Evaluate(symbolTable, "{val3} - {val4}") == -30);
  REQUIRE(parser.Evaluate(symbolTable, "{val6} - {val4}") == 15);

  // Multiplication tests
  REQUIRE(parser.Evaluate(symbolTable, "{val1} * {val4}") == -30);
  REQUIRE(parser.Evaluate(symbolTable, "{val1} * {val1}") == 4);
  REQUIRE(parser.Evaluate(symbolTable, "{val2} * {val2}") == 4);
  REQUIRE(parser.Evaluate(symbolTable, "{val1} * {val2}") == -4);

  // Division tests
  REQUIRE(parser.Evaluate(symbolTable, "{val5} / {val2}") == 0);
  REQUIRE(parser.Evaluate(symbolTable, "{val1} / {val1}") == 1);
  REQUIRE(parser.Evaluate(symbolTable, "{val2} / {val2}") == 1);
  REQUIRE(parser.Evaluate(symbolTable, "{val1} / {val2}") == -1);
  REQUIRE(parser.Evaluate(symbolTable, "{val7} / {val2}") == 0.5);
  
  }
