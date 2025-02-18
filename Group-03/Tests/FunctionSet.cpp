
 #include "../../third-party/Catch/single_include/catch2/catch.hpp"
 #include "../src/FunctionSet.h"
 
 int func1(int x){return x+2;}
 int func2(int x){return x+1;}
 int func3(int x){return x*3;}
 
 
 TEST_CASE("Test1", "Basic")
 {
     int x  = 0;
     CHECK(x == 0);
 }
 
 TEST_CASE("Test2","IsEmpty")
 {
     cse::FunctionSet<int,int> set1;
     CHECK(set1.isEmpty());
 }
 
 TEST_CASE("Test3","AddFuntion")
 {
     cse::FunctionSet<int,int> set2;
     CHECK(set2.isEmpty());
     set2.addFunction(func1);
     CHECK_FALSE(set2.isEmpty());
     set2.addFunction(func2);
     CHECK(set2.countFunSet()==2U);
 }
 TEST_CASE("Test4","ClearAll")
 {
     cse::FunctionSet<int,int> set3;
     CHECK(set3.isEmpty());
     set3.addFunction(func1);
     set3.addFunction(func2);
     CHECK(set3.countFunSet()==2U);
     set3.clearAll();
     CHECK(set3.isEmpty());
 }
 TEST_CASE("Test5","Index")
 {
     cse::FunctionSet<int,int> set5;
     set5.addFunction(func1);
     set5.addFunction(func2);
     CHECK(set5.countFunSet()==2U);
     CHECK(set5.findFunctionIndex(func2)==1);
 }
 TEST_CASE("Test6","RemoveFunc")
 {
     cse::FunctionSet<int,int> set6;
     set6.addFunction(func1);
     set6.addFunction(func2);
     set6.addFunction(func3);
     CHECK(set6.countFunSet()==3U);
     CHECK(set6.findFunctionIndex(func3)==2);
     set6.removeFunction(func2);
     CHECK(set6.findFunctionIndex(func3)==1);
 
 }