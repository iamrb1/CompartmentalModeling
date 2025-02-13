
#define CATCH_CONFIG_MAIN
#include "../../Catch/single_include/catch2/catch.hpp"

#include <vector>
#include <DataTracker.h>

cse::DataTracker tracker;

TEST_CASE("DataTrackerTest Empty") {
    CHECK(tracker.total() == 0);
    CHECK(tracker.mean() == 0.0);
    CHECK(tracker.median() == 0.0);
    CHECK(tracker.min() == 0.0);
    CHECK(tracker.max() == 0.0);
}

TEST_CASE("DataTrackerTest Add") {
    tracker.add_value(10.0);
    CHECK(tracker.total() == 1);
    CHECK(tracker.mean() == 10.0);
    CHECK(tracker.median() == 10.0);
    CHECK(tracker.min() == 10.0);
    CHECK(tracker.max() == 10.0);
}

TEST_CASE("DataTracker Multiple") {
    tracker.add_value(5.0);
    tracker.add_value(10.0);
    tracker.add_value(15.0);
    CHECK(tracker.total() == 3);
    CHECK(tracker.mean() == 10.0);
    CHECK(tracker.median() == 10.0);
    CHECK(tracker.min() == 5.0);
    CHECK(tracker.max() == 15.0);
}

TEST_CASE("DataTracker delete") {
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

TEST_CASE("DeleteNonExistentValue") {
    tracker.add_value(5.0);
    tracker.delete_value(10.0);
    CHECK(tracker.total() == 1);
}

TEST_CASE("MedianEvenNumberOfElements") {
    tracker.add_value(1.0);
    tracker.add_value(3.0);
    tracker.add_value(2.0);
    tracker.add_value(4.0);
    CHECK(tracker.median() == 2.5);
}
