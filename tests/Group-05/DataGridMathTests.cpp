/**
 * @file DataGridMathTests.cpp
 *
 * @author Max Krawec
 */

#include "../../Group-05/src/DataGrid.cpp"
#include "../../Group-05/src/Datum.cpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

using cse::DataGrid;
using cse::Datum;

static const double kEpsilon = 0.0001;

/**
 * @brief Mathematical Tests
 */
TEST_CASE("DataGrid: Mathematical Functions", "[mathematics]") {
  // Cite: Used https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
  // to help with creating a 2D vector.
  std::vector<std::vector<Datum>> test(5, std::vector<Datum>(3));

  test[0][0] = Datum(5.0);
  test[0][1] = Datum("test1");
  test[0][2] = Datum(10.25);

  test[1][0] = Datum(3.5);
  test[1][1] = Datum("test2");
  test[1][2] = Datum("test3");

  test[2][0] = Datum(1.25);
  test[2][1] = Datum("test4");
  test[2][2] = Datum(150.50);

  test[3][0] = Datum(-15);
  test[3][1] = Datum("test5");
  test[3][2] = Datum(200);

  test[4][0] = Datum(4.25);
  test[4][1] = Datum("test6");
  test[4][2] = Datum(20.25);

  DataGrid grid(test);

  // Mean Column Tests
  CHECK_THAT(grid.columnMean(0),
  Catch::WithinAbs(-0.2, kEpsilon));
  CHECK(std::isnan(grid.columnMean(1)));
  CHECK_THAT(grid.columnMean(2),
  Catch::WithinAbs(95.25, kEpsilon));

  CHECK_THROWS(grid.columnMean(99));

  // Median Column Tests
  CHECK_THAT(grid.columnMedian(0),
  Catch::WithinAbs(3.5, kEpsilon));
  CHECK(std::isnan(grid.columnMedian(1)));
  CHECK_THAT(grid.columnMedian(2),
  Catch::WithinAbs(85.375, kEpsilon));

  CHECK_THROWS(grid.columnMedian(99));

  // Standard Deviation Column Tests
  CHECK_THAT(grid.columnStandardDeviation(0),
  Catch::WithinAbs(7.5056645275418, kEpsilon));
  CHECK(std::isnan(grid.columnStandardDeviation(1)));
  CHECK_THAT(grid.columnStandardDeviation(2),
  Catch::WithinAbs(81.968172176766, kEpsilon));

  CHECK_THROWS(grid.columnStandardDeviation(99));

  // Min Column Tests
  CHECK_THAT(grid.columnMin(0),
  Catch::WithinAbs(-15, kEpsilon));
  CHECK(std::isnan(grid.columnMax(1)));
  CHECK_THAT(grid.columnMin(2),
  Catch::WithinAbs(10.25, kEpsilon));

  CHECK_THROWS(grid.columnMin(99));

  // Max Column Tests
  CHECK_THAT(grid.columnMax(0),
  Catch::WithinAbs(5.0, kEpsilon));
  CHECK(std::isnan(grid.columnMax(1)));
  CHECK_THAT(grid.columnMax(2),
  Catch::WithinAbs(200, kEpsilon));

  CHECK_THROWS(grid.columnMax(99));

}

TEST_CASE("Mode Column Tests", "[mathematics]") {
  std::vector<std::vector<Datum>> mode_test(5, std::vector<Datum>(3));

  mode_test[0][0] = Datum(5.55);
  mode_test[0][1] = Datum("test1");
  mode_test[0][2] = Datum(10.25);

  mode_test[1][0] = Datum(5.55);
  mode_test[1][1] = Datum("test2");
  mode_test[1][2] = Datum("test3");

  mode_test[2][0] = Datum(1.25);
  mode_test[2][1] = Datum("test4");
  mode_test[2][2] = Datum(10.25);

  mode_test[3][0] = Datum(-15);
  mode_test[3][1] = Datum("test5");
  mode_test[3][2] = Datum(10.25);

  mode_test[4][0] = Datum(1.25);
  mode_test[4][1] = Datum("test6");
  mode_test[4][2] = Datum(20.25);

  DataGrid grid_mode(mode_test);

  std::vector<double> multipleModes = grid_mode.columnMode(0);
  std::sort(multipleModes.begin(), multipleModes.end());
  CHECK(multipleModes.size() == 2);
  CHECK_THAT(multipleModes[0],
             Catch::WithinAbs(1.25, kEpsilon));
  CHECK_THAT(multipleModes[1],
             Catch::WithinAbs(5.55, kEpsilon));

  CHECK(grid_mode.columnMode(1).empty());

  std::vector<double> singleMode = grid_mode.columnMode(2);
  CHECK(singleMode.size() == 1);
  CHECK_THAT(singleMode[0],
             Catch::WithinAbs(10.25, kEpsilon));

  CHECK_THROWS(grid_mode.columnMode(99));
}
