
#define CATCH_CONFIG_MAIN
#include "../../Catch/single_include/catch2/catch.hpp"
#include <vector>
#include "DataTracker.h"

TEST_CASE("DataTrackerTest Test std::vector", "[base]")
{
  std::vector<int> v{0, 1, 2, 3, 4};

  CHECK(v.size() == 5);
}


TEST_CASE("DataTrackerTest Debug Empty", "InitiallyEmpty") {
    cse::DataTracker tracker;
    CHECK(tracker.total() == 0);
    CHECK(tracker.mean() == 0.0);
    CHECK(tracker.median() == 0.0);
    CHECK(tracker.min() == 0.0);
    CHECK(tracker.max() == 0.0);
}

// TEST_CASE("DataTrackerTest", "AddValue") {
//     cse::DataTracker tracker;
//     tracker.add_value(10.0);
//     CHECK(tracker.total() == 1);
//     CHECK(tracker.mean() == 10.0);
//     CHECK(tracker.median() == 10.0);
//     CHECK(tracker.min() == 10.0);
//     CHECK(tracker.max() == 10.0);
// }
//
// TEST_CASE("DataTrackerTest", "MultipleValues") {
//     cse::DataTracker tracker;
//     tracker.add_value(5.0);
//     tracker.add_value(10.0);
//     tracker.add_value(15.0);
//     CHECK(tracker.total() == 3);
//     CHECK(tracker.mean() == 10.0);
//     CHECK(tracker.median() == 10.0);
//     CHECK(tracker.min() == 5.0);
//     CHECK(tracker.max() == 15.0);
// }
//
// TEST_CASE("DataTrackerTest", "DeleteValue") {
//     cse::DataTracker tracker;
//     tracker.add_value(5.0);
//     tracker.add_value(10.0);
//     tracker.add_value(15.0);
//     tracker.delete_value(10.0);
//     CHECK(tracker.total() == 2);
//     CHECK(tracker.mean() == 10.0);
//     CHECK(tracker.median() == 10.0);
//     CHECK(tracker.min() == 5.0);
//     CHECK(tracker.max() == 15.0);
// }
//
// TEST_CASE("DataTrackerTest", "DeleteNonExistentValue") {
//     cse::DataTracker tracker;
//     tracker.add_value(5.0);
//     tracker.delete_value(10.0);
//     CHECK(tracker.total() == 1);
// }
//
// TEST_CASE("DataTrackerTest", "MedianEvenNumberOfElements") {
//     cse::DataTracker tracker;
//     tracker.add_value(1.0);
//     tracker.add_value(3.0);
//     tracker.add_value(2.0);
//     tracker.add_value(4.0);
//     CHECK(tracker.median() == 2.5);
// }
