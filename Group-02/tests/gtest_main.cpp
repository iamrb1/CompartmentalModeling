/**
 * @file gtest_main.cpp
 * @author Nitish Maindoliya
 */

#include "gtest/gtest.h"

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}