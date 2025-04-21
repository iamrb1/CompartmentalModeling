#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "../../Group-10/Application/application.cpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

double FindOptimalValue (std::istream& stream, std::string line) {
  double totalValueFromApp = -1.0;
  while (std::getline(stream, line)) {
    if (line.find("Optimal Value Calculated:") != std::string::npos) {
      totalValueFromApp = std::stod(line.substr(line.find(":") + 1));
    }
  }
  return totalValueFromApp;
}

double RunOneOutputTest(std::ifstream& inputFile, std::string& line, std::ostringstream& capturedOutput) {
  if (inputFile.is_open()) {
    application(inputFile);
  } else {
    std::cout << "FILE NOT FOUND" << std::endl;
  }
  std::string output = capturedOutput.str();
  std::istringstream stream(output);

  double totalValueFromApp = FindOptimalValue(stream, line);
  return totalValueFromApp;
}



TEST_CASE("Basic", "[FinalApp]") {
  std::ifstream inputFile("DefaultAppScripts/cap10.txt");
  std::string line;
  std::ostringstream capturedOutput;

  // Redirect std::cout
  auto* originalBuf = std::cout.rdbuf();
  std::cout.rdbuf(capturedOutput.rdbuf());

  double totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);

  double expectedValue = 26.0;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));
  
  inputFile = std::ifstream("DefaultAppScripts/cap2.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 6.8;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  inputFile = std::ifstream("DefaultAppScripts/cap12.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 30.1;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  inputFile = std::ifstream("DefaultAppScripts/cap5.5.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 6.0;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  std::cout.rdbuf(originalBuf);
}

TEST_CASE("Weightless mode computes correct total value from script2",
          "[FinalApp][Weightless]") {
  std::ifstream inputFile("WeightlessAppScripts/ex2cap5.txt");
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
    if (line.find("Optimal Value Calculated:") != std::string::npos) {
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
    if (line.find("Optimal Value Calculated:") != std::string::npos) {
      totalValueFromApp = std::stod(line.substr(line.find(":") + 1));
    }
  }

  double expectedValue = 20.0;  // test for for sure

  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));
}
