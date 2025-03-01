#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../Group-05/src/ExpressionParser.cpp"
#include "../../Group-05/src/ExpressionParser.h"
#include <map>
#include <math.h>

TEST_CASE("Advanced Parser Funcitonality", "[parser]") {
    cse::ExpressionParser parser;
    parser.SetSymbolTable("val1", -2);
    parser.SetSymbolTable("val2", 2);
    parser.SetSymbolTable("val3", -15);
    parser.SetSymbolTable("val4", 15);
    parser.SetSymbolTable("val5", 0);
    parser.SetSymbolTable("val6", 30);
    parser.SetSymbolTable("val7", 1);
    parser.SetSymbolTable("val8", M_PI);
    parser.SetSymbolTable("val9", 4);
    parser.SetSymbolTable("val10", M_PI/2);
    parser.SetSymbolTable("val11", M_PI/3);
    parser.SetSymbolTable("val12", 8);
    parser.SetSymbolTable("val13", 9);
    parser.SetSymbolTable("val14", 27);
    auto cubeRoot = [](double num) -> double {
        return cbrt(num); 
    };

    parser.SetFunction('CubeRoot', cubeRoot);

    REQUIRE(parser.Evaluate("{val1} ^ {val1}") == 4);
    REQUIRE(parser.Evaluate("{val1} ^ {val2}") == 4);
    REQUIRE(parser.Evaluate("{val2} ^ {val1}") == 0.25);

    REQUIRE(parser.Evaluate("sqrt({val9})") == 2);
    REQUIRE(parser.Evaluate("sqrt({val13})") == 3);

    REQUIRE(parser.Evaluate("CubeRoot({val12})") == 2);
    REQUIRE(parser.Evaluate("CubeRoot({val4})") == 3);

    REQUIRE(parser.Evaluate("cos({val8})") == -1);
    REQUIRE(parser.Evaluate("sin({val8}))") == 0);
    REQUIRE(parser.Evaluate("cos({val11})") == 0.5);
    REQUIRE(parser.Evaluate("sin({val10})") == 1);

    REQUIRE(parser.Evaluate("{val7}+{val2}*{val4})") == 31);
    REQUIRE(parser.Evaluate("{val7+val4}*{val2}^{val2})") == 61);
    REQUIRE(parser.Evaluate("({val7}+{val2})*{val4})") == 45);


}