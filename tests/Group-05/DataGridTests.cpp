/**
 * @file DataGridTests.cpp
 * @authors Shahaab Ali and Max Krawec
 */

// Made with the help of ChatGPT

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../../Group-05/src/DataGrid.cpp"
#include "../../Group-05/src/Datum.cpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

using cse::DataGrid;
using cse::Datum;

static constexpr double kEpsilon = 0.0001;

/**
 * @brief Test parameterized constructor
 */
TEST_CASE("DataGrid: Parameterized Constructor", "[constructor]") {
  // Test with default double value
  DataGrid grid1(3, 4, 2.5);
  auto [rows1, cols1] = grid1.Shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 4);
  CHECK(grid1.GetValue(0, 0).GetDouble() == 2.5);
  CHECK(grid1.GetValue(2, 3).GetDouble() == 2.5);

  // Test with default string value
  DataGrid grid2(2, 3, "Hello");
  auto [rows2, cols2] = grid2.Shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 3);
  CHECK(grid2.GetValue(0, 0).GetString() == "Hello");
  CHECK(grid2.GetValue(1, 2).GetString() == "Hello");

  // Test with an empty grid
  DataGrid grid3(0, 0);
  auto [rows3, cols3] = grid3.Shape();
  CHECK(rows3 == 0);
  CHECK(cols3 == 0);

  // Test if all values are correctly initialized in a large grid
  DataGrid grid4(5, 5, 99.9);
  auto [rows4, cols4] = grid4.Shape();
  CHECK(rows4 == 5);
  CHECK(cols4 == 5);
  for (size_t i = 0; i < rows4; i++) {
    for (size_t j = 0; j < cols4; j++) {
      CHECK(grid4.GetValue(i, j).GetDouble() == 99.9);
    }
  }
}

/**
 * @brief Test copy and move constructors
 */
TEST_CASE("DataGrid: Copy and Move Constructors", "[constructor]") {
  // Create an original 2x3 grid with initialized values
  DataGrid grid1(2, 3, 7.5);
  auto [rows1, cols1] = grid1.Shape();
  CHECK(rows1 == 2);
  CHECK(cols1 == 3);
  CHECK(grid1.GetValue(0, 0).GetDouble() == 7.5);
  CHECK(grid1.GetValue(1, 2).GetDouble() == 7.5);

  // Copy constructor test
  DataGrid grid2(grid1);
  auto [rows2, cols2] = grid2.Shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 3);
  CHECK(grid2.GetValue(0, 0).GetDouble() == 7.5);
  CHECK(grid2.GetValue(1, 2).GetDouble() == 7.5);

  // Ensure modifying the copy does not affect the original
  grid2.At(0, 0) = Datum(99.9);
  CHECK(grid1.GetValue(0, 0).GetDouble() == 7.5);
  CHECK(grid2.GetValue(0, 0).GetDouble() == 99.9);

  // Move constructor test
  DataGrid grid3(std::move(grid1));
  auto [rows3, cols3] = grid3.Shape();
  CHECK(rows3 == 2);
  CHECK(cols3 == 3);
  CHECK(grid3.GetValue(0, 0).GetDouble() == 7.5);
  CHECK(grid3.GetValue(1, 2).GetDouble() == 7.5);

  // The moved-from grid1 should be in a valid but empty state
  auto [rows1_after_move, cols1_after_move] = grid1.Shape();
  CHECK(rows1_after_move == 0);
  CHECK(cols1_after_move == 0);
}

/**
 * @brief Test inserting default rows
 */
TEST_CASE("DataGrid: Insert Default Row", "[insert]") {
  // Create a 2x3 grid with default values
  DataGrid grid(2, 3, 1.0);
  auto [rows1, cols1] = grid.Shape();
  CHECK(rows1 == 2);
  CHECK(cols1 == 3);
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.GetValue(1, 2).GetDouble() == 1.0);

  // Insert a default row in the middle with value 5.0
  grid.InsertDefaultRow(1, 5.0);
  auto [rows2, cols2] = grid.Shape();
  CHECK(rows2 == 3);
  CHECK(cols2 == 3);

  // Ensure the inserted row has the correct values
  CHECK(grid.GetValue(1, 0).GetDouble() == 5.0);
  CHECK(grid.GetValue(1, 1).GetDouble() == 5.0);
  CHECK(grid.GetValue(1, 2).GetDouble() == 5.0);

  // Ensure previous rows remain unchanged
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.GetValue(2, 2).GetDouble() == 1.0);

  // Insert a new row at the beginning
  grid.InsertDefaultRow(0, 9.9);
  auto [rows3, cols3] = grid.Shape();
  CHECK(rows3 == 4);
  CHECK(cols3 == 3);
  CHECK(grid.GetValue(0, 0).GetDouble() == 9.9);
  CHECK(grid.GetValue(0, 2).GetDouble() == 9.9);

  // Insert a new row at the end
  grid.InsertDefaultRow(4, 3.3);
  auto [rows4, cols4] = grid.Shape();
  CHECK(rows4 == 5);
  CHECK(cols4 == 3);
  CHECK(grid.GetValue(4, 0).GetDouble() == 3.3);
  CHECK(grid.GetValue(4, 2).GetDouble() == 3.3);
}

/**
 * @brief Test inserting default columns
 */
TEST_CASE("DataGrid: Insert Default Column", "[insert]") {
  // Create a 3x2 grid with default value 1.0
  DataGrid grid(3, 2, 1.0);
  auto [rows1, cols1] = grid.Shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 2);
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.GetValue(2, 1).GetDouble() == 1.0);

  // Insert a default column in the middle with value 7.5
  grid.InsertDefaultColumn(1, 7.5);
  auto [rows2, cols2] = grid.Shape();
  CHECK(rows2 == 3);
  CHECK(cols2 == 3);

  // Ensure the inserted column has the correct values
  CHECK(grid.GetValue(0, 1).GetDouble() == 7.5);
  CHECK(grid.GetValue(1, 1).GetDouble() == 7.5);
  CHECK(grid.GetValue(2, 1).GetDouble() == 7.5);

  // Ensure previous columns remain unchanged
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.GetValue(2, 2).GetDouble() == 1.0);

  // Insert a new column at the beginning
  grid.InsertDefaultColumn(0, 9.9);
  auto [rows3, cols3] = grid.Shape();
  CHECK(rows3 == 3);
  CHECK(cols3 == 4);
  CHECK(grid.GetValue(0, 0).GetDouble() == 9.9);
  CHECK(grid.GetValue(2, 0).GetDouble() == 9.9);

  // Insert a new column at the end
  grid.InsertDefaultColumn(4, 3.3);
  auto [rows4, cols4] = grid.Shape();
  CHECK(rows4 == 3);
  CHECK(cols4 == 5);
  CHECK(grid.GetValue(0, 4).GetDouble() == 3.3);
  CHECK(grid.GetValue(2, 4).GetDouble() == 3.3);
}

/**
 * @brief Test inserting premade rows
 */
