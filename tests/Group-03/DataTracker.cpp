#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-03/src/DataTracker.h"

#include <string>
#include <limits>
#include <vector>

TEST_CASE("DataTracker Empty", "[DataTracker]") {
    cse::DataTracker<int> tracker;
    CHECK(tracker.total() == 0);
    CHECK(tracker.mean() == 0.0);
    CHECK(tracker.median() == 0.0);
    CHECK(tracker.min() == 0);
    CHECK(tracker.max() == 0);
    CHECK(tracker.mode() == 0);
}

TEST_CASE("DataTracker Add Integers", "[DataTracker]") {
    cse::DataTracker<int> tracker;
    tracker.add_value(10);
    CHECK(tracker.total() == 1);
    CHECK(tracker.mean() == 10.0);
    CHECK(tracker.median() == 10.0);
    CHECK(tracker.min() == 10);
    CHECK(tracker.max() == 10);
    CHECK(tracker.mode() == 10);
}

TEST_CASE("DataTracker Add Non-Nice Doubles", "[DataTracker]") {
    cse::DataTracker<double> tracker;
    std::vector<double> vals = {-1000.33, 500.5, 100.25, -1000.33, 400.2};
    for (double val : vals) tracker.add_value(val);
    CHECK(tracker.total() == vals.size());
    CHECK(tracker.min() == -1000.33);
    CHECK(tracker.max() == 500.5);
    CHECK(tracker.mode() == -1000.33);
    CHECK(tracker.mean() == Approx(-199.942).epsilon(0.001));
}

TEST_CASE("DataTracker Add With Duplicates", "[DataTracker]") {
    cse::DataTracker<int> tracker;
    tracker.add_value(1);
    tracker.add_value(2);
    tracker.add_value(2);
    tracker.add_value(3);
    CHECK(tracker.mode() == 2);
}

TEST_CASE("DataTracker Median Even", "[DataTracker]") {
    cse::DataTracker<int> tracker;
    tracker.add_value(1);
    tracker.add_value(2);
    tracker.add_value(3);
    tracker.add_value(4);
    CHECK(tracker.median() == 2.5);
}

TEST_CASE("DataTracker Delete Value Edge Cases", "[DataTracker]") {
    cse::DataTracker<int> tracker;
    tracker.add_value(10);
    tracker.add_value(20);
    tracker.add_value(10);
    CHECK(tracker.delete_value(10)); // Deletes one 10
    CHECK(tracker.total() == 2);
    CHECK(tracker.delete_value(10)); // Deletes the second 10
    CHECK(tracker.delete_value(10) == false); // Nothing left to delete
    CHECK(tracker.total() == 1);
    CHECK(tracker.delete_value(30) == false); // Not present
}

TEST_CASE("DataTracker Large and Small Values", "[DataTracker]") {
    cse::DataTracker<int> tracker;
    tracker.add_value(std::numeric_limits<int>::min());
    tracker.add_value(std::numeric_limits<int>::max());
    CHECK(tracker.min() == std::numeric_limits<int>::min());
    CHECK(tracker.max() == std::numeric_limits<int>::max());
}

TEST_CASE("DataTracker Unsigned Int", "[DataTracker]") {
    cse::DataTracker<unsigned int> tracker;
    tracker.add_value(0);
    tracker.add_value(100);
    CHECK(tracker.min() == 0);
    CHECK(tracker.max() == 100);
    CHECK(tracker.mean() == 50.0);
}

TEST_CASE("DataTracker Strings", "[DataTracker]") {
    cse::DataTracker<std::string> tracker;
    tracker.add_value("apple");
    tracker.add_value("banana");
    tracker.add_value("apple");
    CHECK(tracker.mode() == "apple");
    CHECK(tracker.total() == 3);
    tracker.delete_value("banana");
    CHECK(tracker.total() == 2);
    CHECK(tracker.min() == "apple");
    CHECK(tracker.max() == "apple");
}

TEST_CASE("DataTracker Median Complex Sorting", "[DataTracker]") {
    cse::DataTracker<double> tracker;
    tracker.add_value(3.5);
    tracker.add_value(7.8);
    tracker.add_value(1.2);
    tracker.add_value(6.6);
    tracker.add_value(2.1);
    CHECK(tracker.median() == Approx(3.5));
}

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


TEST_CASE("Variance with negatives", "[DataTracker]") {
    cse::DataTracker<double> tracker;
    tracker.add_value(-3.0);
    tracker.add_value(-2.0);
    tracker.add_value(-1.0);
    tracker.add_value(0.0);
    tracker.add_value(1.0);
    tracker.add_value(2.0);
    tracker.add_value(3.0);
    CHECK(tracker.variance() == Approx(4.0));
}

TEST_CASE("Variance on empty DataTracker", "[DataTracker]") {
    cse::DataTracker<double> tracker;
    CHECK(tracker.variance() == Approx(0.0));
}

TEST_CASE("Variance with single input", "[DataTracker]") {
    cse::DataTracker<double> tracker;
    tracker.add_value(42.0);
    CHECK(tracker.variance() == Approx(0.0));
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

    TEST_CASE("DataTracker has function", "[DataTracker]") {
        cse::DataTracker<int> intTracker;
        intTracker.add_value(10);
        intTracker.add_value(20);
        intTracker.add_value(30);
    
        CHECK(intTracker.has(10));
        CHECK(intTracker.has(20));
        CHECK(intTracker.has(30));
        CHECK_FALSE(intTracker.has(40));  
    
        cse::DataTracker<double> dTracker;
        dTracker.add_value(3.14);
        dTracker.add_value(2.718);
    
        CHECK(dTracker.has(3.14));
        CHECK_FALSE(dTracker.has(1.618));  
    
        cse::DataTracker<std::string> sTracker;
        sTracker.add_value("apple");
        sTracker.add_value("banana");
    
        CHECK(sTracker.has("apple"));
        CHECK(sTracker.has("banana"));
        CHECK_FALSE(sTracker.has("cherry"));  
    
        cse::DataTracker<int> emptyTracker;
        CHECK_FALSE(emptyTracker.has(0));
    }
    
