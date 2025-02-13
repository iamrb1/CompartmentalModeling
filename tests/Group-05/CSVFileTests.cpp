/**
 * @file CSVFileTests.cpp
 *
 * @author Muhammad Masood
 *
 * Tests for the CSV class.
 */

#include "../../Group-05/src/CSVfile.cpp"
#include "../../Group-05/src/CSVfile.h"
#include "../../Group-05/src/DataGrid.cpp"
#include "../../Group-05/src/DataGrid.h"
#include "../../Group-05/src/Datum.cpp"
#include "../../Group-05/src/Datum.h"
#include "../../Group-05/src/ReferenceVector.cpp"
#include "../../Group-05/src/ReferenceVector.h"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include <cstdio>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>

using cse::DataGrid;
using cse::Datum;

TEST_CASE("CSVFile LoadCsv - Valid File", "[CSVFile]") {
  // Create a temporary CSV file for testing
  std::string test_file_name = "test_csv_input.csv";
  {
    std::ofstream out_file(test_file_name);
    REQUIRE(out_file.is_open());
    // Write CSV content with a header and two data rows
    out_file << "Name,Age,Score\n";
    out_file << "Alice,30,95.5\n";
    out_file << "Bob,25,88.0\n";
    out_file.close();
  }

  // Load the CSV file into a DataGrid
  cse::DataGrid data_grid;
  try {
    data_grid = cse::CSVFile::LoadCsv(test_file_name);
  } catch (const std::exception &e) {
    FAIL("Exception thrown during LoadCsv: " << e.what());
  }

  // Check that the grid has 3 rows and 3 columns
  std::tuple<const std::size_t, const std::size_t> grid_shape =
      data_grid.shape();
  REQUIRE(std::get<0>(grid_shape) == 3);
  REQUIRE(std::get<1>(grid_shape) == 3);

  // Validate header row
  {
    std::vector<cse::Datum> row0 = data_grid.getRow(0);
    REQUIRE(row0.size() == 3);
    CHECK(row0[0].GetString().value() == "Name");
    CHECK(row0[1].GetString().value() == "Age");
    CHECK(row0[2].GetString().value() == "Score");
  }
  // Validate first data row
  {
    std::vector<cse::Datum> row1 = data_grid.getRow(1);
    REQUIRE(row1.size() == 3);
    CHECK(row1[0].GetString().value() == "Alice");
    CHECK(row1[1].GetDouble().value() == Approx(30.0));
    CHECK(row1[2].GetDouble().value() == Approx(95.5));
  }
  // Validate second data row
  {
    std::vector<cse::Datum> row2 = data_grid.getRow(2);
    REQUIRE(row2.size() == 3);
    CHECK(row2[0].GetString().value() == "Bob");
    CHECK(row2[1].GetDouble().value() == Approx(25.0));
    CHECK(row2[2].GetDouble().value() == Approx(88.0));
  }

  // Cleanup temporary file
  std::remove(test_file_name.c_str());
}

TEST_CASE("CSVFile ExportCsv - Valid DataGrid", "[CSVFile]") {
  // Create a DataGrid with known content
  cse::DataGrid data_grid(3, 3);

  // Fill header row
  data_grid.at(0, 0) = cse::Datum("Name");
  data_grid.at(0, 1) = cse::Datum("Age");
  data_grid.at(0, 2) = cse::Datum("Score");

  // Fill first data row
  data_grid.at(1, 0) = cse::Datum("Charlie");
  data_grid.at(1, 1) = cse::Datum(28);
  data_grid.at(1, 2) = cse::Datum(91.0);

  // Fill second data row
  data_grid.at(2, 0) = cse::Datum("Diana");
  data_grid.at(2, 1) = cse::Datum(32);
  data_grid.at(2, 2) = cse::Datum(85.5);

  // Export the DataGrid to a temporary CSV file
  std::string test_file_name = "test_csv_output.csv";
  try {
    bool export_success = cse::CSVFile::ExportCsv(test_file_name, data_grid);
    REQUIRE(export_success);
  } catch (const std::exception &e) {
    FAIL("Exception thrown during ExportCsv: " << e.what());
  }

  // Read back the exported file and check its content
  std::ifstream in_file(test_file_name);
  REQUIRE(in_file.is_open());
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(in_file, line)) {
    lines.push_back(line);
  }
  in_file.close();

  // The CSV file should have 3 lines corresponding to 3 rows
  REQUIRE(lines.size() == 3);
  // Check header row (no sanitization needed if there are no delimiters or
  // quotes)
  CHECK(lines[0] == "Name,Age,Score");
  // Note: std::to_string produces six decimals for doubles by default
  CHECK(lines[1] == "Charlie,28.000000,91.000000");
  CHECK(lines[2] == "Diana,32.000000,85.500000");

  // Cleanup temporary file
  std::remove(test_file_name.c_str());
}

TEST_CASE("CSVFile LoadCsv - File Not Found", "[CSVFile]") {
  // Attempt to load a non-existent CSV file and expect an exception
  std::string non_existent_file = "non_existent_file.csv";
  CHECK_THROWS_AS(cse::CSVFile::LoadCsv(non_existent_file), std::runtime_error);
}