TEST_CASE("DataGrid: Insert Premade Row", "[insert]") {
  // Create an empty grid and insert a row
  DataGrid grid;
  std::vector<Datum> first_row = {Datum(1.1), Datum(2.2), Datum(3.3)};
  grid.InsertRow(first_row, 0);
  auto [rows1, cols1] = grid.Shape();
  CHECK(rows1 == 1);
  CHECK(cols1 == 3);
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.1);
  CHECK(grid.GetValue(0, 2).GetDouble() == 3.3);

  // Insert at the beginning (index 0)
  std::vector<Datum> new_row = {Datum(9.9), Datum(8.8), Datum(7.7)};
  grid.InsertRow(new_row, 0);
  auto [rows2, cols2] = grid.Shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 3);
  CHECK(grid.GetValue(0, 0).GetDouble() == 9.9);
  CHECK(grid.GetValue(1, 0).GetDouble() == 1.1);

  // Insert at the last index (append at end)
  std::vector<Datum> last_row = {Datum(4.4), Datum(5.5), Datum(6.6)};
  grid.InsertRow(last_row, 2);
  auto [rows3, cols3] = grid.Shape();
  CHECK(rows3 == 3);
  CHECK(cols3 == 3);
  CHECK(grid.GetValue(2, 0).GetDouble() == 4.4);
  CHECK(grid.GetValue(2, 2).GetDouble() == 6.6);
}

/**
 * @brief Test inserting premade columns
 */
TEST_CASE("DataGrid: Insert Premade Column", "[insert]") {
  // Create an empty grid and insert a column
  DataGrid grid;
  std::vector<Datum> first_column = {Datum(1.1), Datum(2.2), Datum(3.3)};
  grid.InsertColumn(first_column, 0);
  auto [rows1, cols1] = grid.Shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 1);
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.1);
  CHECK(grid.GetValue(2, 0).GetDouble() == 3.3);

  // Insert at the beginning (index 0)
  std::vector<Datum> new_column = {Datum(9.9), Datum(8.8), Datum(7.7)};
  grid.InsertColumn(new_column, 0);
  auto [rows2, cols2] = grid.Shape();
  CHECK(rows2 == 3);
  CHECK(cols2 == 2);
  CHECK(grid.GetValue(0, 0).GetDouble() == 9.9);
  CHECK(grid.GetValue(0, 1).GetDouble() == 1.1);

  // Insert at the last index (append at end)
  std::vector<Datum> last_column = {Datum(4.4), Datum(5.5), Datum(6.6)};
  grid.InsertColumn(last_column, 2);
  auto [rows3, cols3] = grid.Shape();
  CHECK(rows3 == 3);
  CHECK(cols3 == 3);
  CHECK(grid.GetValue(0, 2).GetDouble() == 4.4);
  CHECK(grid.GetValue(2, 2).GetDouble() == 6.6);
}

/**
 * @brief Test deleting rows
 */
TEST_CASE("DataGrid: Delete Row", "[delete]") {
  // Create a 3x3 grid with unique values for tracking
  DataGrid grid({{Datum(1), Datum(2), Datum(3)},
                 {Datum(4), Datum(5), Datum(6)},
                 {Datum(7), Datum(8), Datum(9)}});

  auto [rows1, cols1] = grid.Shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 3);

  // Delete the middle row
  grid.DeleteRow(1);
  auto [rows2, cols2] = grid.Shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 3);

  // Ensure the remaining rows are correctly shifted
  CHECK(grid.GetValue(0, 0).GetDouble() == 1);
  CHECK(grid.GetValue(1, 0).GetDouble() == 7);
  CHECK(grid.GetValue(1, 2).GetDouble() == 9);

  // Delete the first row
  grid.DeleteRow(0);
  auto [rows3, cols3] = grid.Shape();
  CHECK(rows3 == 1);
  CHECK(cols3 == 3);
  CHECK(grid.GetValue(0, 0).GetDouble() == 7);
  CHECK(grid.GetValue(0, 2).GetDouble() == 9);

  // Delete the last remaining row
  grid.DeleteRow(0);
  auto [rows4, cols4] = grid.Shape();
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

  auto [rows1, cols1] = grid.Shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 3);

  // Delete the middle column
  grid.DeleteColumn(1);
  auto [rows2, cols2] = grid.Shape();
  CHECK(rows2 == 3);
  CHECK(cols2 == 2);

  // Ensure the remaining columns are correctly shifted
  CHECK(grid.GetValue(0, 0).GetDouble() == 1);
  CHECK(grid.GetValue(0, 1).GetDouble() == 3);
  CHECK(grid.GetValue(1, 0).GetDouble() == 4);
  CHECK(grid.GetValue(1, 1).GetDouble() == 6);
  CHECK(grid.GetValue(2, 0).GetDouble() == 7);
  CHECK(grid.GetValue(2, 1).GetDouble() == 9);

  // Delete the first column
  grid.DeleteColumn(0);
  auto [rows3, cols3] = grid.Shape();
  CHECK(rows3 == 3);
  CHECK(cols3 == 1);
  CHECK(grid.GetValue(0, 0).GetDouble() == 3);
  CHECK(grid.GetValue(1, 0).GetDouble() == 6);
  CHECK(grid.GetValue(2, 0).GetDouble() == 9);

  // Delete the last remaining column
  grid.DeleteColumn(0);
  auto [rows4, cols4] = grid.Shape();
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

  auto [rows, cols] = grid.Shape();
  CHECK(rows == 3);
  CHECK(cols == 3);

  // Access first row
  auto &row1 = grid.GetRow(0);
  CHECK(row1.size() == 3);
  CHECK(row1[0].GetDouble() == 1);
  CHECK(row1[1].GetDouble() == 2);
  CHECK(row1[2].GetDouble() == 3);

  // Access middle row
  auto &row2 = grid.GetRow(1);
  CHECK(row2.size() == 3);
  CHECK(row2[0].GetDouble() == 4);
  CHECK(row2[1].GetDouble() == 5);
  CHECK(row2[2].GetDouble() == 6);

  // Access last row
  auto &row3 = grid.GetRow(2);
  CHECK(row3.size() == 3);
  CHECK(row3[0].GetDouble() == 7);
  CHECK(row3[1].GetDouble() == 8);
  CHECK(row3[2].GetDouble() == 9);

  // Ensure modifying the retrieved row affects the original grid
  row3[0] = Datum(99.9);
  CHECK(grid.GetValue(2, 0).GetDouble() == 99.9);
}

/**
 * @brief Test accessing column
 */
