/**
 * @file ActionMapTest.cpp
 * @author Nitish Maindoliya
 */

#include <catch2/catch.hpp>
#include <cse/ActionMap.h>

TEST_CASE("ActionMap: insert, contains, erase, clear", "[ActionMap]") {
  cse::ActionMap map;

  SECTION("insert and contains") {
    REQUIRE_FALSE(map.contains("action1"));
    REQUIRE_FALSE(map.contains("action2"));

    map.insert("action1", []() {});
    REQUIRE(map.contains("action1"));
    REQUIRE_FALSE(map.contains("action2"));

    map.insert("action2", []() {});
    REQUIRE(map.contains("action1"));
    REQUIRE(map.contains("action2"));
  }

  SECTION("erase") {
    map.insert("action1", []() {});
    map.insert("action2", []() {});

    REQUIRE(map.contains("action1"));
    REQUIRE(map.contains("action2"));

    map.erase("action1");
    REQUIRE_FALSE(map.contains("action1"));
    REQUIRE(map.contains("action2"));

    map.erase("action2");
    REQUIRE_FALSE(map.contains("action1"));
    REQUIRE_FALSE(map.contains("action2"));
  }

  SECTION("clear") {
    map.insert("action1", []() {});
    map.insert("action2", []() {});

    REQUIRE(map.contains("action1"));
    REQUIRE(map.contains("action2"));

    map.clear();
    REQUIRE_FALSE(map.contains("action1"));
    REQUIRE_FALSE(map.contains("action2"));
  }
}

static int global_count = 0;
void increment_global_count() { ++global_count; }
void decrement_global_count() { --global_count; }

TEST_CASE("ActionMap: invoking actions", "[ActionMap]") {
  cse::ActionMap map;
  int count = 0;

  map.insert("increment", [&count]() { ++count; });
  map.insert("decrement", [&count]() { --count; });

  REQUIRE(count == 0);
  map.invoke("increment");
  REQUIRE(count == 1);
  map.invoke("decrement");
  REQUIRE(count == 0);

  map.insert("increment_global", increment_global_count);
  map.insert("decrement_global", decrement_global_count);

  REQUIRE(global_count == 0);
  map.invoke("increment_global");
  REQUIRE(global_count == 1);
  map.invoke("decrement_global");
  REQUIRE(global_count == 0);
}
