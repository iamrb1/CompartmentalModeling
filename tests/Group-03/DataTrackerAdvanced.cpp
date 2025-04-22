#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../Group-03/src/DataTracker.h"
#include <vector>
#define CATCH_CONFIG_MAIN

TEST_CASE("Advanced DataTracker Mode", "[DataTracker]") {
    cse::DataTracker<int> tracker;

    CHECK(tracker.mode() == 0);

    tracker.add_value(1);
    tracker.add_value(2);
    tracker.add_value(2);
    tracker.add_value(3);
    CHECK(tracker.mode() == 2);

    tracker.add_value(3);
    CHECK((tracker.mode() == 2 || tracker.mode() == 3)); // Handle multiple modes

    tracker.add_value(-4);
    tracker.add_value(-4);
    tracker.add_value(-4);

    CHECK(tracker.mode() == -4);

}

TEST_CASE("Advanced DataTracker Variance", "[DataTracker]") {
    cse::DataTracker<double> tracker;
    tracker.add_value(2.0);
    tracker.add_value(4.0);
    tracker.add_value(4.0);
    tracker.add_value(4.0);
    tracker.add_value(5.0);
    tracker.add_value(5.0);
    tracker.add_value(7.0);
    tracker.add_value(9.0);
    CHECK(tracker.variance() == Approx(4.0));
}



TEST_CASE("Advanced DataTracker Prey-Predator Win Condition", "[DataTracker]") {
    cse::DataTracker<int> tracker;
    for (int i = 0; i < 81; ++i) tracker.add_value(1); // 81% prey
    for (int i = 0; i < 19; ++i) tracker.add_value(2); // 19% predator
    CHECK(tracker.winner() == 1); // indicates the winner was the # 1

}

TEST_CASE("Advanced DataTracker Prey-Predator Win Condition (empty)", "[DataTracker]") {
    cse::DataTracker<int> tracker;
    CHECK(tracker.winner() == std::nullopt);} // indicates no winner was found
