/**
* @file DataMapTest.cpp
* @author Rahul B
*/

#include <cse/AdvancedDataMap.hpp>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

TEST_CASE("DataMap Default Constructor Test", "[DataMap]") {
  cse::AdvDataMap data_map;
  data_map.insert("key", 20);

  SECTION("DataMap is not empty") {
    bool y = data_map.empty();
    REQUIRE(y == 0);
  }

  SECTION("Valid DataMap keys") {
    REQUIRE(data_map.count("key") == 1);
  }
}

TEST_CASE("DataMap Initializer List Constructor Test", "[DataMap]") {
  cse::AdvDataMap data_map = {{"name", std::string("professor")}, {"age", 22}, {"dob", std::string("0120")}};

  SECTION("DataMap is not empty") {
    REQUIRE_FALSE(data_map.empty());
  }

  SECTION("Valid DataMap keys") {
      const std::vector<std::string> expected_keys = {"name", "age", "dob"};
      for (const auto& key : expected_keys) {
          REQUIRE(data_map.count(key) == 1);
      }
  }
}

TEST_CASE("DataMap Edge Case", "[DataMap]") {
  cse::AdvDataMap data_map;

  SECTION("Access a key that is not in DataMap") {
    REQUIRE_FALSE(data_map.contains("key"));
  }
}

TEST_CASE("DataMap Copy Constructor Tests", "[DataMap]") {
  cse::AdvDataMap data_map = {{"name", std::string("professor")}, {"age", 22}, {"dob", std::string("0120")}};

  cse::AdvDataMap copy(data_map);

  SECTION("Copy contains same key values as DataMap") {
    REQUIRE(copy.contains("name"));
    REQUIRE(copy.contains("age"));
    REQUIRE(copy.contains("dob"));

    std::string n = data_map.get<std::string>("name");
    int val = data_map.get<int>("age");
    std::string d = data_map.get<std::string>("dob");

    REQUIRE(n == "professor");
    REQUIRE(val == 22);
    REQUIRE(d == "0120");
  }

  SECTION("Change in DataMap should not change copy") {
    data_map.insert("new", 10);
    REQUIRE_FALSE(copy.contains("new"));
  }

  SECTION("Copy should not change DataMap") {
    data_map.clear();
    REQUIRE(copy.size() == 3);
  }
}

TEST_CASE("DataMap Copy Assignment Tests") {
  cse::AdvDataMap data_map = {{"name", std::string("professor")}, {"age", 22}, {"dob", std::string("0120")}};

  cse::AdvDataMap copy;
  copy = data_map;

  SECTION("Copy contains same key values as DataMap") {
    REQUIRE(copy.contains("name"));
    REQUIRE(copy.contains("age"));
    REQUIRE(copy.contains("dob"));

    std::string n = data_map.get<std::string>("name");
    int val = data_map.get<int>("age");
    std::string d = data_map.get<std::string>("dob");

    REQUIRE(n == "professor");
    REQUIRE(val == 22);
    REQUIRE(d == "0120");
  }

  SECTION("Change in DataMap should not change copy") {
    data_map.insert("new", 10);
    REQUIRE_FALSE(copy.contains("new"));
  }
}

TEST_CASE("DataMap Insert Method Tests", "[DataMap]") {
  cse::AdvDataMap data_map;

  SECTION("Insert into DataMap") {
    data_map.insert("key", 20);
    int val = data_map.at<int>("key");
    REQUIRE(val == 20);
  }

  SECTION("DataMap contains method") {
    data_map.insert("key", 20);
    bool t = data_map.contains("key");
    REQUIRE(t == 1);

    bool f = data_map.contains("map");
    REQUIRE(f == 0);
  }
}

TEST_CASE("DataMap [] operator Test") {
  cse::AdvDataMap data_map;
  data_map["key"] = 80;
  int value = data_map["key"].get<int>();
  REQUIRE(value == 80);

  data_map["key_two"];
  auto val = data_map["key_two"].get<int>();
  REQUIRE(val == 0);

  SECTION("key not specified should not cause an error") {
    REQUIRE_NOTHROW(data_map["key_three"]);
      auto val2 = data_map["key_three"].get<int>();
      REQUIRE(val2 == 0);
  }
}

