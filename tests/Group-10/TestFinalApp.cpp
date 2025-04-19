#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "../../Group-10/Application/application.cpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

TEST_CASE("Basic", "[FinalApp]") {
  std::ifstream inputFile("script1.txt");
  std::string line;

  if (inputFile.is_open()) {
    application(inputFile);
  } else {
    std::cout << "FILE NOT FOUND" << std::endl;
  }
}

TEST_CASE("Weightless mode computes correct total value from script2",
          "[FinalApp][Weightless]") {
  std::ifstream inputFile("script2.txt");
  std::ostringstream capturedOutput;

  // Redirect std::cout
  auto* originalBuf = std::cout.rdbuf();
  std::cout.rdbuf(capturedOutput.rdbuf());

  if (inputFile.is_open()) {
    application(inputFile);
  } else {
    std::cout << "FILE NOT FOUND" << std::endl;
  }

  std::cout.rdbuf(originalBuf);

  std::string output = capturedOutput.str();
  std::istringstream stream(output);
  std::string line;

  double totalValueFromApp = -1.0;

  while (std::getline(stream, line)) {
    if (line.find("Optimal Value:") != std::string::npos) {
      totalValueFromApp = std::stod(line.substr(line.find(":") + 1));
    }
  }
  double expectedValue = 27.1;

  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));
}

// Taking way too long
TEST_CASE("Repeats mode computes correct total value from script3",
          "[FinalApp][Repeats]") {
  std::ifstream inputFile("script3.txt");
  std::ostringstream capturedOutput;

  // Redirect std::cout to capture output
  auto* originalBuf = std::cout.rdbuf();
  std::cout.rdbuf(capturedOutput.rdbuf());

  if (inputFile.is_open()) {
    application(inputFile);
  } else {
    std::cout << "FILE NOT FOUND" << std::endl;
  }

  std::cout.rdbuf(originalBuf);  // Restore std::cout

  std::string output = capturedOutput.str();
  std::istringstream stream(output);
  std::string line;

  double totalValueFromApp = -1.0;

  while (std::getline(stream, line)) {
    if (line.find("Optimal Value:") != std::string::npos) {
      totalValueFromApp = std::stod(line.substr(line.find(":") + 1));
    }
  }

  double expectedValue = 20.0;  // test for for sure

  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));
}
