/**
 * @file DataGridTests.cpp
 * @authors Shahaab Ali and Max Krawec
 */

// Made with the help of ChatGPT

#include "../../Group-05/src/DataGrid.cpp"
#include "../../Group-05/src/Datum.cpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

using cse::DataGrid;
using cse::Datum;

static constexpr double kEpsilon = 0.0001;

/**
 * @brief Test parameterized constructor
 */
TEST_CASE("DataGrid: Parameterized Constructor", "[constructor]") {
  // Test with default double value
  DataGrid grid1(3, 4, 2.5);
  auto [rows1, cols1] = grid1.shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 4);
  CHECK(grid1.getValue(0, 0).GetDouble() == 2.5);
  CHECK(grid1.getValue(2, 3).GetDouble() == 2.5);

  // Test with default string value
  DataGrid grid2(2, 3, "Hello");
  auto [rows2, cols2] = grid2.shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 3);
  CHECK(grid2.getValue(0, 0).GetString() == "Hello");
  CHECK(grid2.getValue(1, 2).GetString() == "Hello");

  // Test with an empty grid
  DataGrid grid3(0, 0);
  auto [rows3, cols3] = grid3.shape();
  CHECK(rows3 == 0);
  CHECK(cols3 == 0);

  // Test if all values are correctly initialized in a large grid
  DataGrid grid4(5, 5, 99.9);
  auto [rows4, cols4] = grid4.shape();
  CHECK(rows4 == 5);
  CHECK(cols4 == 5);
  for (size_t i = 0; i < rows4; i++) {
    for (size_t j = 0; j < cols4; j++) {
      CHECK(grid4.getValue(i, j).GetDouble() == 99.9);
    }
  }
}

/**
 * @brief Test copy and move constructors
 */
TEST_CASE("DataGrid: Copy and Move Constructors", "[constructor]") {
  // Create an original 2x3 grid with initialized values
  DataGrid grid1(2, 3, 7.5);
  auto [rows1, cols1] = grid1.shape();
  CHECK(rows1 == 2);
  CHECK(cols1 == 3);
  CHECK(grid1.getValue(0, 0).GetDouble() == 7.5);
  CHECK(grid1.getValue(1, 2).GetDouble() == 7.5);

  // Copy constructor test
  DataGrid grid2(grid1);
  auto [rows2, cols2] = grid2.shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 3);
  CHECK(grid2.getValue(0, 0).GetDouble() == 7.5);
  CHECK(grid2.getValue(1, 2).GetDouble() == 7.5);

  // Ensure modifying the copy does not affect the original
  grid2.at(0, 0) = Datum(99.9);
  CHECK(grid1.getValue(0, 0).GetDouble() == 7.5);
  CHECK(grid2.getValue(0, 0).GetDouble() == 99.9);

  // Move constructor test
  DataGrid grid3(std::move(grid1));
  auto [rows3, cols3] = grid3.shape();
  CHECK(rows3 == 2);
  CHECK(cols3 == 3);
  CHECK(grid3.getValue(0, 0).GetDouble() == 7.5);
  CHECK(grid3.getValue(1, 2).GetDouble() == 7.5);

  // The moved-from grid1 should be in a valid but empty state
  auto [rows1_after_move, cols1_after_move] = grid1.shape();
  CHECK(rows1_after_move == 0);
  CHECK(cols1_after_move == 0);
}

/**
 * @brief Test inserting default rows
 */
TEST_CASE("DataGrid: Insert Default Row", "[insert]") {
  // Create a 2x3 grid with default values
  DataGrid grid(2, 3, 1.0);
  auto [rows1, cols1] = grid.shape();
  CHECK(rows1 == 2);
  CHECK(cols1 == 3);
  CHECK(grid.getValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.getValue(1, 2).GetDouble() == 1.0);

  // Insert a default row in the middle with value 5.0
  grid.insertDefaultRow(1, 5.0);
  auto [rows2, cols2] = grid.shape();
  CHECK(rows2 == 3);
  CHECK(cols2 == 3);

  // Ensure the inserted row has the correct values
  CHECK(grid.getValue(1, 0).GetDouble() == 5.0);
  CHECK(grid.getValue(1, 1).GetDouble() == 5.0);
  CHECK(grid.getValue(1, 2).GetDouble() == 5.0);

  // Ensure previous rows remain unchanged
  CHECK(grid.getValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.getValue(2, 2).GetDouble() == 1.0);

  // Insert a new row at the beginning
  grid.insertDefaultRow(0, 9.9);
  auto [rows3, cols3] = grid.shape();
  CHECK(rows3 == 4);
  CHECK(cols3 == 3);
  CHECK(grid.getValue(0, 0).GetDouble() == 9.9);
  CHECK(grid.getValue(0, 2).GetDouble() == 9.9);

  // Insert a new row at the end
  grid.insertDefaultRow(4, 3.3);
  auto [rows4, cols4] = grid.shape();
  CHECK(rows4 == 5);
  CHECK(cols4 == 3);
  CHECK(grid.getValue(4, 0).GetDouble() == 3.3);
  CHECK(grid.getValue(4, 2).GetDouble() == 3.3);
}

/**
 * @brief Test inserting default columns
 */
TEST_CASE("DataGrid: Insert Default Column", "[insert]") {
  // Create a 3x2 grid with default value 1.0
  DataGrid grid(3, 2, 1.0);
  auto [rows1, cols1] = grid.shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 2);
  CHECK(grid.getValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.getValue(2, 1).GetDouble() == 1.0);

  // Insert a default column in the middle with value 7.5
  grid.insertDefaultColumn(1, 7.5);
  auto [rows2, cols2] = grid.shape();
  CHECK(rows2 == 3);
  CHECK(cols2 == 3);

  // Ensure the inserted column has the correct values
  CHECK(grid.getValue(0, 1).GetDouble() == 7.5);
  CHECK(grid.getValue(1, 1).GetDouble() == 7.5);
  CHECK(grid.getValue(2, 1).GetDouble() == 7.5);

  // Ensure previous columns remain unchanged
  CHECK(grid.getValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.getValue(2, 2).GetDouble() == 1.0);

  // Insert a new column at the beginning
  grid.insertDefaultColumn(0, 9.9);
  auto [rows3, cols3] = grid.shape();
  CHECK(rows3 == 3);
  CHECK(cols3 == 4);
  CHECK(grid.getValue(0, 0).GetDouble() == 9.9);
  CHECK(grid.getValue(2, 0).GetDouble() == 9.9);

  // Insert a new column at the end
  grid.insertDefaultColumn(4, 3.3);
  auto [rows4, cols4] = grid.shape();
  CHECK(rows4 == 3);
  CHECK(cols4 == 5);
  CHECK(grid.getValue(0, 4).GetDouble() == 3.3);
  CHECK(grid.getValue(2, 4).GetDouble() == 3.3);
}

