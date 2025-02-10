// Anand
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-07/Team07Library/DataFileManager.h"
#include "../../Group-07/Team07Library/DataFileManager.cpp"

TEST_CASE("DataFileManagerTest Construct", "[DataFileManager]") {
  cse::DataFileManager dfm;
  dfm.openFile("../CSE498-Spring2025-team07-manager/Group-07/Data/sample.csv");
  REQUIRE(1 == 1);
}
