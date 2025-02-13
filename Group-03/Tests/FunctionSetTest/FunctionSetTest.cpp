/**
 * @file FunctionSetTest.cpp
 * @author jiang
 */



#define CATCH_CONFIG_MAIN
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include <FunctionSet.h>

TEST_CASE("Test1", "Basic")
{
    int x  = 0;
    CHECK(x == 0);
}

TEST_CASE("Test2","IsEmpty")
{
    FunctionSet<int,int> set1;
    CHECK(set1.isEmpty());
}

TEST_CASE("Test3","AddFuntion")
{
    FunctionSet<int,int> set2;
    CHECK(set2.isEmpty());
    set2.addFunction([](int x){return x+1;});
    CHECK_FALSE(set2.isEmpty());
    set2.addFunction([](int x){return x+2;});
    CHECK(set2.countFunSet()==2U);
}
TEST_CASE("Test4","ClearAll")
{
    FunctionSet<int,int> set3;
    CHECK(set3.isEmpty());
    set3.addFunction([](int x){return x+1;});
    set3.addFunction([](int x){return x+2;});
    CHECK(set3.countFunSet()==2U);
    set3.clearAll();
    CHECK(set3.isEmpty());
}