/**
 * @brief Test inserting premade rows
 */
TEST_CASE("DataGrid: Insert Premade Row", "[insert]") {
  // Create an empty grid and insert a row
  DataGrid grid;
  std::vector<Datum> first_row = {Datum(1.1), Datum(2.2), Datum(3.3)};
  grid.insertRow(first_row, 0);
  auto [rows1, cols1] = grid.shape();
  CHECK(rows1 == 1);
  CHECK(cols1 == 3);
  CHECK(grid.getValue(0, 0).GetDouble() == 1.1);
  CHECK(grid.getValue(0, 2).GetDouble() == 3.3);

  // Insert at the beginning (index 0)
  std::vector<Datum> new_row = {Datum(9.9), Datum(8.8), Datum(7.7)};
  grid.insertRow(new_row, 0);
  auto [rows2, cols2] = grid.shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 3);
  CHECK(grid.getValue(0, 0).GetDouble() == 9.9);
  CHECK(grid.getValue(1, 0).GetDouble() == 1.1);

  // Insert at the last index (append at end)
  std::vector<Datum> last_row = {Datum(4.4), Datum(5.5), Datum(6.6)};
  grid.insertRow(last_row, 2);
  auto [rows3, cols3] = grid.shape();
  CHECK(rows3 == 3);
  CHECK(cols3 == 3);
  CHECK(grid.getValue(2, 0).GetDouble() == 4.4);
  CHECK(grid.getValue(2, 2).GetDouble() == 6.6);
}

/**
 * @brief Test inserting premade columns
 */
TEST_CASE("DataGrid: Insert Premade Column", "[insert]") {
  // Create an empty grid and insert a column
  DataGrid grid;
  std::vector<Datum> first_column = {Datum(1.1), Datum(2.2), Datum(3.3)};
  grid.insertColumn(first_column, 0);
  auto [rows1, cols1] = grid.shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 1);
  CHECK(grid.getValue(0, 0).GetDouble() == 1.1);
  CHECK(grid.getValue(2, 0).GetDouble() == 3.3);

  // Insert at the beginning (index 0)
  std::vector<Datum> new_column = {Datum(9.9), Datum(8.8), Datum(7.7)};
  grid.insertColumn(new_column, 0);
  auto [rows2, cols2] = grid.shape();
  CHECK(rows2 == 3);
  CHECK(cols2 == 2);
  CHECK(grid.getValue(0, 0).GetDouble() == 9.9);
  CHECK(grid.getValue(0, 1).GetDouble() == 1.1);

  // Insert at the last index (append at end)
  std::vector<Datum> last_column = {Datum(4.4), Datum(5.5), Datum(6.6)};
  grid.insertColumn(last_column, 2);
  auto [rows3, cols3] = grid.shape();
  CHECK(rows3 == 3);
  CHECK(cols3 == 3);
  CHECK(grid.getValue(0, 2).GetDouble() == 4.4);
  CHECK(grid.getValue(2, 2).GetDouble() == 6.6);
}

/**
 * @brief Test deleting rows
 */
TEST_CASE("DataGrid: Delete Row", "[delete]") {
  // Create a 3x3 grid with unique values for tracking
  DataGrid grid({{Datum(1), Datum(2), Datum(3)},
                 {Datum(4), Datum(5), Datum(6)},
                 {Datum(7), Datum(8), Datum(9)}});

  auto [rows1, cols1] = grid.shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 3);

  // Delete the middle row
  grid.deleteRow(1);
  auto [rows2, cols2] = grid.shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 3);

  // Ensure the remaining rows are correctly shifted
  CHECK(grid.getValue(0, 0).GetDouble() == 1);
  CHECK(grid.getValue(1, 0).GetDouble() == 7);
  CHECK(grid.getValue(1, 2).GetDouble() == 9);

  // Delete the first row
  grid.deleteRow(0);
  auto [rows3, cols3] = grid.shape();
  CHECK(rows3 == 1);
  CHECK(cols3 == 3);
  CHECK(grid.getValue(0, 0).GetDouble() == 7);
  CHECK(grid.getValue(0, 2).GetDouble() == 9);

  // Delete the last remaining row
  grid.deleteRow(0);
  auto [rows4, cols4] = grid.shape();
  CHECK(rows4 == 0);
  CHECK(cols4 == 0);
}

/**
 * @brief Test deleting columns
 */
TEST_CASE("DataGrid: Delete Column", "[delete]") {
  // Create a 3x3 grid with unique values for tracking
  DataGrid grid({{Datum(1), Datum(2), Datum(3)},
                 {Datum(4), Datum(5), Datum(6)},
                 {Datum(7), Datum(8), Datum(9)}});

  auto [rows1, cols1] = grid.shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 3);

  // Delete the middle column
  grid.deleteColumn(1);
  auto [rows2, cols2] = grid.shape();
  CHECK(rows2 == 3);
  CHECK(cols2 == 2);

  // Ensure the remaining columns are correctly shifted
  CHECK(grid.getValue(0, 0).GetDouble() == 1);
  CHECK(grid.getValue(0, 1).GetDouble() == 3);
  CHECK(grid.getValue(1, 0).GetDouble() == 4);
  CHECK(grid.getValue(1, 1).GetDouble() == 6);
  CHECK(grid.getValue(2, 0).GetDouble() == 7);
  CHECK(grid.getValue(2, 1).GetDouble() == 9);

  // Delete the first column
  grid.deleteColumn(0);
  auto [rows3, cols3] = grid.shape();
  CHECK(rows3 == 3);
  CHECK(cols3 == 1);
  CHECK(grid.getValue(0, 0).GetDouble() == 3);
  CHECK(grid.getValue(1, 0).GetDouble() == 6);
  CHECK(grid.getValue(2, 0).GetDouble() == 9);

  // Delete the last remaining column
  grid.deleteColumn(0);
  auto [rows4, cols4] = grid.shape();
  CHECK(rows4 == 3);
  CHECK(cols4 == 0);
}

/**
 * @brief Test accessing row
 */
TEST_CASE("DataGrid: Get Row", "[access]") {
  // Create a 3x3 grid with unique values
  DataGrid grid({{Datum(1), Datum(2), Datum(3)},
                 {Datum(4), Datum(5), Datum(6)},
                 {Datum(7), Datum(8), Datum(9)}});

  auto [rows, cols] = grid.shape();
  CHECK(rows == 3);
  CHECK(cols == 3);

  // Access first row
  auto &row1 = grid.getRow(0);
  CHECK(row1.size() == 3);
  CHECK(row1[0].GetDouble() == 1);
  CHECK(row1[1].GetDouble() == 2);
  CHECK(row1[2].GetDouble() == 3);

  // Access middle row
  auto &row2 = grid.getRow(1);
  CHECK(row2.size() == 3);
  CHECK(row2[0].GetDouble() == 4);
  CHECK(row2[1].GetDouble() == 5);
  CHECK(row2[2].GetDouble() == 6);

  // Access last row
  auto &row3 = grid.getRow(2);
  CHECK(row3.size() == 3);
  CHECK(row3[0].GetDouble() == 7);
  CHECK(row3[1].GetDouble() == 8);
  CHECK(row3[2].GetDouble() == 9);

  // Ensure modifying the retrieved row affects the original grid
  row3[0] = Datum(99.9);
  CHECK(grid.getValue(2, 0).GetDouble() == 99.9);
}