TEST_CASE("DataGrid: Get Column", "[access]") {
  // Create a 3x3 grid with unique values
  DataGrid grid({{Datum(1), Datum(2), Datum(3)},
                 {Datum(4), Datum(5), Datum(6)},
                 {Datum(7), Datum(8), Datum(9)}});

  auto [rows, cols] = grid.Shape();
  CHECK(rows == 3);
  CHECK(cols == 3);

  // Access first column
  auto column0 = grid.GetColumn(0);
  CHECK(column0.Size() == 3);
  CHECK(column0[0].GetDouble() == 1);
  CHECK(column0[1].GetDouble() == 4);
  CHECK(column0[2].GetDouble() == 7);

  // Access middle column
  auto column1 = grid.GetColumn(1);
  CHECK(column1.Size() == 3);
  CHECK(column1[0].GetDouble() == 2);
  CHECK(column1[1].GetDouble() == 5);
  CHECK(column1[2].GetDouble() == 8);

  // Access last column
  auto column2 = grid.GetColumn(2);
  CHECK(column2.Size() == 3);
  CHECK(column2[0].GetDouble() == 3);
  CHECK(column2[1].GetDouble() == 6);
  CHECK(column2[2].GetDouble() == 9);

  // Modify a value in the retrieved column and verify the original grid changes
  column1[1] = Datum(99.9);
  CHECK(grid.GetValue(1, 1).GetDouble() == 99.9);
}

/**
 * @brief Test accessing a specific value
 */
TEST_CASE("DataGrid: Get Value", "[access]") {
  // Create a 2x2 grid with default values
  DataGrid grid(2, 2, 0.0);

  auto [rows, cols] = grid.Shape();
  CHECK(rows == 2);
  CHECK(cols == 2);

  // Assign and retrieve a double value
  Datum d(42.0);
  grid.At(1, 1) = d;
  CHECK(grid.GetValue(1, 1).GetDouble() == 42.0);

  // Assign and retrieve a string value
  Datum s("Hello");
  grid.At(0, 0) = s;
  CHECK(grid.GetValue(0, 0).GetString() == "Hello");

  // Modify an existing value
  grid.At(1, 1) = Datum(99.9);
  CHECK(grid.GetValue(1, 1).GetDouble() == 99.9);

  // Verify other values remain unchanged
  CHECK(grid.GetValue(0, 1).GetDouble() == 0.0);
  CHECK(grid.GetValue(1, 0).GetDouble() == 0.0);
}

/**
 * @brief Test resizing
 */
TEST_CASE("DataGrid: Resize", "[resize]") {
  // Create a 2x2 grid with default value 1.0
  DataGrid grid(2, 2, 1.0);
  auto [rows1, cols1] = grid.Shape();
  CHECK(rows1 == 2);
  CHECK(cols1 == 2);
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.GetValue(1, 1).GetDouble() == 1.0);

  // Resize to a larger 4x5 grid with new default value 3.14
  grid.Resize(4, 5, 3.14);
  auto [rows2, cols2] = grid.Shape();
  CHECK(rows2 == 4);
  CHECK(cols2 == 5);

  // Verify old values are preserved
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.GetValue(1, 1).GetDouble() == 1.0);

  // Verify new cells are initialized to 3.14
  CHECK(grid.GetValue(2, 2).GetDouble() == 3.14);
  CHECK(grid.GetValue(3, 4).GetDouble() == 3.14);

  // Resize to a smaller 2x2 grid
  grid.Resize(2, 2, 0.0);
  auto [rows3, cols3] = grid.Shape();
  CHECK(rows3 == 2);
  CHECK(cols3 == 2);

  // Ensure resized grid still contains previous values within bounds
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.GetValue(1, 1).GetDouble() == 1.0);

  // Resize to a 3x3 grid with string default value "Empty"
  grid.Resize(3, 3, "Empty");
  auto [rows4, cols4] = grid.Shape();
  CHECK(rows4 == 3);
  CHECK(cols4 == 3);

  // Verify new string values
  CHECK(grid.GetValue(2, 2).GetString() == "Empty");
  CHECK(grid.GetValue(2, 1).GetString() == "Empty");

  // Ensure existing values remain unchanged
  CHECK(grid.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(grid.GetValue(1, 1).GetDouble() == 1.0);
}

/**
 * @brief Test clear function
 */
TEST_CASE("DataGrid: Clear", "[clear]") {
  // Create a 3x3 grid with default value 42.0
  DataGrid grid(3, 3, 42.0);
  auto [rows1, cols1] = grid.Shape();
  CHECK(rows1 == 3);
  CHECK(cols1 == 3);

  // Verify initial values
  CHECK(grid.GetValue(0, 0).GetDouble() == 42.0);
  CHECK(grid.GetValue(2, 2).GetDouble() == 42.0);

  // Clear the grid
  grid.Clear();
  auto [rows2, cols2] = grid.Shape();
  CHECK(rows2 == 0);
  CHECK(cols2 == 0);

  // Clear an already empty grid
  grid.Clear();
  auto [rows3, cols3] = grid.Shape();
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
  grid.SortColumn(0, true);

  CHECK(grid.GetValue(0, 0).GetDouble() == 1);
  CHECK(grid.GetValue(1, 0).GetDouble() == 2);
  CHECK(grid.GetValue(2, 0).GetDouble() == 3);

  // Verify the second column remains unchanged
  CHECK(grid.GetValue(0, 1).GetString() == "a");
  CHECK(grid.GetValue(1, 1).GetString() == "c");
  CHECK(grid.GetValue(2, 1).GetString() == "b");

  // Sort column 0 in descending order
  grid.SortColumn(0, false);
  CHECK(grid.GetValue(0, 0).GetDouble() == 3);
  CHECK(grid.GetValue(1, 0).GetDouble() == 2);
  CHECK(grid.GetValue(2, 0).GetDouble() == 1);
}

/**
 * @brief Test sorting the entire DataGrid row-wise
 */
TEST_CASE("DataGrid: Sort Entire Grid", "[sort]") {
  // Create a 3x2 grid with mixed types (numbers & strings)
  DataGrid grid(
      {{Datum(3), Datum("b")}, {Datum(1), Datum("a")}, {Datum(2), Datum("c")}});

  // Sort rows in ascending order (row-wise sorting)
  grid.Sort(true);

  CHECK(grid.GetValue(0, 0).GetDouble() == 1);
  CHECK(grid.GetValue(1, 0).GetDouble() == 2);
  CHECK(grid.GetValue(2, 0).GetDouble() == 3);

  CHECK(grid.GetValue(0, 1).GetString() == "a");
  CHECK(grid.GetValue(1, 1).GetString() == "c");
  CHECK(grid.GetValue(2, 1).GetString() == "b");

  // Sort rows in descending order
  grid.Sort(false);
  CHECK(grid.GetValue(0, 0).GetDouble() == 3);
  CHECK(grid.GetValue(1, 0).GetDouble() == 2);
  CHECK(grid.GetValue(2, 0).GetDouble() == 1);

  CHECK(grid.GetValue(0, 1).GetString() == "b");
  CHECK(grid.GetValue(1, 1).GetString() == "c");
  CHECK(grid.GetValue(2, 1).GetString() == "a");
}

/**
 * @brief Test sorting a column with mixed data types
 */
