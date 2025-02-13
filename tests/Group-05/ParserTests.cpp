#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-05/src/Parser.h"

TEST_CASE("Parser Evaluations", "[parser]") {
    Parser parser;
    // Addition tests
    REQUIRE(parser.Evaluate("{-2} + {2}") == 0);
    REQUIRE(parser.Evaluate("{-15} + {-15}") == -30);
    REQUIRE(parser.Evaluate("{15} + {15}") == 30);

    // Subtraction tests
    REQUIRE(parser.Evaluate("{-2} - {0}") == -2);
    REQUIRE(parser.Evaluate("{15} - {-15}") == 30);
    REQUIRE(parser.Evaluate("{-15} - {15}") == -30);
    REQUIRE(parser.Evaluate("{30} - {15}") == 15);

    // Multiplication tests
    REQUIRE(parser.Evaluate("{-2} * {0}") == 0);
    REQUIRE(parser.Evaluate("{-2} * {-2}") == 4);
    REQUIRE(parser.Evaluate("{2} * {2}") == 4);
    REQUIRE(parser.Evaluate("{-2} * {2}") == -4);

    // Division tests
    REQUIRE(parser.Evaluate("{0} / {2}") == 0);
    REQUIRE(parser.Evaluate("{-2} / {-2}") == 1);
    REQUIRE(parser.Evaluate("{2} / {2}") == 1);
    REQUIRE(parser.Evaluate("{-2} / {2}") == -1);
    REQUIRE(parser.Evaluate("{0.5} / {2}") == 0);
    REQUIRE(parser.Evaluate("{1} / {4}") == 0);
}