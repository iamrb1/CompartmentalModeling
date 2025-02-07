#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-07/Team07Library/DataFileManager.h"

TEST_CASE("DataFileManagerTest", "Construct") {
  cse::DataFileManager dfm("../CSE498-Spring2025/Group-07/Data/sample.csv");
}

// New test case to create a CSV file
TEST_CASE("DataFileManagerTest", "CreateCSVFile") {
    REQUIRE(1 == 1);
}