/**
 * @file DataMapTest.cpp
 * @author Rahul B
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include <cse/DataMap.h>

TEST_CASE("DataMap Default Constructor Test", "[DataMap]") {
  cse::DataMap data_map;
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
  cse::DataMap data_map = {
    {"name", std::string("rahul")},
    {"age", 22},
    {"dob", std::string("1209")}
  };

  SECTION("DataMap is not empty") {
    bool y = data_map.empty();
    REQUIRE(y == 0);
  }

  SECTION("Valid DataMap keys") {
    REQUIRE(data_map.count("name") == 1);
  }
}

TEST_CASE("DataMap Copy Constructor Tests", "[DataMap]") {
  cse::DataMap data_map = {
      {"name", std::string("rahul")},
      {"age", 22},
      {"dob", std::string("1209")}
  };

  cse::DataMap copy(data_map);

  SECTION("Copy contains same key values as DataMap") {
    REQUIRE(copy.contains("name"));
    REQUIRE(copy.contains("age"));
    REQUIRE(copy.contains("dob"));

    std::string n = data_map.get<std::string>("name");
    int val = data_map.get<int>("age");
    std::string d = data_map.get<std::string>("dob");

    REQUIRE(n == "rahul");
    REQUIRE(val == 22);
    REQUIRE(d == "1209");
  }

  SECTION("Change in DataMap should not change copy") {
    data_map.insert("new", 10);
    REQUIRE_FALSE(copy.contains("new"));
  }
}

TEST_CASE("DataMap Copy Assignment Tests") {
  cse::DataMap data_map = {
      {"name", std::string("rahul")},
      {"age", 22},
      {"dob", std::string("1209")}
  };

  cse::DataMap copy;
  copy = data_map;

  SECTION("Copy contains same key values as DataMap") {
    REQUIRE(copy.contains("name"));
    REQUIRE(copy.contains("age"));
    REQUIRE(copy.contains("dob"));

    std::string n = data_map.get<std::string>("name");
    int val = data_map.get<int>("age");
    std::string d = data_map.get<std::string>("dob");

    REQUIRE(n == "rahul");
    REQUIRE(val == 22);
    REQUIRE(d == "1209");
  }

  SECTION("Change in DataMap should not change copy") {
    data_map.insert("new", 10);
    REQUIRE_FALSE(copy.contains("new"));
  }

}

TEST_CASE("DataMap Insert Method Tests", "[DataMap]") {
  cse::DataMap data_map;

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
  cse::DataMap data_map;
  data_map["joe"] = 80;
  int val = std::any_cast<int>(data_map["joe"]);
  REQUIRE(val == 80);

  data_map["bob"];
  auto value = std::any_cast<int>(data_map["bob"]);
  REQUIRE(value == 0);
}

TEST_CASE("DataMap .at method Tests","[DataMap]") {
  cse::DataMap data_map;
  data_map.insert("key", 80.2);
  data_map.insert("joe", std::string("value"));

  SECTION("Success .at method") {
    auto val = data_map.at<double>("key");
    CHECK(val == 80.2);
  }

  SECTION(".at method wrong key specified") {
    REQUIRE_FALSE(data_map.contains("bob"));
  }
}

TEST_CASE("DataMap get method Test") {
  cse::DataMap data_map;
  data_map.insert("key", 80.2);
  auto val = data_map.get<double>("key");
  REQUIRE(val == 80.2);
}

TEST_CASE("DataMap erase method Test") {
  cse::DataMap data_map;
  data_map.insert("key", 80.2);
  data_map.erase("key");
  REQUIRE(data_map.empty() == 1);
}

TEST_CASE("DataMap clear method Test") {
  cse::DataMap data_map;
  data_map.insert("key", 80.2);
  data_map.clear();
  REQUIRE(data_map.empty() == 1);
}

TEST_CASE("DataMap size method Test") {
  cse::DataMap data_map;
  data_map.insert("key", 80.2);
  REQUIRE(data_map.size() == 1);
}

TEST_CASE("DataMap empty method Test") {
  cse::DataMap data_map;
  data_map.insert("key", 80.2);
  REQUIRE_FALSE(data_map.empty() == 1);

  data_map.clear();
  REQUIRE(data_map.empty() == 1);
}

TEST_CASE("DataMap count method Test") {
  cse::DataMap data_map;
  data_map.insert("key", 80.2);
  REQUIRE(data_map.count("key") == 1);
}
