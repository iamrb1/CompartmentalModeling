// Anand
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-07/Team07Library/DataFileManager.h"
#include "../../Group-07/Team07Library/DataFileManager.cpp"
#include <filesystem>
#include <cstdlib>

/**
 * @brief Clears the file prior to concluding test function.
 * @param filePath The path of the file to clear.
 */
void resetFile(const std::string& filePath) {
    // https://en.cppreference.com/w/cpp/io/ios_base/openmode
    std::ofstream file(filePath, std::ios::trunc);  // Open truncated file to help clear it
    file.close();
}

/**
 * @brief Gets the root of the file location to implement tests on multiple machines.
 * @return The root directory of the project as a string.
 */
std::string getProjectRoot() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    // MAY NEED TO BE CHANGED DEPENDING ON FOLDER NAME
    while (!currentPath.empty() && currentPath.filename() != "CSE498-Spring2025") {
        currentPath = currentPath.parent_path();
    }
    return currentPath.string();
}

TEST_CASE("DataFileManager Construct and Destruct", "[DataFileManager]") {
    cse::DataFileManager dfm;
    // https://stackoverflow.com/questions/45931968/point-of-require-nothrow-in-the-catch-c-testing-framework
    REQUIRE_NOTHROW(cse::DataFileManager());
}

TEST_CASE("File Operations", "[DataFileManager]") {
    std::string projectRoot = getProjectRoot();
    std::cout << projectRoot << std::endl;
    std::string filePath = projectRoot + "/Group-07/Data/sample.csv";
    std::cout << filePath << std::endl;
    resetFile(filePath);  // Clear our file before running the test

    cse::DataFileManager dfm;
    dfm.openFile(filePath);

    REQUIRE(dfm.getFile() == filePath);
    REQUIRE(dfm.getData().size() == 0);  // Ensure file is initially empty

    dfm.closeFile();
    std::ifstream file(filePath);
    REQUIRE(file.is_open());

    // Ensure the file is empty after the test
    std::string line;
    REQUIRE(!std::getline(file, line));

    resetFile(filePath);  
}

TEST_CASE("Function Management", "[DataFileManager]") {
    std::string projectRoot = getProjectRoot();
    std::cout << projectRoot << std::endl;
    std::string filePath = projectRoot + "/Group-07/Data/sample.csv";
    std::cout << filePath << std::endl;
    resetFile(filePath);  // Clear the file before running the test

    cse::DataFileManager dfm;
    dfm.openFile(filePath);

    // Add functions
    int x = 5;
    int y = 10;
    dfm.addFunction("X Value", [&x]() { return x; });
    dfm.addFunction("Y Value", [&y]() { return y; });

    REQUIRE_NOTHROW(dfm.listFunctions());
    REQUIRE(dfm.getData().size() == 0);  // File is initially empty

    // Update CSV file with functions
    dfm.update();
    REQUIRE(dfm.checkUpdate() == false);  // Update made, flag reset to false
    x++;
    dfm.update();
    y++;
    dfm.update();

    // Read the file to check the updates
    std::ifstream file(filePath);
    std::string line;
    std::vector<std::vector<std::string>> data;

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    REQUIRE(data.size() == 4);  // 1 header row + 3 rows of updated data
    REQUIRE(data[0][0] == "Y Value");
    REQUIRE(data[0][1] == "X Value");
    REQUIRE(data[1][0] == "10");  // Initial y value
    REQUIRE(data[1][1] == "5"); // Initial x value
    REQUIRE(data[2][0] == "10"); // y remains 10
    REQUIRE(data[2][1] == "6"); // x incremented to 6
    REQUIRE(data[3][0] == "11"); // y incremented to 11 
    REQUIRE(data[3][1] == "6"); // x remains 6

    resetFile(filePath); 
}

TEST_CASE("File and Data Management", "[DataFileManager]") {
    std::string projectRoot = getProjectRoot();
    std::cout << projectRoot << std::endl;
    std::string filePath = projectRoot + "/Group-07/Data/sample.csv";
    std::cout << filePath << std::endl;
    resetFile(filePath);  

    cse::DataFileManager dfm;
    dfm.openFile(filePath);

    // Add dummy data
    int dummy = 42;
    dfm.addFunction("Dummy", [&dummy]() { return dummy; });

    // Update CSV file with dummy data
    dfm.update();

    // Read the file to check the updates
    std::ifstream file(filePath);
    std::string line;
    std::vector<std::vector<std::string>> data;

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    // Ensure file is updated correctly
    REQUIRE(data.size() == 2);  // 1 header row + 1 row of dummy data
    REQUIRE(data[0][0] == "Dummy");
    REQUIRE(data[1][0] == "42");  // Dummy data

    resetFile(filePath); 
}
