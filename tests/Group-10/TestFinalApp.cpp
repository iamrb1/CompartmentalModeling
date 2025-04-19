#include <iostream>
#include <fstream>
#include <string>

#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-10/Application/application.cpp"


/**
    Attempt script files with a duplicate main.cpp using filestream rather than std::cin
*/
// int main() {
//     PrintTerminal();
//     return 0;
// }

TEST_CASE("Baisc", "[FinalApp]") {
  std::ifstream inputFile("script1.txt");
  std::string line;

  
  if (inputFile.is_open()) {
    application(inputFile);
  } else {
    std::cout << "FILE NOT FOUND" << std::endl;
  }
}