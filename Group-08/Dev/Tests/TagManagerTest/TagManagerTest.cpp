/**
 * @class   TagMangerTest
 * @file    TagManagerTest.cpp
 * @author  Kelsi Elliott
 * @brief   Tests the TagManager class 
 */

#include "../../../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../TagManager/TagManager.h"

TEST_CASE("Test addTag", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("school", "urgent");
    tm.addTag("school", "low");
    tm.addTag("school", "medium");
    tm.addTag("work", "low");
    tm.addTag("personal", "medium");


    CHECK(tm.getTags("school").size() == 3);
    CHECK(tm.getTags("work").size() == 1);
    CHECK(tm.getTags("personal").size() == 1);
    CHECK(tm.getTags("CSE498").empty());

}

TEST_CASE("Test removeTag", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("CSE498", "urgent");
    tm.addTag("CSE498", "work");

    tm.removeTag("CSE498", "work");

    CHECK(tm.getTags("CSE498").size() == 1);

    tm.removeTag("CSE498", "no");
    CHECK(tm.getTags("CSE498").size() == 1);
}

TEST_CASE("Test getTag", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("CSE498", "urgent");
    tm.addTag("CSE498", "work");

    auto tags = tm.getTags("CSE498");
    CHECK(tags.size() == 2);
    CHECK(tags.count("urgent") == 1);
    CHECK(tags.count("work") == 1);

    tags = tm.getTags("no");
    CHECK(tags.empty());
}

TEST_CASE("Test getTaskTags", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("CSE498", "urgent");
    tm.addTag("CSE498", "work");
    tm.addTag("school", "urgent");

    auto tasks = tm.getTaskTags("urgent");
    CHECK(tasks.size() == 2);
    CHECK(tasks.count("CSE498") == 1);
    CHECK(tasks.count("school") == 1);
}

TEST_CASE("Test clearTagsForTask", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("CSE498", "urgent");
    tm.addTag("CSE498", "work");
    tm.addTag("school", "urgent");

    CHECK(tm.getTags("CSE498").size()== 2);
    auto tasks = tm.getTags("CSE498");
    CHECK(tasks.count("urgent") == 1);
    CHECK(tasks.count("work") == 1);
    CHECK(tm.getTags("school").size() == 1);

    tm.clearTagsForTask("CSE498");

    CHECK(tm.getTags("CSE498").empty());
    CHECK(tm.getTags("school").size() == 1);
}

TEST_CASE("Test clearTags", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("CSE498", "urgent");
    tm.addTag("CSE498", "work");
    tm.addTag("school", "urgent");

    tm.clearTags("urgent");

    CHECK(tm.getTags("school").empty());
    CHECK(tm.getTags("CSE498").size() == 1);
}

TEST_CASE("Test hasTag", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("CSE498", "urgent");

    CHECK(tm.hasTag("CSE498", "urgent"));
    CHECK_FALSE(tm.hasTag("CSE498", "no"));
    CHECK_FALSE(tm.hasTag("school", "urgent"));
}

TEST_CASE("Test removing a tag removes it from task", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("CSE498", "urgent");
    tm.addTag("CSE498", "work");

    tm.removeTag("CSE498", "urgent");

    CHECK_FALSE(tm.hasTag("CSE498", "urgent"));
    CHECK(tm.hasTag("CSE498", "work"));
}

TEST_CASE("Test clearTagsForTask remove all associations", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("CSE498", "urgent");
    tm.addTag("CSE498", "work");

    tm.clearTagsForTask("CSE498");

    CHECK_FALSE(tm.hasTag("CSE498", "urgent"));
    CHECK_FALSE(tm.hasTag("CSE498", "work"));
}

TEST_CASE("Test clearTags remove all associations", "[TagManager]") {
    cse::TagManager tm;

    tm.addTag("CSE498", "urgent");
    tm.addTag("CSE498", "work");
    tm.addTag("school", "urgent");

    tm.clearTags("urgent");

    CHECK(tm.hasTag("CSE498", "work"));
    CHECK_FALSE(tm.hasTag("CSE498", "urgent"));
    CHECK_FALSE(tm.hasTag("school", "urgent"));
}


