
#define CATCH_CONFIG_MAIN
#include "../../Catch/single_include/catch2/catch.hpp"

#include <vector>
#include <DataTracker.h>

cse::DataTracker tracker;

TEST_CASE("DataTrackerTest 1", "Empty") {
    CHECK(tracker.total() == 0);
    CHECK(tracker.mean() == 0.0);
    CHECK(tracker.median() == 0.0);
    CHECK(tracker.min() == 0.0);
    CHECK(tracker.max() == 0.0);
}

TEST_CASE("DataTrackerTest 2", "Add") {
    tracker.add_value(10.0);
    CHECK(tracker.total() == 1);
    CHECK(tracker.mean() == 10.0);
    CHECK(tracker.median() == 10.0);
    CHECK(tracker.min() == 10.0);
    CHECK(tracker.max() == 10.0);
}

TEST_CASE("DataTracker 3", "Multiple") {
    tracker.add_value(5.0);
    tracker.add_value(10.0);
    tracker.add_value(15.0);
    CHECK(tracker.total() == 3);
    CHECK(tracker.mean() == 10.0);
    CHECK(tracker.median() == 10.0);
    CHECK(tracker.min() == 5.0);
    CHECK(tracker.max() == 15.0);
}

TEST_CASE("DataTracker 4", "delete") {
    tracker.add_value(5.0);
    tracker.add_value(10.0);
    tracker.add_value(15.0);
    tracker.delete_value(10.0);
    CHECK(tracker.total() == 2);
    CHECK(tracker.mean() == 10.0);
    CHECK(tracker.median() == 10.0);
    CHECK(tracker.min() == 5.0);
    CHECK(tracker.max() == 15.0);
}


TEST_CASE("DataTracker 5", "even number") {
    tracker.add_value(1.0);
    tracker.add_value(3.0);
    tracker.add_value(2.0);
    tracker.add_value(4.0);
    CHECK(tracker.median() == 2.5);
}