/**
 * @brief Test accessing column
 */
TEST_CASE("DataGrid: Get Column", "[access]") {
  // Create a 3x3 grid with unique values
  DataGrid grid({{Datum(1), Datum(2), Datum(3)},
                 {Datum(4), Datum(5), Datum(6)},
                 {Datum(7), Datum(8), Datum(9)}});

  auto [rows, cols] = grid.shape();
  CHECK(rows == 3);
  CHECK(cols == 3);

  // Access first column
  auto column0 = grid.getColumn(0);
  CHECK(column0.Size() == 3);
  CHECK(column0[0].GetDouble() == 1);
  CHECK(column0[1].GetDouble() == 4);
  CHECK(column0[2].GetDouble() == 7);

  // Access middle column
  auto column1 = grid.getColumn(1);
  CHECK(column1.Size() == 3);
  CHECK(column1[0].GetDouble() == 2);
  CHECK(column1[1].GetDouble() == 5);
  CHECK(column1[2].GetDouble() == 8);

  // Access last column
  auto column2 = grid.getColumn(2);
  CHECK(column2.Size() == 3);
  CHECK(column2[0].GetDouble() == 3);
  CHECK(column2[1].GetDouble() == 6);
  CHECK(column2[2].GetDouble() == 9);

  // Modify a value in the retrieved column and verify the original grid changes
  column1[1] = Datum(99.9);
  CHECK(grid.getValue(1, 1).GetDouble() == 99.9);
}

/**
 * @brief Test accessing a specific value
 */
TEST_CASE("DataGrid: Get Value", "[access]") {
  // Create a 2x2 grid with default values
  DataGrid grid(2, 2, 0.0);

  auto [rows, cols] = grid.shape();
  CHECK(rows == 2);
  CHECK(cols == 2);

  // Assign and retrieve a double value
  Datum d(42.0);
  grid.at(1, 1) = d;
  CHECK(grid.getValue(1, 1).GetDouble() == 42.0);

  // Assign and retrieve a string value
  Datum s("Hello");
  grid.at(0, 0) = s;
  CHECK(grid.getValue(0, 0).GetString() == "Hello");

  // Modify an existing value
  grid.at(1, 1) = Datum(99.9);
  CHECK(grid.getValue(1, 1).GetDouble() == 99.9);

  // Verify other values remain unchanged
  CHECK(grid.getValue(0, 1).GetDouble() == 0.0);
  CHECK(grid.getValue(1, 0).GetDouble() == 0.0);
}

/**
 * @brief Test resizing
 */
TEST_CASE("DataGrid: Resize", "[resize]") {
  // Create a 2x2 grid with default value 1.0
  DataGrid grid(2, 2, 1.0);
  auto [rows1, cols1] = grid.shape();
  CHECK(rows1 == 2);
  CHECK(cols1 == 2);
  CHECK(grid.getValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.getValue(1, 1).GetDouble() == 1.0);

  // Resize to a larger 4x5 grid with new default value 3.14
  grid.resize(4, 5, 3.14);
  auto [rows2, cols2] = grid.shape();
  CHECK(rows2 == 4);
  CHECK(cols2 == 5);

  // Verify old values are preserved
  CHECK(grid.getValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.getValue(1, 1).GetDouble() == 1.0);

  // Verify new cells are initialized to 3.14
  CHECK(grid.getValue(2, 2).GetDouble() == 3.14);
  CHECK(grid.getValue(3, 4).GetDouble() == 3.14);

  // Resize to a smaller 2x2 grid
  grid.resize(2, 2, 0.0);
  auto [rows3, cols3] = grid.shape();
  CHECK(rows3 == 2);
  CHECK(cols3 == 2);

  // Ensure resized grid still contains previous values within bounds
  CHECK(grid.getValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.getValue(1, 1).GetDouble() == 1.0);

  // Resize to a 3x3 grid with string default value "Empty"
  grid.resize(3, 3, "Empty");
  auto [rows4, cols4] = grid.shape();
  CHECK(rows4 == 3);
  CHECK(cols4 == 3);

  // Verify new string values
  CHECK(grid.getValue(2, 2).GetString() == "Empty");
  CHECK(grid.getValue(2, 1).GetString() == "Empty");

  // Ensure existing values remain unchanged
  CHECK(grid.getValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.getValue(1, 1).GetDouble() == 1.0);
}

/**
 * @brief Test clear function
 */
TEST_CASE("DataGrid: Clear", "[clear]") {
  // Create a 3x3 grid with default value 42.0
  DataGrid grid(3, 3, 42.0);
  auto [rows1, cols1] = grid.shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 3);

  // Verify initial values
  CHECK(grid.getValue(0, 0).GetDouble() == 42.0);
  CHECK(grid.getValue(2, 2).GetDouble() == 42.0);

  // Clear the grid
  grid.clear();
  auto [rows2, cols2] = grid.shape();
  CHECK(rows2 == 0);
  CHECK(cols2 == 0);

  // Clear an already empty grid
  grid.clear();
  auto [rows3, cols3] = grid.shape();
  CHECK(rows3 == 0);
  CHECK(cols3 == 0);
}

/**
 * @brief Test stream output
 */
TEST_CASE("DataGrid: Stream Output", "[stream]") {
  // Create a 2x2 grid with unique values
  DataGrid grid({{Datum(1.1), Datum(2.2)}, {Datum("Hello"), Datum("World")}});

  std::ostringstream os;
  os << grid;
  std::string output = os.str();

  // Check if the output contains the correct grid shape
  CHECK(output.find("Grid Shape: 2 x 2") != std::string::npos);

  // Check if numeric values are correctly included
  CHECK(output.find("1.1") != std::string::npos);
  CHECK(output.find("2.2") != std::string::npos);

  // Check if string values are correctly included
  CHECK(output.find("Hello") != std::string::npos);
  CHECK(output.find("World") != std::string::npos);

  // Check for expected table-like format (assuming tab/spaces or newlines)
  CHECK(output.find("\n") != std::string::npos);
}

/**
 * @brief Test sorting a specific column in DataGrid
 */
