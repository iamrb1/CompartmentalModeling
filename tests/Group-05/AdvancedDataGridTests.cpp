#include "../../Group-05/src/DataGrid.cpp"
#include "../../Group-05/src/DataGrid.h"
#include "../../Group-05/src/Datum.cpp"
#include "../../Group-05/src/Datum.h"
#include "../../Group-05/src/ReferenceVector.h"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream> //added this include

using cse::DataGrid;

TEST_CASE("DataGrid: Merge", "[merge]]") {
    DataGrid grid1(2, 3); 
    DataGrid grid2(2, 3);             // Copy constructor
    new_grid= mergeGrid(grid1, grid2, true);
    CHECK(new_grid.shape() == std::make_tuple(2, 3));
    new_grid=mergeGrid(grid1, grid2, false);
    CHECK(new_grid.shape() == std::make_tuple(2, 3));

  }

  