TEST_CASE("DataGrid: Sort Column with Mixed Data Types", "[sort]") {
  // Create a column with mixed data types (numbers & strings)
  DataGrid grid({{Datum("Apple")}, {Datum(10)}, {Datum("Banana")}, {Datum(5)}});

  // Sort column 0 in ascending order
  grid.SortColumn(0, true);

  // Ensure doubles come before strings in sorting
  CHECK(grid.GetValue(0, 0).GetDouble() == 5);
  CHECK(grid.GetValue(1, 0).GetDouble() == 10);
  CHECK(grid.GetValue(2, 0).GetString() == "Apple");
  CHECK(grid.GetValue(3, 0).GetString() == "Banana");

  // Sort column 0 in descending order
  grid.SortColumn(0, false);
  CHECK(grid.GetValue(0, 0).GetDouble() == 10);
  CHECK(grid.GetValue(1, 0).GetDouble() == 5);
  CHECK(grid.GetValue(2, 0).GetString() == "Banana");
  CHECK(grid.GetValue(3, 0).GetString() == "Apple");
}

/**
 * @brief Test merging two grids
 */
TEST_CASE("DataGrid: Merge", "[merge]") {
  // Create two 2x2 grids with different values
  DataGrid grid1({{Datum(1.0), Datum(2.0)}, {Datum(3.0), Datum(4.0)}});

  DataGrid grid2({{Datum(5.0), Datum(6.0)}, {Datum(7.0), Datum(8.0)}});

  // Merge the grids row-wise (stack below)
  DataGrid mergedRow = grid1.Merge(grid2, true);
  auto [rows1, cols1] = mergedRow.Shape();
  CHECK(rows1 == 4);
  CHECK(cols1 == 2);

  // Ensure values are correctly merged row-wise
  CHECK(mergedRow.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(mergedRow.GetValue(1, 1).GetDouble() == 4.0);
  CHECK(mergedRow.GetValue(2, 0).GetDouble() == 5.0);
  CHECK(mergedRow.GetValue(3, 1).GetDouble() == 8.0);

  // Merge the grids column-wise (stack side-by-side)
  DataGrid mergedCol = grid1.Merge(grid2, false);
  auto [rows2, cols2] = mergedCol.Shape();
  CHECK(rows2 == 2);
  CHECK(cols2 == 4);

  // Ensure values are correctly merged column-wise
  CHECK(mergedCol.GetValue(0, 0).GetDouble() == 1.0);
  CHECK(mergedCol.GetValue(0, 1).GetDouble() == 2.0);
  CHECK(mergedCol.GetValue(0, 2).GetDouble() == 5.0);
  CHECK(mergedCol.GetValue(0, 3).GetDouble() == 6.0);
  CHECK(mergedCol.GetValue(1, 2).GetDouble() == 7.0);
  CHECK(mergedCol.GetValue(1, 3).GetDouble() == 8.0);
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
  auto indices1 = grid.Search(2, Datum(10.0));
  CHECK(indices1.size() == 2);
  CHECK(indices1[0] == 0);
  CHECK(indices1[1] == 3);

  // Search for string "Apple" in column 1
  auto indices2 = grid.Search(1, Datum("Apple"));
  CHECK(indices2.size() == 2);
  CHECK(indices2[0] == 0);
  CHECK(indices2[1] == 2);

  // Search for a non-existent value (should return an empty list)
  auto indices3 = grid.Search(0, Datum(99.9));
  CHECK(indices3.empty());

  // Search for the value 5.0 in column 0 (multiple occurrences)
  auto indices4 = grid.Search(0, Datum(5.0));
  CHECK(indices4.size() == 2);
  CHECK(indices4[0] == 0);
  CHECK(indices4[1] == 2);

  // Search for string "Cherry" in column 1
  auto indices5 = grid.Search(1, Datum("Cherry"));
  CHECK(indices5.size() == 1);
  CHECK(indices5[0] == 3);

  // Test mixed-type column (numbers and strings)
  DataGrid mixedGrid({{Datum(10.0)}, {Datum("10")}, {Datum(20.0)}});

  // Ensure searching for number 10.0 does not match string "10"
  auto indices6 = mixedGrid.Search(0, Datum(10.0));
  CHECK(indices6.size() == 1);
  CHECK(indices6[0] == 0);

  // Ensure searching for string "10" does not match number 10.0
  auto indices7 = mixedGrid.Search(0, Datum("10"));
  CHECK(indices7.size() == 1);
  CHECK(indices7[0] == 1);

  // Test searching in an empty DataGrid
  DataGrid emptyGrid(0, 0);
  auto indices8 = emptyGrid.Search(0, Datum(10));
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
    DataGrid sliced = grid.Slice(1, 2, 1, 2);
    auto [rows, cols] = sliced.Shape();

    CHECK(rows == 2);
    CHECK(cols == 2);
    CHECK(sliced.GetValue(0, 0).GetDouble() == 6);
    CHECK(sliced.GetValue(0, 1).GetDouble() == 7);
    CHECK(sliced.GetValue(1, 0).GetDouble() == 10);
    CHECK(sliced.GetValue(1, 1).GetDouble() == 11);
  }

  SECTION("Edge case - Extract entire grid") {
    DataGrid sliced = grid.Slice(0, 3, 0, 3);
    auto [rows, cols] = sliced.Shape();
    CHECK(rows == 4);
    CHECK(cols == 4);
    CHECK(sliced.GetValue(3, 3).GetDouble() == 16);
  }

  SECTION("Edge case - Extract a single row") {
    DataGrid sliced = grid.Slice(2, 2, 0, 3);
    auto [rows, cols] = sliced.Shape();
    CHECK(rows == 1);
    CHECK(cols == 4);
    CHECK(sliced.GetValue(0, 2).GetDouble() == 11);
  }

  SECTION("Edge case - Extract a single column") {
    DataGrid sliced = grid.Slice(0, 3, 1, 1);
    auto [rows, cols] = sliced.Shape();
    CHECK(rows == 4);
    CHECK(cols == 1);
    CHECK(sliced.GetValue(3, 0).GetDouble() == 14);
  }

  SECTION("Edge case - Extract a single element") {
    DataGrid sliced = grid.Slice(1, 1, 2, 2);
    auto [rows, cols] = sliced.Shape();
    CHECK(rows == 1);
    CHECK(cols == 1);
    CHECK(sliced.GetValue(0, 0).GetDouble() == 7);
  }
}

/**
 * @brief Comprehensive test for DataGrid class under different conditions
 */