TEST_CASE("DataGrid: Sort Column", "[sort]") {
  // Create a 3x2 grid with mixed data types (numbers & strings)
  DataGrid grid(
      {{Datum(3), Datum("b")}, {Datum(1), Datum("a")}, {Datum(2), Datum("c")}});

  // Sort column 0 (numerical values) in ascending order
  grid.sortColumn(0, true);

  CHECK(grid.getValue(0, 0).GetDouble() == 1);
  CHECK(grid.getValue(1, 0).GetDouble() == 2);
  CHECK(grid.getValue(2, 0).GetDouble() == 3);

  // Verify the second column remains unchanged
  CHECK(grid.getValue(0, 1).GetString() == "a");
  CHECK(grid.getValue(1, 1).GetString() == "c");
  CHECK(grid.getValue(2, 1).GetString() == "b");

  // Sort column 0 in descending order
  grid.sortColumn(0, false);
  CHECK(grid.getValue(0, 0).GetDouble() == 3);
  CHECK(grid.getValue(1, 0).GetDouble() == 2);
  CHECK(grid.getValue(2, 0).GetDouble() == 1);
}

/**
 * @brief Test sorting the entire DataGrid row-wise
 */
TEST_CASE("DataGrid: Sort Entire Grid", "[sort]") {
  // Create a 3x2 grid with mixed types (numbers & strings)
  DataGrid grid(
      {{Datum(3), Datum("b")}, {Datum(1), Datum("a")}, {Datum(2), Datum("c")}});

  // Sort rows in ascending order (row-wise sorting)
  grid.sort(true);

  CHECK(grid.getValue(0, 0).GetDouble() == 1);
  CHECK(grid.getValue(1, 0).GetDouble() == 2);
  CHECK(grid.getValue(2, 0).GetDouble() == 3);

  CHECK(grid.getValue(0, 1).GetString() == "a");
  CHECK(grid.getValue(1, 1).GetString() == "c");
  CHECK(grid.getValue(2, 1).GetString() == "b");

  // Sort rows in descending order
  grid.sort(false);
  CHECK(grid.getValue(0, 0).GetDouble() == 3);
  CHECK(grid.getValue(1, 0).GetDouble() == 2);
  CHECK(grid.getValue(2, 0).GetDouble() == 1);

  CHECK(grid.getValue(0, 1).GetString() == "b");
  CHECK(grid.getValue(1, 1).GetString() == "c");
  CHECK(grid.getValue(2, 1).GetString() == "a");
}

/**
 * @brief Test sorting a column with mixed data types
 */
TEST_CASE("DataGrid: Sort Column with Mixed Data Types", "[sort]") {
  // Create a column with mixed data types (numbers & strings)
  DataGrid grid({{Datum("Apple")}, {Datum(10)}, {Datum("Banana")}, {Datum(5)}});

  // Sort column 0 in ascending order
  grid.sortColumn(0, true);

  // Ensure doubles come before strings in sorting
  CHECK(grid.getValue(0, 0).GetDouble() == 5);
  CHECK(grid.getValue(1, 0).GetDouble() == 10);
  CHECK(grid.getValue(2, 0).GetString() == "Apple");
  CHECK(grid.getValue(3, 0).GetString() == "Banana");

  // Sort column 0 in descending order
  grid.sortColumn(0, false);
  CHECK(grid.getValue(0, 0).GetDouble() == 10);
  CHECK(grid.getValue(1, 0).GetDouble() == 5);
  CHECK(grid.getValue(2, 0).GetString() == "Banana");
  CHECK(grid.getValue(3, 0).GetString() == "Apple");
}

/**
 * @brief Test merging two grids
 */
TEST_CASE("DataGrid: Merge", "[merge]") {
  // Create two 2x2 grids with different values
  DataGrid grid1({{Datum(1.0), Datum(2.0)}, {Datum(3.0), Datum(4.0)}});

  DataGrid grid2({{Datum(5.0), Datum(6.0)}, {Datum(7.0), Datum(8.0)}});

  // Merge the grids row-wise (stack below)
  DataGrid mergedRow = grid1.merge(grid2, true);
  auto [rows1, cols1] = mergedRow.shape();
  CHECK(rows1 == 4);
  CHECK(cols1 == 2);

  // Ensure values are correctly merged row-wise
  CHECK(mergedRow.getValue(0, 0).GetDouble() == 1.0);
  CHECK(mergedRow.getValue(1, 1).GetDouble() == 4.0);
  CHECK(mergedRow.getValue(2, 0).GetDouble() == 5.0);
  CHECK(mergedRow.getValue(3, 1).GetDouble() == 8.0);

  // Merge the grids column-wise (stack side-by-side)
  DataGrid mergedCol = grid1.merge(grid2, false);
  auto [rows2, cols2] = mergedCol.shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 4);

  // Ensure values are correctly merged column-wise
  CHECK(mergedCol.getValue(0, 0).GetDouble() == 1.0);
  CHECK(mergedCol.getValue(0, 1).GetDouble() == 2.0);
  CHECK(mergedCol.getValue(0, 2).GetDouble() == 5.0);
  CHECK(mergedCol.getValue(0, 3).GetDouble() == 6.0);
  CHECK(mergedCol.getValue(1, 2).GetDouble() == 7.0);
  CHECK(mergedCol.getValue(1, 3).GetDouble() == 8.0);
}

/**
 * @brief Test searching in a column
 */
TEST_CASE("DataGrid: Search", "[search]") {
  // Create a 4x3 grid with numbers and strings
  DataGrid grid({{Datum(5.0), Datum("Apple"), Datum(10.0)},
                 {Datum(10.0), Datum("Banana"), Datum(20.0)},
                 {Datum(5.0), Datum("Apple"), Datum(30.0)},
                 {Datum(15.0), Datum("Cherry"), Datum(10.0)}});

  // Search for numeric value 10.0 in column 2
  auto indices1 = grid.search(2, Datum(10.0));
  CHECK(indices1.size() == 2);
  CHECK(indices1[0] == 0);
  CHECK(indices1[1] == 3);

  // Search for string "Apple" in column 1
  auto indices2 = grid.search(1, Datum("Apple"));
  CHECK(indices2.size() == 2);
  CHECK(indices2[0] == 0);
  CHECK(indices2[1] == 2);

  // Search for a non-existent value (should return an empty list)
  auto indices3 = grid.search(0, Datum(99.9));
  CHECK(indices3.empty());

  // Search for the value 5.0 in column 0 (multiple occurrences)
  auto indices4 = grid.search(0, Datum(5.0));
  CHECK(indices4.size() == 2);
  CHECK(indices4[0] == 0);
  CHECK(indices4[1] == 2);

  // Search for string "Cherry" in column 1
  auto indices5 = grid.search(1, Datum("Cherry"));
  CHECK(indices5.size() == 1);
  CHECK(indices5[0] == 3);

  // Test mixed-type column (numbers and strings)
  DataGrid mixedGrid({{Datum(10.0)}, {Datum("10")}, {Datum(20.0)}});

  // Ensure searching for number 10.0 does not match string "10"
  auto indices6 = mixedGrid.search(0, Datum(10.0));
  CHECK(indices6.size() == 1);
  CHECK(indices6[0] == 0);

  // Ensure searching for string "10" does not match number 10.0
  auto indices7 = mixedGrid.search(0, Datum("10"));
  CHECK(indices7.size() == 1);
  CHECK(indices7[0] == 1);

  // Test searching in an empty DataGrid
  DataGrid emptyGrid(0, 0);
  auto indices8 = emptyGrid.search(0, Datum(10));
  CHECK(indices8.empty());
}

