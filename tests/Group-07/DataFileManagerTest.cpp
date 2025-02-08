// Anand
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-07/Team07Library/DataFileManager.h"

TEST_CASE("DataFileManagerTest Construct", "[DataFileManager]") {
  cse::DataFileManager dfm;
}

// New test case to create a CSV file
TEST_CASE("DataFileManagerTest MakeCSV", "[DataFileManager]") {
    cse::DataFileManager dfm;
    REQUIRE(1 == 1);
}