TEST_CASE("DataGrid: Comprehensive Test", "[comprehensive]") {
  // Step 1: Create a 5x5 grid with default value 1.0
  DataGrid grid(5, 5, 1.0);
  auto [rows1, cols1] = grid.Shape();
  CHECK(rows1 == 5);
  CHECK(cols1 == 5);

  // Step 2: Insert a default row at index 2 with value 2.0
  grid.InsertDefaultRow(2, 2.0);
  auto [rows2, cols2] = grid.Shape();
  CHECK(rows2 == 6);
  CHECK(cols2 == 5);

  // Verify inserted row values
  for (size_t j = 0; j < cols2; ++j) {
    CHECK(grid.GetValue(2, j).GetDouble() == 2.0);
  }

  // Step 3: Insert a default column at index 3 with value "Water"
  grid.InsertDefaultColumn(3, "Water");
  auto [rows3, cols3] = grid.Shape();
  CHECK(rows3 == 6);
  CHECK(cols3 == 6);

  // Verify inserted column values
  for (size_t i = 0; i < rows3; ++i) {
    CHECK(grid.GetValue(i, 3).GetString() == "Water");
  }

  // Step 4: Modify a specific value
  grid.At(4, 4) = Datum(9.99);
  CHECK(grid.GetValue(4, 4).GetDouble() == 9.99);

  // Step 5: Delete the first row
  grid.DeleteRow(0);
  auto [rows4, cols4] = grid.Shape();
  CHECK(rows4 == 5);
  CHECK(cols4 == 6);

  // Step 6: Delete the second column (was originally index 1)
  grid.DeleteColumn(1);
  auto [rows5, cols5] = grid.Shape();
  CHECK(rows5 == 5);
  CHECK(cols5 == 5);

  // Step 7: Resize the grid to 3x3 with default value 0.0
  grid.Resize(3, 3, 0.0);
  auto [rows6, cols6] = grid.Shape();
  CHECK(rows6 == 3);
  CHECK(cols6 == 3);

  // Step 8: Verify the last modified value persists if within bounds
  if (rows6 > 2 && cols6 > 2) {
    CHECK(grid.GetValue(2, 2).GetString() == "Water");
  }
}

/**
 * @brief Mathematical Tests (Except mode)
 */