/**
 * @brief Test slicing a grid
 */
TEST_CASE("DataGrid: Slice", "[slice]") {
  // Create a 4x4 grid with incremental values
  DataGrid grid({{Datum(1), Datum(2), Datum(3), Datum(4)},
                 {Datum(5), Datum(6), Datum(7), Datum(8)},
                 {Datum(9), Datum(10), Datum(11), Datum(12)},
                 {Datum(13), Datum(14), Datum(15), Datum(16)}});

  SECTION("Basic slice - Extract 2x2 subgrid") {
    DataGrid sliced = grid.slice(1, 2, 1, 2);
    auto [rows, cols] = sliced.shape();

    CHECK(rows == 2);
    CHECK(cols == 2);
    CHECK(sliced.getValue(0, 0).GetDouble() == 6);
    CHECK(sliced.getValue(0, 1).GetDouble() == 7);
    CHECK(sliced.getValue(1, 0).GetDouble() == 10);
    CHECK(sliced.getValue(1, 1).GetDouble() == 11);
  }

  SECTION("Edge case - Extract entire grid") {
    DataGrid sliced = grid.slice(0, 3, 0, 3);
    auto [rows, cols] = sliced.shape();
    CHECK(rows == 4);
    CHECK(cols == 4);
    CHECK(sliced.getValue(3, 3).GetDouble() == 16);
  }

  SECTION("Edge case - Extract a single row") {
    DataGrid sliced = grid.slice(2, 2, 0, 3);
    auto [rows, cols] = sliced.shape();
    CHECK(rows == 1);
    CHECK(cols == 4);
    CHECK(sliced.getValue(0, 2).GetDouble() == 11);
  }

  SECTION("Edge case - Extract a single column") {
    DataGrid sliced = grid.slice(0, 3, 1, 1);
    auto [rows, cols] = sliced.shape();
    CHECK(rows == 4);
    CHECK(cols == 1);
    CHECK(sliced.getValue(3, 0).GetDouble() == 14);
  }

  SECTION("Edge case - Extract a single element") {
    DataGrid sliced = grid.slice(1, 1, 2, 2);
    auto [rows, cols] = sliced.shape();
    CHECK(rows == 1);
    CHECK(cols == 1);
    CHECK(sliced.getValue(0, 0).GetDouble() == 7);
  }
}

/**
 * @brief Comprehensive test for DataGrid class under different conditions
 */
TEST_CASE("DataGrid: Comprehensive Test", "[comprehensive]") {
  // Step 1: Create a 5x5 grid with default value 1.0
  DataGrid grid(5, 5, 1.0);
  auto [rows1, cols1] = grid.shape();
  CHECK(rows1 == 5);
  CHECK(cols1 == 5);

  // Step 2: Insert a default row at index 2 with value 2.0
  grid.insertDefaultRow(2, 2.0);
  auto [rows2, cols2] = grid.shape();
  CHECK(rows2 == 6);
  CHECK(cols2 == 5);

  // Verify inserted row values
  for (size_t j = 0; j < cols2; ++j) {
    CHECK(grid.getValue(2, j).GetDouble() == 2.0);
  }

  // Step 3: Insert a default column at index 3 with value "Water"
  grid.insertDefaultColumn(3, "Water");
  auto [rows3, cols3] = grid.shape();
  CHECK(rows3 == 6);
  CHECK(cols3 == 6);

  // Verify inserted column values
  for (size_t i = 0; i < rows3; ++i) {
    CHECK(grid.getValue(i, 3).GetString() == "Water");
  }

  // Step 4: Modify a specific value
  grid.at(4, 4) = Datum(9.99);
  CHECK(grid.getValue(4, 4).GetDouble() == 9.99);

  // Step 5: Delete the first row
  grid.deleteRow(0);
  auto [rows4, cols4] = grid.shape();
  CHECK(rows4 == 5);
  CHECK(cols4 == 6);

  // Step 6: Delete the second column (was originally index 1)
  grid.deleteColumn(1);
  auto [rows5, cols5] = grid.shape();
  CHECK(rows5 == 5);
  CHECK(cols5 == 5);

  // Step 7: Resize the grid to 3x3 with default value 0.0
  grid.resize(3, 3, 0.0);
  auto [rows6, cols6] = grid.shape();
  CHECK(rows6 == 3);
  CHECK(cols6 == 3);

  // Step 8: Verify the last modified value persists if within bounds
  if (rows6 > 2 && cols6 > 2) {
    CHECK(grid.getValue(2, 2).GetString() == "Water");
  }
}

/**
 * @brief Mathematical Tests (Except mode)
 */
TEST_CASE("DataGrid: Mathematical Functions", "[mathematics]") {
  // Cite: Used https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
  // to help with creating a 2D vector.
  std::vector<std::vector<Datum>> math_vector(5, std::vector<Datum>(3));

  math_vector[0][0] = Datum(5.0);
  math_vector[1][0] = Datum(3.5);
  math_vector[2][0] = Datum(1.25);
  math_vector[3][0] = Datum(-15);
  math_vector[4][0] = Datum(4.25);

  math_vector[0][1] = Datum("test1");
  math_vector[1][1] = Datum("test2");
  math_vector[2][1] = Datum("test4");
  math_vector[3][1] = Datum("test5");

  math_vector[0][2] = Datum(10.25);
  math_vector[1][2] = Datum("test3");
  math_vector[2][2] = Datum(150.50);
  math_vector[3][2] = Datum(200);
  math_vector[4][1] = Datum("test6");
  math_vector[4][2] = Datum(20.25);

  DataGrid grid(math_vector);

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

  DataGrid empty_data_grid;
  CHECK_THROWS(empty_data_grid.columnMean(99));
  CHECK_THROWS(empty_data_grid.columnMedian(99));
  CHECK_THROWS(empty_data_grid.columnStandardDeviation(99));
  CHECK_THROWS(empty_data_grid.columnMin(99));
  CHECK_THROWS(empty_data_grid.columnMax(99));

}

/**
 * @brief Mathematical Test: Mode
 */
