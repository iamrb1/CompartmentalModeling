/**
 * @file gtest_main.cpp
 * @author Nathan Staniszewski
 */

#include "gtest/gtest.h"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);


    return RUN_ALL_TESTS();
}