TEST_CASE("DataGrid: Mathematical Functions", "[mathematics]") {
  // Cite: Used
  // https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
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
  CHECK_THAT(grid.ColumnMean(0), Catch::WithinAbs(-0.2, kEpsilon));
  CHECK(std::isnan(grid.ColumnMean(1)));
  CHECK_THAT(grid.ColumnMean(2), Catch::WithinAbs(95.25, kEpsilon));

  CHECK_THROWS(grid.ColumnMean(99));

  // Median Column Tests
  CHECK_THAT(grid.ColumnMedian(0), Catch::WithinAbs(3.5, kEpsilon));
  CHECK(std::isnan(grid.ColumnMedian(1)));
  CHECK_THAT(grid.ColumnMedian(2), Catch::WithinAbs(85.375, kEpsilon));

  CHECK_THROWS(grid.ColumnMedian(99));

  // Standard Deviation Column Tests
  CHECK_THAT(grid.ColumnStandardDeviation(0),
             Catch::WithinAbs(7.5056645275418, kEpsilon));
  CHECK(std::isnan(grid.ColumnStandardDeviation(1)));
  CHECK_THAT(grid.ColumnStandardDeviation(2),
             Catch::WithinAbs(81.968172176766, kEpsilon));

  CHECK_THROWS(grid.ColumnStandardDeviation(99));

  // Min Column Tests
  CHECK_THAT(grid.ColumnMin(0), Catch::WithinAbs(-15, kEpsilon));
  CHECK(std::isnan(grid.ColumnMax(1)));
  CHECK_THAT(grid.ColumnMin(2), Catch::WithinAbs(10.25, kEpsilon));

  CHECK_THROWS(grid.ColumnMin(99));

  // Max Column Tests
  CHECK_THAT(grid.ColumnMax(0), Catch::WithinAbs(5.0, kEpsilon));
  CHECK(std::isnan(grid.ColumnMax(1)));
  CHECK_THAT(grid.ColumnMax(2), Catch::WithinAbs(200, kEpsilon));

  CHECK_THROWS(grid.ColumnMax(99));

  DataGrid empty_data_grid;
  CHECK_THROWS(empty_data_grid.ColumnMean(99));
  CHECK_THROWS(empty_data_grid.ColumnMedian(99));
  CHECK_THROWS(empty_data_grid.ColumnStandardDeviation(99));
  CHECK_THROWS(empty_data_grid.ColumnMin(99));
  CHECK_THROWS(empty_data_grid.ColumnMax(99));
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

  std::vector<double> multipleModes = grid_mode.ColumnMode(0);
  std::sort(multipleModes.begin(), multipleModes.end());
  CHECK(multipleModes.size() == 2);
  CHECK_THAT(multipleModes[0], Catch::WithinAbs(1.25, kEpsilon));
  CHECK_THAT(multipleModes[1], Catch::WithinAbs(5.55, kEpsilon));

  CHECK(grid_mode.ColumnMode(1).empty());

  std::vector<double> singleMode = grid_mode.ColumnMode(2);
  CHECK(singleMode.size() == 1);
  CHECK_THAT(singleMode[0], Catch::WithinAbs(10.25, kEpsilon));

  CHECK_THROWS(grid_mode.ColumnMode(99));

  // Empty test
  DataGrid empty_data_grid;
  CHECK_THROWS(empty_data_grid.ColumnMode(99));
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

  DataGrid::DataGridMathSummary data_grid_math_summary =
      grid.CalculateDataGridMathSummary();

  CHECK_THAT(data_grid_math_summary.mean, Catch::WithinAbs(3.97222, kEpsilon));
  CHECK_THAT(data_grid_math_summary.median, Catch::WithinAbs(4.25, kEpsilon));
  CHECK_THAT(data_grid_math_summary.standardDeviation,
             Catch::WithinAbs(8.7022063329783, kEpsilon));
  CHECK_THAT(data_grid_math_summary.min, Catch::WithinAbs(-15, kEpsilon));
  CHECK_THAT(data_grid_math_summary.max, Catch::WithinAbs(20.25, kEpsilon));

  std::sort(data_grid_math_summary.mode.begin(),
            data_grid_math_summary.mode.end());
  CHECK(data_grid_math_summary.mode.size() == 2);
  CHECK_THAT(data_grid_math_summary.mode[0], Catch::WithinAbs(1.25, kEpsilon));
  CHECK_THAT(data_grid_math_summary.mode[1], Catch::WithinAbs(5.0, kEpsilon));

  // DataGrid with NaN
  std::vector<std::vector<Datum>> vector_nan(2, std::vector<Datum>(2));

  vector_nan[0][0] = Datum(5.0);
  vector_nan[1][0] = Datum(std::numeric_limits<double>::quiet_NaN());

  vector_nan[0][1] = Datum(5.0);
  vector_nan[1][1] = Datum(3.5);

  DataGrid nan_grid(vector_nan);

  DataGrid::DataGridMathSummary nan_data_grid_math_summary =
      nan_grid.CalculateDataGridMathSummary();

  CHECK_THAT(nan_data_grid_math_summary.mean, Catch::WithinAbs(4.5, kEpsilon));
  CHECK_THAT(nan_data_grid_math_summary.median, Catch::WithinAbs(5, kEpsilon));
  CHECK_THAT(nan_data_grid_math_summary.standardDeviation,
             Catch::WithinAbs(0.70710678118655, kEpsilon));
  CHECK_THAT(nan_data_grid_math_summary.min, Catch::WithinAbs(3.5, kEpsilon));
  CHECK_THAT(nan_data_grid_math_summary.max, Catch::WithinAbs(5.0, kEpsilon));

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

  DataGrid::DataGridMathSummary string_data_grid_math_summary =
      string_grid.CalculateDataGridMathSummary();

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
      grid.ColumnLessThan(0, Datum(16.0));
  CHECK(less_than_values_doubles.Size() == 3);
  CHECK(less_than_values_doubles[0] == Datum(5.0));
  CHECK(less_than_values_doubles[1] == Datum(10.0));
  CHECK(less_than_values_doubles[2] == Datum(15.0));

  cse::ReferenceVector<Datum> less_than_values_doubles1 =
      grid.ColumnLessThan(0, Datum(25.0));
  CHECK(less_than_values_doubles1.Size() == 4);
  CHECK(less_than_values_doubles1[0] == Datum(5.0));
  CHECK(less_than_values_doubles1[1] == Datum(10.0));
  CHECK(less_than_values_doubles1[2] == Datum(15.0));
  CHECK(less_than_values_doubles1[3] == Datum(20.0));

  cse::ReferenceVector<Datum> less_than_values_strings =
      grid.ColumnLessThan(1, Datum("f"));
  CHECK(less_than_values_strings.Size() == 3);
  CHECK(less_than_values_strings[0] == Datum("a"));
  CHECK(less_than_values_strings[1] == Datum("c"));
  CHECK(less_than_values_strings[2] == Datum("e"));

  cse::ReferenceVector<Datum> less_than_values_strings1 =
      grid.ColumnLessThan(1, Datum("i"));
  CHECK(less_than_values_strings1.Size() == 4);
  CHECK(less_than_values_strings1[0] == Datum("a"));
  CHECK(less_than_values_strings1[1] == Datum("c"));
  CHECK(less_than_values_strings1[2] == Datum("e"));
  CHECK(less_than_values_strings1[3] == Datum("g"));

  cse::ReferenceVector<Datum> less_than_values_mix_strings =
      grid.ColumnLessThan(2, Datum("bb"));
  CHECK(less_than_values_mix_strings.Size() == 1);
  CHECK(less_than_values_mix_strings[0] == Datum("aa"));

  cse::ReferenceVector<Datum> less_than_values_mix_doubles =
      grid.ColumnLessThan(2, Datum(100.0));
  CHECK(less_than_values_mix_doubles.Size() == 2);
  CHECK(less_than_values_mix_doubles[0] == Datum(55.55));
  CHECK(less_than_values_mix_doubles[1] == Datum(-150.50));

  // *** Less Than Or Equal Comparisons ***

  cse::ReferenceVector<Datum> less_than_or_equal_values_doubles =
      grid.ColumnLessThanOrEqual(0, Datum(16.0));
  CHECK(less_than_or_equal_values_doubles.Size() == 3);
  CHECK(less_than_or_equal_values_doubles[0] == Datum(5.0));
  CHECK(less_than_or_equal_values_doubles[1] == Datum(10.0));
  CHECK(less_than_or_equal_values_doubles[2] == Datum(15.0));

  cse::ReferenceVector<Datum> less_than_or_equal_values_doubles1 =
      grid.ColumnLessThanOrEqual(0, Datum(25.0));
  CHECK(less_than_or_equal_values_doubles1.Size() == 5);
  CHECK(less_than_or_equal_values_doubles1[0] == Datum(5.0));
  CHECK(less_than_or_equal_values_doubles1[1] == Datum(10.0));
  CHECK(less_than_or_equal_values_doubles1[2] == Datum(15.0));
  CHECK(less_than_or_equal_values_doubles1[3] == Datum(20.0));
  CHECK(less_than_or_equal_values_doubles1[4] == Datum(25.0));

  cse::ReferenceVector<Datum> less_than_or_equal_values_strings =
      grid.ColumnLessThanOrEqual(1, Datum("f"));
  CHECK(less_than_or_equal_values_strings.Size() == 3);
  CHECK(less_than_or_equal_values_strings[0] == Datum("a"));
  CHECK(less_than_or_equal_values_strings[1] == Datum("c"));
  CHECK(less_than_or_equal_values_strings[2] == Datum("e"));

  cse::ReferenceVector<Datum> less_than_or_equal_values_strings1 =
      grid.ColumnLessThanOrEqual(1, Datum("i"));
  CHECK(less_than_or_equal_values_strings1.Size() == 5);
  CHECK(less_than_or_equal_values_strings1[0] == Datum("a"));
  CHECK(less_than_or_equal_values_strings1[1] == Datum("c"));
  CHECK(less_than_or_equal_values_strings1[2] == Datum("e"));
  CHECK(less_than_or_equal_values_strings1[3] == Datum("g"));
  CHECK(less_than_or_equal_values_strings1[4] == Datum("i"));

  cse::ReferenceVector<Datum> less_than_or_equal_values_mix_strings =
      grid.ColumnLessThanOrEqual(2, Datum("bb"));
  CHECK(less_than_or_equal_values_mix_strings.Size() == 1);
  CHECK(less_than_or_equal_values_mix_strings[0] == Datum("aa"));

  cse::ReferenceVector<Datum> less_than_or_equal_values_mix_doubles =
      grid.ColumnLessThanOrEqual(2, Datum(100.0));
  CHECK(less_than_or_equal_values_mix_doubles.Size() == 2);
  CHECK(less_than_or_equal_values_mix_doubles[0] == Datum(55.55));
  CHECK(less_than_or_equal_values_mix_doubles[1] == Datum(-150.50));

  // *** Greater Than Comparisons ***

  cse::ReferenceVector<Datum> greater_than_values_doubles =
      grid.ColumnGreaterThan(0, Datum(16.0));
  CHECK(greater_than_values_doubles.Size() == 2);
  CHECK(greater_than_values_doubles[0] == Datum(20.0));
  CHECK(greater_than_values_doubles[1] == Datum(25.0));

  cse::ReferenceVector<Datum> greater_than_values_doubles1 =
      grid.ColumnGreaterThan(0, Datum(20.0));
  CHECK(greater_than_values_doubles1.Size() == 1);
  CHECK(greater_than_values_doubles1[0] == Datum(25.0));

  cse::ReferenceVector<Datum> greater_than_values_strings =
      grid.ColumnGreaterThan(1, Datum("f"));
  CHECK(greater_than_values_strings.Size() == 2);
  CHECK(greater_than_values_strings[0] == Datum("g"));
  CHECK(greater_than_values_strings[1] == Datum("i"));

  cse::ReferenceVector<Datum> greater_than_values_strings1 =
      grid.ColumnGreaterThan(1, Datum("c"));
  CHECK(greater_than_values_strings1.Size() == 3);
  CHECK(greater_than_values_strings1[0] == Datum("e"));
  CHECK(greater_than_values_strings1[1] == Datum("g"));
  CHECK(greater_than_values_strings1[2] == Datum("i"));

  cse::ReferenceVector<Datum> greater_than_values_mix_strings =
      grid.ColumnGreaterThan(2, Datum("bb"));
  CHECK(greater_than_values_mix_strings.Size() == 1);
  CHECK(greater_than_values_mix_strings[0] == Datum("cc"));

  cse::ReferenceVector<Datum> greater_than_values_mix_doubles =
      grid.ColumnGreaterThan(2, Datum(1.23));
  CHECK(greater_than_values_mix_doubles.Size() == 2);
  CHECK(greater_than_values_mix_doubles[0] == Datum(55.55));
  CHECK(greater_than_values_mix_doubles[1] == Datum(123.123));

  // *** Greater Than Or Equal Comparisons ***

  cse::ReferenceVector<Datum> greater_than_or_equal_values_doubles =
      grid.ColumnGreaterThanOrEqual(0, Datum(16.0));
  CHECK(greater_than_or_equal_values_doubles.Size() == 2);
  CHECK(greater_than_or_equal_values_doubles[0] == Datum(20.0));
  CHECK(greater_than_or_equal_values_doubles[1] == Datum(25.0));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_doubles1 =
      grid.ColumnGreaterThanOrEqual(0, Datum(20.0));
  CHECK(greater_than_or_equal_values_doubles1.Size() == 2);
  CHECK(greater_than_or_equal_values_doubles1[0] == Datum(20.0));
  CHECK(greater_than_or_equal_values_doubles1[1] == Datum(25.0));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_strings =
      grid.ColumnGreaterThanOrEqual(1, Datum("f"));
  CHECK(greater_than_or_equal_values_strings.Size() == 2);
  CHECK(greater_than_or_equal_values_strings[0] == Datum("g"));
  CHECK(greater_than_or_equal_values_strings[1] == Datum("i"));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_strings1 =
      grid.ColumnGreaterThanOrEqual(1, Datum("c"));
  CHECK(greater_than_or_equal_values_strings1.Size() == 4);
  CHECK(greater_than_or_equal_values_strings1[0] == Datum("c"));
  CHECK(greater_than_or_equal_values_strings1[1] == Datum("e"));
  CHECK(greater_than_or_equal_values_strings1[2] == Datum("g"));
  CHECK(greater_than_or_equal_values_strings1[3] == Datum("i"));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_mix_strings =
      grid.ColumnGreaterThanOrEqual(2, Datum("aa"));
  CHECK(greater_than_or_equal_values_mix_strings.Size() == 2);
  CHECK(greater_than_or_equal_values_mix_strings[0] == Datum("aa"));
  CHECK(greater_than_or_equal_values_mix_strings[1] == Datum("cc"));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_mix_doubles =
      grid.ColumnGreaterThanOrEqual(2, Datum(55.55));
  CHECK(greater_than_or_equal_values_mix_doubles.Size() == 2);
  CHECK(greater_than_or_equal_values_mix_doubles[0] == Datum(55.55));
  CHECK(greater_than_or_equal_values_mix_doubles[1] == Datum(123.123));

  // *** Equal Comparisons ***

  cse::ReferenceVector<Datum> equal_values_doubles =
      grid.ColumnEqual(0, Datum(16.0));
  CHECK(equal_values_doubles.Size() == 0);

  cse::ReferenceVector<Datum> equal_values_doubles1 =
      grid.ColumnEqual(0, Datum(20.0));
  CHECK(equal_values_doubles1.Size() == 1);
  CHECK(equal_values_doubles1[0] == Datum(20.0));

  cse::ReferenceVector<Datum> equal_values_strings =
      grid.ColumnEqual(1, Datum("f"));
  CHECK(equal_values_strings.Size() == 0);

  cse::ReferenceVector<Datum> equal_values_strings1 =
      grid.ColumnEqual(1, Datum("c"));
  CHECK(equal_values_strings1.Size() == 1);
  CHECK(equal_values_strings1[0] == Datum("c"));

  cse::ReferenceVector<Datum> equal_values_mix_strings =
      grid.ColumnEqual(2, Datum("aa"));
  CHECK(equal_values_mix_strings.Size() == 1);
  CHECK(equal_values_mix_strings[0] == Datum("aa"));

  cse::ReferenceVector<Datum> equal_values_mix_doubles =
      grid.ColumnEqual(2, Datum(55.55));
  CHECK(equal_values_mix_doubles.Size() == 1);
  CHECK(equal_values_mix_doubles[0] == Datum(55.55));

  // *** Not Equal Comparisons ***

  cse::ReferenceVector<Datum> not_equal_values_doubles =
      grid.ColumnNotEqual(0, Datum(16.0));
  CHECK(not_equal_values_doubles.Size() == 5);
  CHECK(not_equal_values_doubles[0] == Datum(5.0));
  CHECK(not_equal_values_doubles[1] == Datum(10.0));
  CHECK(not_equal_values_doubles[2] == Datum(15.0));
  CHECK(not_equal_values_doubles[3] == Datum(20.0));
  CHECK(not_equal_values_doubles[4] == Datum(25.0));

  cse::ReferenceVector<Datum> not_equal_values_doubles1 =
      grid.ColumnNotEqual(0, Datum(20.0));
  CHECK(not_equal_values_doubles1.Size() == 4);
  CHECK(not_equal_values_doubles1[0] == Datum(5.0));
  CHECK(not_equal_values_doubles1[1] == Datum(10.0));
  CHECK(not_equal_values_doubles1[2] == Datum(15.0));
  CHECK(not_equal_values_doubles1[3] == Datum(25.0));

  cse::ReferenceVector<Datum> not_equal_values_strings =
      grid.ColumnNotEqual(1, Datum("f"));
  CHECK(not_equal_values_strings.Size() == 5);
  CHECK(not_equal_values_strings[0] == Datum("a"));
  CHECK(not_equal_values_strings[1] == Datum("c"));
  CHECK(not_equal_values_strings[2] == Datum("e"));
  CHECK(not_equal_values_strings[3] == Datum("g"));
  CHECK(not_equal_values_strings[4] == Datum("i"));

  cse::ReferenceVector<Datum> not_equal_values_strings1 =
      grid.ColumnNotEqual(1, Datum("c"));
  CHECK(not_equal_values_strings1.Size() == 4);
  CHECK(not_equal_values_strings1[0] == Datum("a"));
  CHECK(not_equal_values_strings1[1] == Datum("e"));
  CHECK(not_equal_values_strings1[2] == Datum("g"));
  CHECK(not_equal_values_strings1[3] == Datum("i"));

  cse::ReferenceVector<Datum> not_equal_values_mix_strings =
      grid.ColumnNotEqual(2, Datum("aa"));
  CHECK(not_equal_values_mix_strings.Size() == 1);
  CHECK(not_equal_values_mix_strings[0] == Datum("cc"));

  cse::ReferenceVector<Datum> not_equal_values_mix_doubles =
      grid.ColumnNotEqual(2, Datum(55.55));
  CHECK(not_equal_values_mix_doubles.Size() == 2);
  CHECK(not_equal_values_mix_doubles[0] == Datum(-150.50));
  CHECK(not_equal_values_mix_doubles[1] == Datum(123.123));

  // Out of bounds column error check
  CHECK_THROWS(grid.ColumnLessThan(999, Datum(25.0)));

  // Checks if updating the comparison ReferenceVector updates the grid
  less_than_values_doubles[2] = Datum(100);
  CHECK(grid.At(2, 0) == Datum(100));

  less_than_or_equal_values_strings[0] = Datum(123.123);
  CHECK(grid.At(0, 1) == Datum(123.123));
}