TEST_CASE("Mode Column Tests", "[mathematics]") {
  std::vector<std::vector<Datum>> mode_test(5, std::vector<Datum>(3));

  mode_test[0][0] = Datum(5.55);
  mode_test[1][0] = Datum(5.55);
  mode_test[2][0] = Datum(1.25);
  mode_test[3][0] = Datum(-15);
  mode_test[4][0] = Datum(1.25);

  mode_test[0][1] = Datum("test1");
  mode_test[1][1] = Datum("test2");
  mode_test[2][1] = Datum("test4");
  mode_test[3][1] = Datum("test5");
  mode_test[4][1] = Datum("test6");

  mode_test[0][2] = Datum(10.25);
  mode_test[1][2] = Datum("test3");
  mode_test[2][2] = Datum(10.25);
  mode_test[3][2] = Datum(10.25);
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

  // Empty test
  DataGrid empty_data_grid;
  CHECK_THROWS(empty_data_grid.columnMode(99));
}

/**
 * @brief DataGrid Mathematical Summary
 */
TEST_CASE("DataGrid Mathematical Summary", "[mathematics]") {
  // Regular test (doubles and strings)
  std::vector<std::vector<Datum>> grid_test(5, std::vector<Datum>(3));

  grid_test[0][0] = Datum(5.0);
  grid_test[1][0] = Datum(3.5);
  grid_test[2][0] = Datum(1.25);
  grid_test[3][0] = Datum(-15);
  grid_test[4][0] = Datum(4.25);

  grid_test[0][1] = Datum("test1");
  grid_test[1][1] = Datum("test2");
  grid_test[2][1] = Datum("test4");
  grid_test[3][1] = Datum("test5");
  grid_test[4][1] = Datum("test6");

  grid_test[0][2] = Datum(5.0);
  grid_test[1][2] = Datum("test3");
  grid_test[2][2] = Datum(10.25);
  grid_test[3][2] = Datum(1.25);
  grid_test[4][2] = Datum(20.25);

  DataGrid grid(grid_test);

  DataGrid::DataGridMathSummary data_grid_math_summary = grid.dataGridMathSummary();

  CHECK_THAT(data_grid_math_summary.mean,
             Catch::WithinAbs(3.97222, kEpsilon));
  CHECK_THAT(data_grid_math_summary.median,
             Catch::WithinAbs(4.25, kEpsilon));
  CHECK_THAT(data_grid_math_summary.standardDeviation,
             Catch::WithinAbs(8.7022063329783, kEpsilon));
  CHECK_THAT(data_grid_math_summary.min,
             Catch::WithinAbs(-15, kEpsilon));
  CHECK_THAT(data_grid_math_summary.max,
             Catch::WithinAbs(20.25, kEpsilon));

  std::sort(data_grid_math_summary.mode.begin(), data_grid_math_summary.mode.end());
  CHECK(data_grid_math_summary.mode.size() == 2);
  CHECK_THAT(data_grid_math_summary.mode[0],
             Catch::WithinAbs(1.25, kEpsilon));
  CHECK_THAT(data_grid_math_summary.mode[1],
             Catch::WithinAbs(5.0, kEpsilon));

  // DataGrid with NaN
  std::vector<std::vector<Datum>> vector_nan(2, std::vector<Datum>(2));

  vector_nan[0][0] = Datum(5.0);
  vector_nan[1][0] = Datum(std::numeric_limits<double>::quiet_NaN());

  vector_nan[0][1] = Datum(5.0);
  vector_nan[1][1] = Datum(3.5);

  DataGrid nan_grid(vector_nan);

  DataGrid::DataGridMathSummary nan_data_grid_math_summary = nan_grid.dataGridMathSummary();

  CHECK_THAT(nan_data_grid_math_summary.mean,
             Catch::WithinAbs(4.5, kEpsilon));
  CHECK_THAT(nan_data_grid_math_summary.median,
             Catch::WithinAbs(5, kEpsilon));
  CHECK_THAT(nan_data_grid_math_summary.standardDeviation,
             Catch::WithinAbs(0.70710678118655, kEpsilon));
  CHECK_THAT(nan_data_grid_math_summary.min,
             Catch::WithinAbs(3.5, kEpsilon));
  CHECK_THAT(nan_data_grid_math_summary.max,
             Catch::WithinAbs(5.0, kEpsilon));

  CHECK(nan_data_grid_math_summary.mode.size() == 1);
  CHECK_THAT(nan_data_grid_math_summary.mode[0],
             Catch::WithinAbs(5.0, kEpsilon));

  // Strings only
  std::vector<std::vector<Datum>> vector_strings(2, std::vector<Datum>(2));

  vector_strings[0][0] = Datum("a");
  vector_strings[1][0] = Datum("b");

  vector_strings[0][1] = Datum("c");
  vector_strings[1][1] = Datum("d");

  DataGrid string_grid(vector_strings);

  DataGrid::DataGridMathSummary string_data_grid_math_summary = string_grid.dataGridMathSummary();

  CHECK(std::isnan(string_data_grid_math_summary.mean));
  CHECK(std::isnan(string_data_grid_math_summary.median));
  CHECK(std::isnan(string_data_grid_math_summary.standardDeviation));
  CHECK(std::isnan(string_data_grid_math_summary.min));
  CHECK(std::isnan(string_data_grid_math_summary.max));
  CHECK(string_data_grid_math_summary.mode.size() == 0);
}