TEST_CASE("DataMap .at method Tests", "[DataMap]") {
  cse::AdvDataMap data_map;
  data_map.insert("key", 80.2);
  data_map.insert("key_two", std::string("value"));

  SECTION("Success .at method") {
    auto val = data_map.at<double>("key");
    CHECK(val == 80.2);
  }

  SECTION(".at method wrong key specified") {
    REQUIRE_FALSE(data_map.contains("key_three"));
  }
}

TEST_CASE("DataMap get method Test") {
  cse::AdvDataMap data_map;
  data_map.insert("key", 80.2);
  auto val = data_map.get<double>("key");
  REQUIRE(val == 80.2);
}

TEST_CASE("DataMap erase method Test") {
  cse::AdvDataMap data_map;
  data_map.insert("key", 80.2);
  data_map.erase("key");
  REQUIRE(data_map.empty());
  data_map.insert("key2", 80.2);
  REQUIRE(data_map.size() == 1);
  data_map.erase("key2");
  REQUIRE(data_map.empty());
}

TEST_CASE("DataMap clear method Test") {
  cse::AdvDataMap data_map;
  data_map.insert("key", 80.2);
  data_map.clear();
  REQUIRE(data_map.empty());

  data_map.insert("key2", "GVBHJN");
  data_map.insert("key3", 80);
  data_map.clear();
  REQUIRE(data_map.empty());
}

TEST_CASE("DataMap size method Test") {
  cse::AdvDataMap data_map;
  data_map.insert("key", 80.2);
  REQUIRE(data_map.size() == 1);
  data_map.insert("key2", 80.1);
  data_map.insert("key3", "FGHJ");
  data_map.insert("key4", 80);
  REQUIRE(data_map.size() == 4);
}

TEST_CASE("DataMap empty method Test") {
  cse::AdvDataMap data_map;
  data_map.insert("key", 80.2);
  REQUIRE_FALSE(data_map.empty() == 1);

  data_map.clear();
  REQUIRE(data_map.empty() == 1);
}

TEST_CASE("DataMap count method Test") {
  cse::AdvDataMap data_map;
  data_map.insert("key", 80.2);
  REQUIRE(data_map.count("key") == 1);
  data_map.insert("key2", 33);
  data_map.insert("key3", "sss");
  data_map.insert("key4", "30.34");
  data_map.insert("key5", 11.12);
  REQUIRE(data_map.count("key4") == 1);
  data_map.insert("key6", "sds");
  REQUIRE(data_map.count("key2") == 1);
}

TEST_CASE("DataMap to_string method Test") {
  cse::AdvDataMap data_map;
  data_map.insert("key", 80.2);
  data_map.insert("name", std::string("professor"));

  auto val = data_map.to_string<double>("key");

  SECTION("Convert double to string") {
    REQUIRE(data_map.to_string<double>("key") == "80.200000");
  }

  SECTION("Return string value") {
    REQUIRE(data_map.to_string<std::string>("name") == "professor");
  }
}

TEST_CASE("Test Different Data Types in DataMap") {
  cse::AdvDataMap data_map;
  data_map.insert("int", 100);
  data_map.insert("double", 10.5);
  data_map.insert("string", std::string("test"));
  data_map.insert("bool", true);

  REQUIRE(data_map.get<int>("int") == 100);
  REQUIRE(data_map.get<double>("double") == 10.5);
  REQUIRE(data_map.get<std::string>("string") == "test");
  REQUIRE(data_map.get<bool>("bool") == true);
}

TEST_CASE("DataMap is_conv_to_string method Tests") {
  cse::AdvDataMap data_map;
  std::vector<std::string> strings = {"test", "test2", "test3"};
  std::vector<double> doubles = {4.4, 2.5, 6.1};
  data_map.insert("string_key", std::string("Hello"));
  data_map.insert("vec0", strings[0]);
  data_map.insert("vec2", strings[2]);
  data_map.insert("vec01", doubles[0]);
  data_map.insert("vec21", doubles[2]);
  data_map.insert("int_key", 42);

  SECTION("Check if value can be converted to string, and for vector indices") {
    REQUIRE(data_map.is_conv_to_string("string_key"));
    REQUIRE(data_map.is_conv_to_string("vec0"));
    REQUIRE(data_map.is_conv_to_string("vec2"));
    REQUIRE_FALSE(data_map.is_conv_to_string("vec01"));
    REQUIRE_FALSE(data_map.is_conv_to_string("vec21"));
    REQUIRE_FALSE(data_map.is_conv_to_string("int_key"));
  }
}

