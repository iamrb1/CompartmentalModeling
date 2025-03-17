// Tests Header
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

// Misc. Headers
#include <iostream>
#include <stdexcept>
#include <string>

// Class Header
#include "../../Group-10/Classes/MemoryFactory.hpp"

TEST_CASE("MemoryFactory Construction", "[constructor]") {
  SECTION("Testing default construction") {
    // Constructor with Default Size
    cse::MemoryFactory<int> intFactory = cse::MemoryFactory<int>();
    REQUIRE(intFactory.GetAvailable() == 10);
    CHECK(intFactory.GetSize() == 10);
    // CHECK(intFactory.RunMemoryChecks(sizeof(int)*10));

    cse::MemoryFactory<std::string> stringFactory =
        cse::MemoryFactory<std::string>();
    REQUIRE(stringFactory.GetAvailable() == 10);
    CHECK(stringFactory.GetSize() == 10);
    CHECK(stringFactory.RunMemoryChecks(sizeof(std::string) * 10));
  }

  SECTION("Testing Custom Size for Allocation") {
    // Constructor with Custom Size
    cse::MemoryFactory<int> intFactory2 = cse::MemoryFactory<int>(20);
    REQUIRE(intFactory2.GetAvailable() == 20);
    CHECK(intFactory2.GetSize() == 20);
    CHECK(intFactory2.RunMemoryChecks(sizeof(int) * 20));

    cse::MemoryFactory<std::string> stringFactory2 =
        cse::MemoryFactory<std::string>(20);
    REQUIRE(stringFactory2.GetAvailable() == 20);
    CHECK(stringFactory2.GetSize() == 20);
    CHECK(stringFactory2.RunMemoryChecks(sizeof(std::string) * 20));

    cse::MemoryFactory<int> intFactory3 = cse::MemoryFactory<int>(1);
    REQUIRE(intFactory3.GetAvailable() == 1);
    CHECK(intFactory3.GetSize() == 1);
    CHECK(intFactory3.RunMemoryChecks(sizeof(int)));
  }

  SECTION("Testing Customized Initial Values for Allocation") {
    cse::MemoryFactory<int> intFactory = cse::MemoryFactory<int>(10, 37);
    REQUIRE(intFactory.GetAvailable() == 10);
    REQUIRE(intFactory.GetSize() == 10);
    int* firstItem = intFactory.Allocate();
    int* secondItem = intFactory.Allocate();
    REQUIRE(*firstItem == 37);
    REQUIRE(*secondItem == 37);
    intFactory.Deallocate(firstItem);
    intFactory.Deallocate(secondItem);

    std::string originalLiteral = "Hello There";
    cse::MemoryFactory<std::string> stringFactory =
        cse::MemoryFactory<std::string>(10, originalLiteral);
    std::string* stringLiteral = stringFactory.Allocate();
    std::string* stringLiteral2 = stringFactory.Allocate();
    REQUIRE(*stringLiteral == originalLiteral);
    REQUIRE(*stringLiteral2 == originalLiteral);
    stringFactory.Deallocate(stringLiteral);
    stringFactory.Deallocate(stringLiteral2);
  }
}

TEST_CASE("MemoryFactory Allocation", "[standard_use]") {
  SECTION("Testing variable allocation and assignments.") {
    cse::MemoryFactory<int> intFactory = cse::MemoryFactory<int>(5);
    REQUIRE(intFactory.GetAvailable() == 5);
    int* Test = intFactory.Allocate();
    *Test = 5;
    REQUIRE(intFactory.GetAvailable() == 4);
    REQUIRE(*Test == 5);
    intFactory.Deallocate(Test);
    REQUIRE(intFactory.GetAvailable() == 5);
  }

  SECTION("Testing advanced variable initialization") {
    cse::MemoryFactory<int> intFactory = cse::MemoryFactory<int>(5);
    REQUIRE(intFactory.GetAvailable() == 5);
    int* customTest = intFactory.Allocate(6);
    REQUIRE(*customTest == 6);
    intFactory.Deallocate(customTest);
    REQUIRE(intFactory.GetAvailable() == 5);

    int* defaultTest = intFactory.Allocate();
    REQUIRE(*defaultTest == 0);
    intFactory.Deallocate(defaultTest);

    struct TestStruct {
      double variable1 = 1.0;
      std::string variable2 = "";
      TestStruct() {}
      TestStruct(double var1, std::string var2)
          : variable1(var1), variable2(var2) {}

      bool operator!=(const TestStruct& other) {
        return (variable1 != other.variable1) || (variable2 != other.variable2);
      }

      bool operator==(const TestStruct& other) {
        return (variable1 == other.variable1) && (variable2 == other.variable2);
      }
    };

    cse::MemoryFactory<TestStruct> structFactory =
        cse::MemoryFactory<TestStruct>();
    TestStruct* struct1 = structFactory.Allocate(37.5, "Hello Struct");
    REQUIRE(struct1->variable1 == 37.5);
    REQUIRE(struct1->variable2 == "Hello Struct");
    structFactory.Deallocate(struct1);
  }
}

TEST_CASE("MemoryFactory Deallocation", "[errors]") {
  cse::MemoryFactory<int> intFactory = cse::MemoryFactory<int>(5);
  REQUIRE(intFactory.GetAvailable() == 5);
  int* Test = intFactory.Allocate();
  int errorVariable = 5;
  int* errorTest = &errorVariable;
  REQUIRE_THROWS_AS(intFactory.Deallocate(errorTest), std::invalid_argument);
  REQUIRE(intFactory.GetAvailable() == 4);
  intFactory.Deallocate(Test);
  REQUIRE(intFactory.GetAvailable() == 5);
  /*
    Deallocating one of the factory objects twice can lead to undefined behavior
    (as the factory believes it is in a default state and free to hand out to
    the next need) as such an error should be thrown
  */
  REQUIRE_THROWS_AS(intFactory.Deallocate(Test), std::invalid_argument);
  REQUIRE(intFactory.GetAvailable() == 5);
}

TEST_CASE("MemoryFactory Max Capacity Usage", "[private_functionality]") {
  SECTION("Testing max factory usage.") {
    cse::MemoryFactory<int> intFactory = cse::MemoryFactory<int>(2);
    int* item1 = intFactory.Allocate(0);
    CHECK(*item1 == 0);
    int* item2 = intFactory.Allocate(1);
    CHECK(*item2 == 1);
    REQUIRE(intFactory.GetAvailable() == 0);
    int* item3 = intFactory.Allocate(2);
    REQUIRE(*item3 == 2);
    REQUIRE(intFactory.GetAvailable() == 1);

    intFactory.Deallocate(item1);
    intFactory.Deallocate(item2);
    intFactory.Deallocate(item3);
    // Memory Allocated should have doubled after item3 was allocated
    REQUIRE(intFactory.GetAvailable() == 4);
  }
}