TEST_CASE("Comparison Tests", "[comparison]") {
  std::vector<std::vector<Datum>> comparison_vector(5, std::vector<Datum>(3));

  comparison_vector[0][0] = Datum(5.0);
  comparison_vector[1][0] = Datum(10.0);
  comparison_vector[2][0] = Datum(15.0);
  comparison_vector[3][0] = Datum(20.0);
  comparison_vector[4][0] = Datum(25.0);

  comparison_vector[0][1] = Datum("a");
  comparison_vector[1][1] = Datum("c");
  comparison_vector[2][1] = Datum("e");
  comparison_vector[3][1] = Datum("g");
  comparison_vector[4][1] = Datum("i");

  comparison_vector[0][2] = Datum(55.55);
  comparison_vector[1][2] = Datum("aa");
  comparison_vector[2][2] = Datum(-150.50);
  comparison_vector[3][2] = Datum("cc");
  comparison_vector[4][2] = Datum(123.123);

  DataGrid grid(comparison_vector);


  // *** Less Than Comparisons ***

  cse::ReferenceVector<Datum> less_than_values_doubles =
      grid.columnLessThan(0, Datum(16.0));
  CHECK(less_than_values_doubles.Size() == 3);
  CHECK(less_than_values_doubles[0] == Datum(5.0));
  CHECK(less_than_values_doubles[1] == Datum(10.0));
  CHECK(less_than_values_doubles[2] == Datum(15.0));

  cse::ReferenceVector<Datum> less_than_values_doubles1 =
      grid.columnLessThan(0, Datum(25.0));
  CHECK(less_than_values_doubles1.Size() == 4);
  CHECK(less_than_values_doubles1[0] == Datum(5.0));
  CHECK(less_than_values_doubles1[1] == Datum(10.0));
  CHECK(less_than_values_doubles1[2] == Datum(15.0));
  CHECK(less_than_values_doubles1[3] == Datum(20.0));

  cse::ReferenceVector<Datum> less_than_values_strings =
      grid.columnLessThan(1, Datum("f"));
  CHECK(less_than_values_strings.Size() == 3);
  CHECK(less_than_values_strings[0] == Datum("a"));
  CHECK(less_than_values_strings[1] == Datum("c"));
  CHECK(less_than_values_strings[2] == Datum("e"));

  cse::ReferenceVector<Datum> less_than_values_strings1 =
      grid.columnLessThan(1, Datum("i"));
  CHECK(less_than_values_strings1.Size() == 4);
  CHECK(less_than_values_strings1[0] == Datum("a"));
  CHECK(less_than_values_strings1[1] == Datum("c"));
  CHECK(less_than_values_strings1[2] == Datum("e"));
  CHECK(less_than_values_strings1[3] == Datum("g"));

  cse::ReferenceVector<Datum> less_than_values_mix_strings =
      grid.columnLessThan(2, Datum("bb"));
  CHECK(less_than_values_mix_strings.Size() == 1);
  CHECK(less_than_values_mix_strings[0] == Datum("aa"));

  cse::ReferenceVector<Datum> less_than_values_mix_doubles =
      grid.columnLessThan(2, Datum(100.0));
  CHECK(less_than_values_mix_doubles.Size() == 2);
  CHECK(less_than_values_mix_doubles[0] == Datum(55.55));
  CHECK(less_than_values_mix_doubles[1] == Datum(-150.50));


  // *** Less Than Or Equal Comparisons ***

  cse::ReferenceVector<Datum> less_than_or_equal_values_doubles =
      grid.columnLessThanOrEqual(0, Datum(16.0));
  CHECK(less_than_or_equal_values_doubles.Size() == 3);
  CHECK(less_than_or_equal_values_doubles[0] == Datum(5.0));
  CHECK(less_than_or_equal_values_doubles[1] == Datum(10.0));
  CHECK(less_than_or_equal_values_doubles[2] == Datum(15.0));

  cse::ReferenceVector<Datum> less_than_or_equal_values_doubles1 =
      grid.columnLessThanOrEqual(0, Datum(25.0));
  CHECK(less_than_or_equal_values_doubles1.Size() == 5);
  CHECK(less_than_or_equal_values_doubles1[0] == Datum(5.0));
  CHECK(less_than_or_equal_values_doubles1[1] == Datum(10.0));
  CHECK(less_than_or_equal_values_doubles1[2] == Datum(15.0));
  CHECK(less_than_or_equal_values_doubles1[3] == Datum(20.0));
  CHECK(less_than_or_equal_values_doubles1[4] == Datum(25.0));

  cse::ReferenceVector<Datum> less_than_or_equal_values_strings =
      grid.columnLessThanOrEqual(1, Datum("f"));
  CHECK(less_than_or_equal_values_strings.Size() == 3);
  CHECK(less_than_or_equal_values_strings[0] == Datum("a"));
  CHECK(less_than_or_equal_values_strings[1] == Datum("c"));
  CHECK(less_than_or_equal_values_strings[2] == Datum("e"));

  cse::ReferenceVector<Datum> less_than_or_equal_values_strings1 =
      grid.columnLessThanOrEqual(1, Datum("i"));
  CHECK(less_than_or_equal_values_strings1.Size() == 5);
  CHECK(less_than_or_equal_values_strings1[0] == Datum("a"));
  CHECK(less_than_or_equal_values_strings1[1] == Datum("c"));
  CHECK(less_than_or_equal_values_strings1[2] == Datum("e"));
  CHECK(less_than_or_equal_values_strings1[3] == Datum("g"));
  CHECK(less_than_or_equal_values_strings1[4] == Datum("i"));

  cse::ReferenceVector<Datum> less_than_or_equal_values_mix_strings =
      grid.columnLessThanOrEqual(2, Datum("bb"));
  CHECK(less_than_or_equal_values_mix_strings.Size() == 1);
  CHECK(less_than_or_equal_values_mix_strings[0] == Datum("aa"));

  cse::ReferenceVector<Datum> less_than_or_equal_values_mix_doubles =
      grid.columnLessThanOrEqual(2, Datum(100.0));
  CHECK(less_than_or_equal_values_mix_doubles.Size() == 2);
  CHECK(less_than_or_equal_values_mix_doubles[0] == Datum(55.55));
  CHECK(less_than_or_equal_values_mix_doubles[1] == Datum(-150.50));


  // *** Greater Than Comparisons ***

  cse::ReferenceVector<Datum> greater_than_values_doubles =
      grid.columnGreaterThan(0, Datum(16.0));
  CHECK(greater_than_values_doubles.Size() == 2);
  CHECK(greater_than_values_doubles[0] == Datum(20.0));
  CHECK(greater_than_values_doubles[1] == Datum(25.0));

  cse::ReferenceVector<Datum> greater_than_values_doubles1 =
      grid.columnGreaterThan(0, Datum(20.0));
  CHECK(greater_than_values_doubles1.Size() == 1);
  CHECK(greater_than_values_doubles1[0] == Datum(25.0));

  cse::ReferenceVector<Datum> greater_than_values_strings =
      grid.columnGreaterThan(1, Datum("f"));
  CHECK(greater_than_values_strings.Size() == 2);
  CHECK(greater_than_values_strings[0] == Datum("g"));
  CHECK(greater_than_values_strings[1] == Datum("i"));

  cse::ReferenceVector<Datum> greater_than_values_strings1 =
      grid.columnGreaterThan(1, Datum("c"));
  CHECK(greater_than_values_strings1.Size() == 3);
  CHECK(greater_than_values_strings1[0] == Datum("e"));
  CHECK(greater_than_values_strings1[1] == Datum("g"));
  CHECK(greater_than_values_strings1[2] == Datum("i"));

  cse::ReferenceVector<Datum> greater_than_values_mix_strings =
      grid.columnGreaterThan(2, Datum("bb"));
  CHECK(greater_than_values_mix_strings.Size() == 1);
  CHECK(greater_than_values_mix_strings[0] == Datum("cc"));

  cse::ReferenceVector<Datum> greater_than_values_mix_doubles =
      grid.columnGreaterThan(2, Datum(1.23));
  CHECK(greater_than_values_mix_doubles.Size() == 2);
  CHECK(greater_than_values_mix_doubles[0] == Datum(55.55));
  CHECK(greater_than_values_mix_doubles[1] == Datum(123.123));


  // *** Greater Than Or Equal Comparisons ***

  cse::ReferenceVector<Datum> greater_than_or_equal_values_doubles =
      grid.columnGreaterThanOrEqual(0, Datum(16.0));
  CHECK(greater_than_or_equal_values_doubles.Size() == 2);
  CHECK(greater_than_or_equal_values_doubles[0] == Datum(20.0));
  CHECK(greater_than_or_equal_values_doubles[1] == Datum(25.0));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_doubles1 =
      grid.columnGreaterThanOrEqual(0, Datum(20.0));
  CHECK(greater_than_or_equal_values_doubles1.Size() == 2);
  CHECK(greater_than_or_equal_values_doubles1[0] == Datum(20.0));
  CHECK(greater_than_or_equal_values_doubles1[1] == Datum(25.0));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_strings =
      grid.columnGreaterThanOrEqual(1, Datum("f"));
  CHECK(greater_than_or_equal_values_strings.Size() == 2);
  CHECK(greater_than_or_equal_values_strings[0] == Datum("g"));
  CHECK(greater_than_or_equal_values_strings[1] == Datum("i"));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_strings1 =
      grid.columnGreaterThanOrEqual(1, Datum("c"));
  CHECK(greater_than_or_equal_values_strings1.Size() == 4);
  CHECK(greater_than_or_equal_values_strings1[0] == Datum("c"));
  CHECK(greater_than_or_equal_values_strings1[1] == Datum("e"));
  CHECK(greater_than_or_equal_values_strings1[2] == Datum("g"));
  CHECK(greater_than_or_equal_values_strings1[3] == Datum("i"));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_mix_strings =
      grid.columnGreaterThanOrEqual(2, Datum("aa"));
  CHECK(greater_than_or_equal_values_mix_strings.Size() == 2);
  CHECK(greater_than_or_equal_values_mix_strings[0] == Datum("aa"));
  CHECK(greater_than_or_equal_values_mix_strings[1] == Datum("cc"));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_mix_doubles =
      grid.columnGreaterThanOrEqual(2, Datum(55.55));
  CHECK(greater_than_or_equal_values_mix_doubles.Size() == 2);
  CHECK(greater_than_or_equal_values_mix_doubles[0] == Datum(55.55));
  CHECK(greater_than_or_equal_values_mix_doubles[1] == Datum(123.123));


  // *** Equal Comparisons ***

  cse::ReferenceVector<Datum> equal_values_doubles =
      grid.columnEqual(0, Datum(16.0));
  CHECK(equal_values_doubles.Size() == 0);

  cse::ReferenceVector<Datum> equal_values_doubles1 =
      grid.columnEqual(0, Datum(20.0));
  CHECK(equal_values_doubles1.Size() == 1);
  CHECK(equal_values_doubles1[0] == Datum(20.0));

  cse::ReferenceVector<Datum> equal_values_strings =
      grid.columnEqual(1, Datum("f"));
  CHECK(equal_values_strings.Size() == 0);

  cse::ReferenceVector<Datum> equal_values_strings1 =
      grid.columnEqual(1, Datum("c"));
  CHECK(equal_values_strings1.Size() == 1);
  CHECK(equal_values_strings1[0] == Datum("c"));

  cse::ReferenceVector<Datum> equal_values_mix_strings =
      grid.columnEqual(2, Datum("aa"));
  CHECK(equal_values_mix_strings.Size() == 1);
  CHECK(equal_values_mix_strings[0] == Datum("aa"));

  cse::ReferenceVector<Datum> equal_values_mix_doubles =
      grid.columnEqual(2, Datum(55.55));
  CHECK(equal_values_mix_doubles.Size() == 1);
  CHECK(equal_values_mix_doubles[0] == Datum(55.55));


  // *** Not Equal Comparisons ***

  cse::ReferenceVector<Datum> not_equal_values_doubles =
      grid.columnNotEqual(0, Datum(16.0));
  CHECK(not_equal_values_doubles.Size() == 5);
  CHECK(not_equal_values_doubles[0] == Datum(5.0));
  CHECK(not_equal_values_doubles[1] == Datum(10.0));
  CHECK(not_equal_values_doubles[2] == Datum(15.0));
  CHECK(not_equal_values_doubles[3] == Datum(20.0));
  CHECK(not_equal_values_doubles[4] == Datum(25.0));

  cse::ReferenceVector<Datum> not_equal_values_doubles1 =
      grid.columnNotEqual(0, Datum(20.0));
  CHECK(not_equal_values_doubles1.Size() == 4);
  CHECK(not_equal_values_doubles1[0] == Datum(5.0));
  CHECK(not_equal_values_doubles1[1] == Datum(10.0));
  CHECK(not_equal_values_doubles1[2] == Datum(15.0));
  CHECK(not_equal_values_doubles1[3] == Datum(25.0));

  cse::ReferenceVector<Datum> not_equal_values_strings =
      grid.columnNotEqual(1, Datum("f"));
  CHECK(not_equal_values_strings.Size() == 5);
  CHECK(not_equal_values_strings[0] == Datum("a"));
  CHECK(not_equal_values_strings[1] == Datum("c"));
  CHECK(not_equal_values_strings[2] == Datum("e"));
  CHECK(not_equal_values_strings[3] == Datum("g"));
  CHECK(not_equal_values_strings[4] == Datum("i"));

  cse::ReferenceVector<Datum> not_equal_values_strings1 =
      grid.columnNotEqual(1, Datum("c"));
  CHECK(not_equal_values_strings1.Size() == 4);
  CHECK(not_equal_values_strings1[0] == Datum("a"));
  CHECK(not_equal_values_strings1[1] == Datum("e"));
  CHECK(not_equal_values_strings1[2] == Datum("g"));
  CHECK(not_equal_values_strings1[3] == Datum("i"));

  cse::ReferenceVector<Datum> not_equal_values_mix_strings =
      grid.columnNotEqual(2, Datum("aa"));
  CHECK(not_equal_values_mix_strings.Size() == 1);
  CHECK(not_equal_values_mix_strings[0] == Datum("cc"));

  cse::ReferenceVector<Datum> not_equal_values_mix_doubles =
      grid.columnNotEqual(2, Datum(55.55));
  CHECK(not_equal_values_mix_doubles.Size() == 2);
  CHECK(not_equal_values_mix_doubles[0] == Datum(-150.50));
  CHECK(not_equal_values_mix_doubles[1] == Datum(123.123));

  // Out of bounds column error check
  CHECK_THROWS(grid.columnLessThan(999, Datum(25.0)));

  // Checks if updating the comparison ReferenceVector updates the grid
  less_than_values_doubles[2] = Datum(100);
  CHECK(grid.at(2,0) == Datum(100));

  less_than_or_equal_values_strings[0] = Datum("x");
  CHECK(grid.at(0,1) == Datum("x"));
}
