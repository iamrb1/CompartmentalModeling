#include <string>
#include <iostream>
#include <sstream>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../Group-06/ErrorManager/ErrorManager.cpp"
TEST_CASE("Test info error", "base")
{
    // this resource was used to redirect cout to stringstream:
    // https://truong.io/posts/capturing_stdout_for_c++_unit_testing.html
    cse::ErrorManager manager;
    

    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    manager.printError("Info error", cse::ErrorManager::ErrorLevel::Info);
    std::cout.rdbuf(oldCout);
    REQUIRE(buffer.str() == "[\033[32mInfo\033[0m]: Info error\n");
}