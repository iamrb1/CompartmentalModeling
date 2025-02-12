/**
 * @file FunctionSetTest.cpp
 * @author jiang
 */

#include <pch.h>
#include "gtest/gtest.h"
#include <FunctionSet.h>

TEST(FunctionSetTest, IsEmpty)
{
    FunctionSet<int, int> eX1;
    EXPECT_TRUE(eX1.isEmpty());

    FunctionSet<int, int> eX2;
    ASSERT_EQ(eX2.countFunSet(),0U);
}

TEST(FunctionSetTest, IsAdded)
{
    FunctionSet<int, int> eY1;
    eY1.addFunction([](int x){ return x + 1; });
    eY1.addFunction([](int x){ return x * 2; });
    ASSERT_EQ(eY1.countFunSet(),2U);
}