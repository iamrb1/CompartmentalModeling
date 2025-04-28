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

bool CheckForLine(std::istringstream & stream, std::string search) {
  stream.clear();
  stream.seekg(std::ios::beg);
  bool res = false;
  std::string line;
  while (std::getline(stream, line)) {
    if (line.find(search) != std::string::npos) {
      res = true;
    }
  }
  return res;
}


std::istringstream GetText(std::ifstream& inputFile, std::ostringstream& capturedOutput) {
  if (inputFile.is_open()) {
    application(inputFile);
  } else {
    std::cout << "FILE NOT FOUND" << std::endl;
  }
  std::string output = capturedOutput.str();
  std::istringstream stream(output);

  return stream;
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

  // Optimized Test Cases
  inputFile = std::ifstream("OptimizedAppScripts/cap2.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 6.8;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  inputFile = std::ifstream("OptimizedAppScripts/cap12.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 30.1;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  inputFile = std::ifstream("OptimizedAppScripts/cap5.5.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 6.0;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  std::cout.rdbuf(originalBuf);
}

TEST_CASE("Basic With CSV File", "[FinalApp]") {
  std::ifstream inputFile("CSVAppScripts/cap10csv.txt");
  std::string line;
  std::ostringstream capturedOutput;

  // Redirect std::cout
  auto* originalBuf = std::cout.rdbuf();
  std::cout.rdbuf(capturedOutput.rdbuf());

  double totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);

  double expectedValue = 26.0;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));
  
  inputFile = std::ifstream("CSVAppScripts/cap2csv.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  
  expectedValue = 6.8;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  inputFile = std::ifstream("CSVAppScripts/cap12csv.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 30.1;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  inputFile = std::ifstream("CSVAppScripts/cap5.5csv.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 6.0;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  // Optimized Test Cases
  inputFile = std::ifstream("CSVOptimizedAppScripts/cap2csv.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 6.8;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  inputFile = std::ifstream("CSVOptimizedAppScripts/cap12csv.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 30.1;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  inputFile = std::ifstream("CSVOptimizedAppScripts/cap5.5csv.txt");
  totalValueFromApp = RunOneOutputTest(inputFile, line, capturedOutput);
  
  expectedValue = 6.0;
  REQUIRE(totalValueFromApp == Approx(expectedValue).margin(0.01));

  std::cout.rdbuf(originalBuf);
}


TEST_CASE("Weightless mode computes correct total value from script2",
          "[FinalApp][Weightless]") {
  std::ifstream inputFile1("WeightlessAppScripts/ex2cap5.txt");
  std::ostringstream capturedOutput;
  std::string line;

  // Redirect std::cout
  auto* originalBuf = std::cout.rdbuf();
  std::cout.rdbuf(capturedOutput.rdbuf());

  double foundValue = RunOneOutputTest(inputFile1, line, capturedOutput);

  double expectedValue = 27.1;

  REQUIRE(foundValue == Approx(expectedValue).margin(0.01));

  std::ifstream inputFile2("WeightlessAppScripts/ex2cap10.txt");

  line = "";
  foundValue = RunOneOutputTest(inputFile2, line, capturedOutput);
  
  expectedValue = 39.2;
  REQUIRE(foundValue == Approx(expectedValue).margin(0.1));

  std::ifstream inputFile3("WeightlessAppScripts/ex1cap5.txt");
  line = "";
  foundValue = RunOneOutputTest(inputFile3, line, capturedOutput);
  expectedValue = 29.7;
  REQUIRE(foundValue == Approx(expectedValue).margin(0.1));

  std::cout.rdbuf(originalBuf);

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


void TestCompare(const std::string & path) {
  std::ifstream inputFile(path);
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

  std::vector<double> valuesFound;
  double speedUp;
  
  while (std::getline(stream, line)) {
    if (line.find("Optimal Value Calculated:") != std::string::npos) {
      valuesFound.push_back(std::stod(line.substr(line.find(":") + 1)));
    }
    if (line.find("Speedup:") != std::string::npos) {
      speedUp = std::stod(line.substr(line.find(":") + 1, line.length() - 1));
    }
  }
  REQUIRE(valuesFound.size() == 2);
  REQUIRE(valuesFound[0] == valuesFound[1]);
  REQUIRE(speedUp > 0.0);
}

TEST_CASE("Compare has the same values for both optimized and unoptimized, as well as a positive speedup",
  "[FinalApp][Compare]") {
  
  std::vector<std::string> filePaths = {"CompareAppScripts/ex1cap10.txt", "CompareAppScripts/ex2cap10.txt",
    "CompareAppScripts/ex1cap5.txt"};
  for (auto path : filePaths) {
    TestCompare(path);
  }
}

TEST_CASE("Check user input for a bad file / non-existent", "[FinalApp][BadFile]") {
  std::string fileRead = "BadFileScripts/BadName.txt";
  std::string line;
  std::ifstream input(fileRead);
  std::ostringstream capturedOutput;

  // Redirect std::cout
  auto* originalBuf = std::cout.rdbuf();
  std::cout.rdbuf(capturedOutput.rdbuf());
  
  auto outputStream = GetText(input, capturedOutput);
  std::cout.rdbuf(originalBuf);

  // Since we have multiple commands in BadName.txt, we're gonna check the output for multiple liens
  // this first one is the DNE.txt - should be invalid path error
  std::string wrongFileName = "DNE.txt";
  std::string toFind = "** \""+wrongFileName+"\" is not a valid file path";
  REQUIRE(CheckForLine(outputStream, toFind));

  // next test is bad file name - this corresponds to the second command in BadName.txt
  REQUIRE(CheckForLine(outputStream, "**The file must be of a valid type (.txt or .csv)"));
}


TEST_CASE("Check for bad formed inputs", "[FinalApp][MalformedQuery]") {
  std::string fileRead = "BadFileScripts/MalformedQuery.txt";
  std::string line;
  std::ifstream input(fileRead);
  std::ostringstream capturedOutput;

  // Redirect std::cout
  auto* originalBuf = std::cout.rdbuf();
  std::cout.rdbuf(capturedOutput.rdbuf());
  
  auto outputStream = GetText(input, capturedOutput);
  std::cout.rdbuf(originalBuf); // reset buffer

  REQUIRE(CheckForLine(outputStream, "**Command not recognized."));
  REQUIRE(CheckForLine(outputStream, "**Error: the quit command cannot be run with additional"));
  REQUIRE(CheckForLine(outputStream, "**Must specify a default capacity"));
  REQUIRE(CheckForLine(outputStream, "**Too many arguments provided for set-capacity"));
  REQUIRE(CheckForLine(outputStream, "**Invalid value given for the capacity"));
  REQUIRE(CheckForLine(outputStream, "**Please specify a filename as the second argument."));
  REQUIRE(CheckForLine(outputStream, "**Specify capacity=\033[32m<capacity>\033[0m."));
  REQUIRE(CheckForLine(outputStream, "**Capacity must be a numeric value."));
}

TEST_CASE("Check for bad file structure: missing columns", "[FinalApp][BadFileStructure]") {
  std::string fileRead = "BadFileScripts/MissingColumnsScript.txt";
  std::ifstream input(fileRead);
  std::ostringstream capturedOutput;

  // Redirect std::cout
  auto* originalBuf = std::cout.rdbuf();
  std::cout.rdbuf(capturedOutput.rdbuf());

  // Run the application
  auto outputStream = GetText(input, capturedOutput);
  std::cout.rdbuf(originalBuf); // Restore std::cout

  // Check for the specific error message
  REQUIRE(CheckForLine(outputStream, "CSV parse error on line 2 expected 3 fields, got: 2"));
}

TEST_CASE("Check for bad file structure: non-numeric values", "[FinalApp][BadFileStructure]") {
  std::string fileRead = "BadFileScripts/NonNumericValuesScript.txt";
  std::ifstream input(fileRead);
  std::ostringstream capturedOutput;

  // Redirect std::cout
  auto* originalBuf = std::cout.rdbuf();
  std::cout.rdbuf(capturedOutput.rdbuf());

  // Run the application
  auto outputStream = GetText(input, capturedOutput);
  std::cout.rdbuf(originalBuf); // Restore std::cout

  // Check for the specific error message
  REQUIRE(CheckForLine(outputStream, "CSV parse error on line 2 invalid argument."));
}