TEST_CASE("NaN Comparison Tests", "[comparison]") {
  std::vector<std::vector<Datum>> comparison_nan_vector(3,
                                                        std::vector<Datum>(2));

  comparison_nan_vector[0][0] = Datum(5.0);
  comparison_nan_vector[1][0] = Datum(std::numeric_limits<double>::quiet_NaN());
  comparison_nan_vector[2][0] = Datum(15.0);

  comparison_nan_vector[0][1] = Datum("a");
  comparison_nan_vector[1][1] = Datum(std::numeric_limits<double>::quiet_NaN());
  comparison_nan_vector[2][1] = Datum("e");

  DataGrid nan_grid(comparison_nan_vector);

  // *** Less Than Comparisons ***

  cse::ReferenceVector<Datum> less_than_values_doubles_nan =
      nan_grid.ColumnLessThan(0, Datum(12.0));
  CHECK(less_than_values_doubles_nan.Size() == 1);
  CHECK(less_than_values_doubles_nan[0] == Datum(5.0));

  cse::ReferenceVector<Datum> less_than_values_strings_nan =
      nan_grid.ColumnLessThan(1, Datum("b"));
  CHECK(less_than_values_strings_nan.Size() == 1);
  CHECK(less_than_values_strings_nan[0] == Datum("a"));

  cse::ReferenceVector<Datum> less_than_values_nan = nan_grid.ColumnLessThan(
      0, Datum(std::numeric_limits<double>::quiet_NaN()));
  CHECK(less_than_values_nan.Size() == 0);

  // *** Less Than Or Equal Comparisons ***

  cse::ReferenceVector<Datum> less_than_or_equal_values_doubles_nan =
      nan_grid.ColumnLessThanOrEqual(0, Datum(12.0));
  CHECK(less_than_or_equal_values_doubles_nan.Size() == 1);
  CHECK(less_than_or_equal_values_doubles_nan[0] == Datum(5.0));

  cse::ReferenceVector<Datum> less_than_or_equal_values_strings_nan =
      nan_grid.ColumnLessThanOrEqual(1, Datum("b"));
  CHECK(less_than_or_equal_values_strings_nan.Size() == 1);
  CHECK(less_than_or_equal_values_strings_nan[0] == Datum("a"));

  cse::ReferenceVector<Datum> less_than_or_equal_values_nan =
      nan_grid.ColumnLessThanOrEqual(
          0, Datum(std::numeric_limits<double>::quiet_NaN()));
  CHECK(less_than_or_equal_values_nan.Size() == 0);

  // *** Greater Than Comparisons ***

  cse::ReferenceVector<Datum> greater_than_values_doubles_nan =
      nan_grid.ColumnGreaterThan(0, Datum(-12.12));
  CHECK(greater_than_values_doubles_nan.Size() == 2);
  CHECK(greater_than_values_doubles_nan[0] == Datum(5.0));
  CHECK(greater_than_values_doubles_nan[1] == Datum(15.0));

  cse::ReferenceVector<Datum> greater_than_values_strings_nan =
      nan_grid.ColumnGreaterThan(1, Datum("b"));
  CHECK(greater_than_values_strings_nan.Size() == 1);
  CHECK(greater_than_values_strings_nan[0] == Datum("e"));

  cse::ReferenceVector<Datum> greater_than_values_nan =
      nan_grid.ColumnGreaterThan(
          0, Datum(std::numeric_limits<double>::quiet_NaN()));
  CHECK(greater_than_values_nan.Size() == 0);

  // *** Greater Than Or Equal Comparisons ***

  cse::ReferenceVector<Datum> greater_than_or_equal_values_doubles_nan =
      nan_grid.ColumnGreaterThanOrEqual(0, Datum(-12.12));
  CHECK(greater_than_or_equal_values_doubles_nan.Size() == 2);
  CHECK(greater_than_or_equal_values_doubles_nan[0] == Datum(5.0));
  CHECK(greater_than_or_equal_values_doubles_nan[1] == Datum(15.0));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_strings_nan =
      nan_grid.ColumnGreaterThanOrEqual(1, Datum("b"));
  CHECK(greater_than_or_equal_values_strings_nan.Size() == 1);
  CHECK(greater_than_or_equal_values_strings_nan[0] == Datum("e"));

  cse::ReferenceVector<Datum> greater_than_or_equal_values_nan =
      nan_grid.ColumnGreaterThanOrEqual(
          0, Datum(std::numeric_limits<double>::quiet_NaN()));
  CHECK(greater_than_or_equal_values_nan.Size() == 0);

  // *** Equal Comparisons ***

  cse::ReferenceVector<Datum> equal_values_doubles_nan =
      nan_grid.ColumnEqual(0, Datum(5.0));
  CHECK(equal_values_doubles_nan.Size() == 1);
  CHECK(equal_values_doubles_nan[0] == Datum(5.0));

  cse::ReferenceVector<Datum> equal_values_strings_nan =
      nan_grid.ColumnEqual(1, Datum("a"));
  CHECK(equal_values_strings_nan.Size() == 1);
  CHECK(equal_values_strings_nan[0] == Datum("a"));

  cse::ReferenceVector<Datum> equal_values_nan =
      nan_grid.ColumnEqual(0, Datum(std::numeric_limits<double>::quiet_NaN()));
  CHECK(equal_values_nan.Size() == 0);

  // *** Not Equal Comparisons ***

  cse::ReferenceVector<Datum> not_equal_values_doubles_nan =
      nan_grid.ColumnNotEqual(0, Datum(5.0));
  CHECK(not_equal_values_doubles_nan.Size() == 2);
  CHECK(std::isnan(not_equal_values_doubles_nan[0].GetDouble()));
  CHECK(not_equal_values_doubles_nan[1] == Datum(15.0));

  cse::ReferenceVector<Datum> not_equal_values_strings_nan =
      nan_grid.ColumnNotEqual(1, Datum("a"));
  CHECK(not_equal_values_strings_nan.Size() == 1);
  CHECK(not_equal_values_strings_nan[0] == Datum("e"));

  cse::ReferenceVector<Datum> not_equal_values_nan = nan_grid.ColumnNotEqual(
      0, Datum(std::numeric_limits<double>::quiet_NaN()));
  CHECK(not_equal_values_nan.Size() == 3);
}
