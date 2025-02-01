/**
 * @file FailSafeTest.cpp
 * @author Nitish Maindoliya
 */

#include <cse/ActionMap.h>
#include "gtest/gtest.h"

TEST(FailSafeTest, TestFailSafe) {
  EXPECT_EQ(1, 1);
};

TEST(FailSafeTest, TestFailSafe2) {
  EXPECT_EQ(1, 2);
};