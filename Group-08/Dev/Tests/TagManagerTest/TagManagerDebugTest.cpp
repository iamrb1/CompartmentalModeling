#include "../../../../third-party/Catch/single_include/catch2/catch.hpp"
#include "TagManager.h"


TEST_CASE("Test AddTag", "[TagManager]") {
    TagManager tm;

    tm.AddTag("school", "urgent");
    tm.AddTag("school", "low");
    tm.AddTag("school", "medium");
    tm.AddTag("work", "low");
    tm.AddTag("personal", "medium");


    CHECK(tm.GetTags("school").size() == 3);
    CHECK(tm.GetTags("work").size() == 1);
    CHECK(tm.GetTags("personal").size() == 1);
    CHECK(tm.GetTags("CSE498").empty());

}

TEST_CASE("Test RemoveTag", "[TagManager]") {
    TagManager tm;

    tm.AddTag("CSE498", "urgent");
    tm.AddTag("CSE498", "work");

    tm.RemoveTag("CSE498", "work");

    CHECK(tm.GetTags("CSE498").size() == 1);

    tm.RemoveTag("CSE498", "no");
    CHECK(tm.GetTags("CSE498").size() == 1);
}

TEST_CASE("Test GetTag", "[TagManager]") {
    TagManager tm;

    tm.AddTag("CSE498", "urgent");
    tm.AddTag("CSE498", "work");

    auto tags = tm.GetTags("CSE498");
    CHECK(tags.size() == 2);
    CHECK(tags.count("urgent") == 1);
    CHECK(tags.count("work") == 1);

    tags = tm.GetTags("no");
    CHECK(tags.empty());
}

TEST_CASE("Test GetTaskTags", "[TagManager]") {
    TagManager tm;

    tm.AddTag("CSE498", "urgent");
    tm.AddTag("CSE498", "work");
    tm.AddTag("school", "urgent");

    auto tasks = tm.GetTaskTags("urgent");
    CHECK(tasks.size() == 2);
    CHECK(tasks.count("CSE498") == 1);
    CHECK(tasks.count("school") == 1);
}

TEST_CASE("Test ClearTagForTask", "[TagManager]") {
    TagManager tm;

    tm.AddTag("CSE498", "urgent");
    tm.AddTag("CSE498", "work");
    tm.AddTag("school", "urgent");

    tm.ClearTagsForTask("CSE498");

    CHECK(tm.GetTags("CSE498").empty());
    CHECK(tm.GetTags("school").size() == 1);
}

TEST_CASE("Test ClearTags", "[TagManager]") {
    TagManager tm;

    tm.AddTag("CSE498", "urgent");
    tm.AddTag("CSE498", "work");
    tm.AddTag("school", "urgent");

    tm.ClearTags("urgent");

    CHECK(tm.GetTags("school").empty());
    CHECK(tm.GetTags("CSE498").size() == 1);
}

TEST_CASE("Test HasTag", "[TagManager]") {
    TagManager tm;

    tm.AddTag("CSE498", "urgent");

    CHECK(tm.HasTag("CSE498", "urgent"));
    CHECK_FALSE(tm.HasTag("CSE498", "no"));
    CHECK_FALSE(tm.HasTag("school", "urgent"));
}

TEST_CASE("Test removing a tag removes it from task", "[TagManager]") {
    TagManager tm;

    tm.AddTag("CSE498", "urgent");
    tm.AddTag("CSE498", "work");

    tm.RemoveTag("CSE498", "urgent");

    CHECK_FALSE(tm.HasTag("CSE498", "urgent"));
    CHECK(tm.HasTag("CSE498", "work"));
}

TEST_CASE("Test ClearTagsForTask remove all associations", "[TagManager]") {
    TagManager tm;

    tm.AddTag("CSE498", "urgent");
    tm.AddTag("CSE498", "work");

    tm.ClearTagsForTask("CSE498");

    CHECK_FALSE(tm.HasTag("CSE498", "urgent"));
    CHECK_FALSE(tm.HasTag("CSE498", "work"));
}

TEST_CASE("Test ClearTags remove all associations", "[TagManager]") {
    TagManager tm;

    tm.AddTag("CSE498", "urgent");
    tm.AddTag("CSE498", "work");
    tm.AddTag("school", "urgent");

    tm.ClearTags("urgent");

    CHECK(tm.HasTag("CSE498", "work"));
    CHECK_FALSE(tm.HasTag("CSE498", "urgent"));
    CHECK_FALSE(tm.HasTag("school", "urgent"));
}