TEST_CASE("DataMap is_numeric method Tests") {
  cse::AdvDataMap data_map;
  data_map.insert("int_key", 42);
  data_map.insert("double_key", 3.14);
  data_map.insert("string_key", std::string("Hello"));

  SECTION("Check if value is numeric") {
    REQUIRE(data_map.is_numeric("int_key"));
    REQUIRE(data_map.is_numeric("double_key"));
    REQUIRE_FALSE(data_map.is_numeric("string_key"));
  }
}

TEST_CASE("DataMap get_type_keys method Tests") {
  cse::AdvDataMap data_map;
  data_map.insert("key1", 100);
  data_map.insert("key0", std::string("Test_key_0"));
  data_map.insert("key2", 101);

  SECTION("Check if all keys are returned for int") {
    std::vector<std::string> vector1 = data_map.get_type_keys<int>();
    REQUIRE(std::size(vector1) == 2);
  }
  SECTION("Check if all keys are returned for string") {
    data_map.insert("key3", std::string("Test_key_3"));
    data_map.insert("key4", std::string("Test_key_4"));
    std::vector<std::string> vector2 = data_map.get_type_keys<std::string>();
    REQUIRE(std::size(vector2) == 3);
  }
  SECTION("Check both at once") {
      ///Should be string keys for key0, key3, key4, key5, key6
      ///Shouuld be int keys for key1 and key2
    data_map.insert("key3", std::string("Test_key_3"));
    data_map.insert("key4", std::string("Test_key_4"));
    data_map.insert("key5", std::string("Test_key_5"));
    data_map.insert("key6", std::string("Test_key_6"));
    std::vector<std::string> vector21 = data_map.get_type_keys<std::string>();
    std::vector<std::string> vector23 = data_map.get_type_keys<int>();
    REQUIRE(std::size(vector21) == 5);
    REQUIRE(std::size(vector23) == 2);
  }
  SECTION("Clear and check if empty") {
    data_map.clear();
    std::vector<std::string> vector22 = data_map.get_type_keys<std::string>();
    REQUIRE(std::size(vector22) == 0);
    REQUIRE(data_map.empty() == true);
  }
}

TEST_CASE("Test mem_size for multiple types") {
  cse::AdvDataMap data_map;
  data_map.insert("key1", 100);
  data_map.insert("key0", std::string("Test_key_0"));
  data_map.insert("key2", 10.10);

  SECTION("Check memory size for int, string, and double") {
    size_t int_size = data_map.get_mem_size<int>("key1");
    size_t string_size = data_map.get_mem_size<std::string>("key0");
    size_t double_size = data_map.get_mem_size<double>("key2");
    REQUIRE(int_size == sizeof(data_map.get<int>("key1")));
    REQUIRE(string_size == sizeof(data_map.get<std::string>("key0")));
    REQUIRE(double_size == sizeof(data_map.get<double>("key2")));
  }
}

TEST_CASE("Test types for multiple keys") {
  cse::AdvDataMap data_map;
  data_map.insert("key1", 100);
  data_map.insert("key0", std::string("Test_key_0"));
  data_map.insert("key2", 10.10);

  SECTION("Check keys with the type they should be") {
    REQUIRE(data_map.is_type<int>("key1"));
    REQUIRE(data_map.is_type<std::string>("key0"));
    REQUIRE(data_map.is_type<double>("key2"));
  }
}

TEST_CASE("Testing using an empty string as a key") {
  cse::AdvDataMap data_map;
  data_map.insert("", 123);

  REQUIRE(data_map.to_string<int>("") == std::to_string(123));

  REQUIRE(data_map.contains(""));
  data_map.clear();

  data_map.insert("", 44.32);
  REQUIRE(data_map.to_string<double>("") == std::to_string(44.32));

}