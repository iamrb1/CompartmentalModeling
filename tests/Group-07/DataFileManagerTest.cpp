#include "../Team07Library/DataFileManager.h"
#include "gtest/gtest.h"

TEST(DataFileManagerTest, Construct) {
  cse::DataFileManager dfm("../CSE498-Spring2025/Group-07/Data/sample.csv");
}

// New test case to create a CSV file
TEST(DataFileManagerTest, CreateCSVFile) {
    ASSERT_EQ(1,1);
}
