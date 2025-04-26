/**
 * @file FinalApplicationTests.cpp
 *
 * @author Max Krawec, Shahaab Ali
 */

#include "../../Group-05/FinalApplication.cpp"
#include "../../Group-05/src/CSVfile.cpp"
#include "../../Group-05/src/DataGrid.cpp"
#include "../../Group-05/src/Datum.cpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

//
// Test CreateGridMenu invalid inputs
// Developed with ChatGPT as a guide
//
TEST_CASE("CreateGridMenu: invalid inputs", "[CreateGridMenu]") {
  FinalApplication app;
  std::istringstream is(
      "\n\n"                    // blank lines before first prompt
      "abc\n"                   // non-numeric for rows
      "99.99.99\n"              // irregular numeric for rows
      "2.5\n"                   // decimal for integer rows
      "-1\n"                    // negative for rows
      "1001\n"                  // too large for rows
      "5\n"                     // valid rows
      "def\n"                   // non-numeric for columns
      "1.1.1\n"                 // irregular numeric for columns
      "3.14\n"                  // decimal for integer columns
      "-2\n"                    // negative for columns
      "1001\n"                  // too large for columns
      "4\n"                     // valid columns
      "n\n"                     // choose numeric default
      "foo\n"                   // non-numeric default
      "88..88\n"                // irregular numeric default
      "1e21\n"                  // too large number, invalid
      "12345678901234567890\n"  // very large number, valid
  );
  std::ostringstream os;
  auto grid = app.CreateGridMenu(os, is);

  // Should return a 5×4 grid with default value 1.2345678901234568e+19
  CHECK(grid.Shape() == std::make_tuple(std::size_t(5), std::size_t(4)));
  CHECK(grid.GetValue(2, 3).AsDouble() == Approx(1.2345678901234568e+19));

  const auto out = os.str();
  CHECK(out.find("Invalid input. Please enter a positive integer.") !=
        std::string::npos);
  CHECK(
      out.find("Invalid input. Please enter an integer between 1 and 1000.") !=
      std::string::npos);
  CHECK(out.find("Invalid input. Please enter a valid number.") !=
        std::string::npos);
  CHECK(out.find("Please enter a number between") != std::string::npos);
}

//
// Test CreateGridMenu invalid inputs + string default
// Developed with ChatGPT as a guide
//
TEST_CASE("CreateGridMenu: invalid inputs + string default",
          "[CreateGridMenu]") {
  FinalApplication app;
  // prepare a string of 101 characters
  std::string long_str(101, 'x');
  std::istringstream is(
      "1\n"  // valid rows
      "1\n"  // valid columns
      "s\n"  // choose string default
      "\n"   // empty string
      + long_str +
      "\n"       // too-long string
      "hello\n"  // valid default
  );
  std::ostringstream os;
  auto grid = app.CreateGridMenu(os, is);

  // Should return a 1×1 grid with default string "hello"
  CHECK(grid.Shape() == std::make_tuple(std::size_t(1), std::size_t(1)));
  CHECK(grid.GetValue(0, 0).AsString() == "hello");

  const auto out = os.str();
  CHECK(out.find("Please enter a non-empty string.") != std::string::npos);
  CHECK(out.find("Invalid input. String too long") != std::string::npos);
}

//
// Test GridMenu invalid menu options and test grid
// Developed with ChatGPT as a guide
//
TEST_CASE("GridMenu: invalid menu options and test grid", "[GridMenu]") {
  FinalApplication app;
  std::ostringstream os;
  std::istringstream is(
      "\n"     // blank line before first prompt
      "123\n"  // invalid (numeric)
      "?\n"    // invalid (punctuation)
      "abc\n"  // invalid (word)
      "t\n"    // valid: premade
  );
  auto grid = app.GridMenu(os, is);

  // Check if test grid was returned (5×3 premade grid)
  auto [r, c] = grid.Shape();
  CHECK(r == 5);
  CHECK(c == 3);
  CHECK(grid.GetValue(0, 0).AsDouble() == Approx(5.0));
  CHECK(grid.GetValue(1, 1).AsString() == "test2");

  REQUIRE(os.str().find("Invalid option. Try again.") != std::string::npos);
}

//
// Test GridMenu CSV import
// Developed with ChatGPT as a guide
//
TEST_CASE("GridMenu imports CSV correctly when file exists", "[GridMenu]") {
  // Create a temporary CSV file
  const char* fname = "temp_test.csv";
  {
    std::ofstream fout(fname);
    fout << "10,hello\n20,world\n";  // csv file values
  }

  FinalApplication app;
  std::ostringstream os;
  std::istringstream is(
      "i\n"              // choose import
      "temp_test.csv\n"  // file name
  );
  auto grid = app.GridMenu(os, is);

  // Remove temp file
  std::remove(fname);

  // CSV has 2 rows, 2 columns
  auto [r, c] = grid.Shape();
  CHECK(r == 2);
  CHECK(c == 2);

  // Check parsed values
  CHECK(grid.GetValue(0, 0).AsDouble() == Approx(10.0));
  CHECK(grid.GetValue(0, 1).AsString() == "hello");
  CHECK(grid.GetValue(1, 0).AsDouble() == Approx(20.0));
  CHECK(grid.GetValue(1, 1).AsString() == "world");
}

//
// Test GridMenu import CSV failure then use test grid
// Developed with ChatGPT as a guide
//
TEST_CASE("GridMenu handles import failure and retries menu", "[GridMenu]") {
  FinalApplication app;
  std::ostringstream os;
  std::istringstream is(
      "i\n"                // attempt import
      "nonexistent.csv\n"  // bad filename
      "t\n"                // fallback to premade
  );
  auto grid = app.GridMenu(os, is);

  // Should fall back to premade 5×3
  auto [r, c] = grid.Shape();
  CHECK(r == 5);
  CHECK(c == 3);

  // Ensure the failure message was printed
  REQUIRE(os.str().find("Import failed:") != std::string::npos);
}

TEST_CASE("MathMenu()", "[math_menu]") {
  std::vector<std::vector<cse::Datum>> test_grid(5, std::vector<cse::Datum>(1));

  test_grid[0][0] = cse::Datum(10.25);
  test_grid[1][0] = cse::Datum("test1");
  test_grid[2][0] = cse::Datum(150.50);
  test_grid[3][0] = cse::Datum("test2");
  test_grid[4][0] = cse::Datum(20.25);

  cse::DataGrid grid(test_grid);
  FinalApplication final_application;

  // ** Mean **

  // Valid input
  // CITE: Used ChatGPT to learn how to write the os and is code. The test cases
  // were manually written
  std::istringstream is("cmean\n0\nb\n");
  std::ostringstream os;
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Mean at column 0") != std::string::npos);

  // Invalid grid index - int
  is.str("cmean\n99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - int (negative)
  is.str("cmean\n-99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - double
  is.str("cmean\n99.99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid input (words)
  is.str("cmean\nhello!\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Median **

  // Valid input
  is.str("cmed\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Median at column 0") != std::string::npos);

  // Invalid grid index - int
  is.str("cmed\n99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - int (negative)
  is.str("cmed\n-99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - double
  is.str("cmed\n99.99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid input (words)
  is.str("cmed\nhello!\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Standard Deviation **

  // Valid input
  is.str("csd\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Standard deviation at column 0") != std::string::npos);

  // Invalid grid index - int
  is.str("csd\n99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - int (negative)
  is.str("csd\n-99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - double
  is.str("csd\n99.99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid input (words)
  is.str("csd\nhello!\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Minimum **

  // Valid input
  is.str("cmin\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Min at column 0") != std::string::npos);

  // Invalid grid index - int
  is.str("cmin\n99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - int (negative)
  is.str("cmin\n-99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - double
  is.str("cmin\n99.99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid input (words)
  is.str("cmin\nhello!\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Maximum **

  // Valid input
  is.str("cmax\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Max at column 0") != std::string::npos);

  // Invalid grid index - int
  is.str("cmax\n99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - int (negative)
  is.str("cmax\n-99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - double
  is.str("cmax\n99.99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid input (words)
  is.str("cmax\nhello!\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Mode **

  // Valid input
  is.str("cmode\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Mode(s) at column 0") != std::string::npos);

  // Invalid grid index - int
  is.str("cmode\n99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - int (negative)
  is.str("cmax\n-99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - double
  is.str("cmode\n99.99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid input (words)
  is.str("cmode\nhello!\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Summary **

  // Valid input
  is.str("sum\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Grid Summary") != std::string::npos);

  // Invalid grid index - int
  is.str("sum\n99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - int (negative)
  is.str("sum\n-99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid grid index - double
  is.str("sum\n99.99\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid input (words)
  is.str("sum\nhello!\n0\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Extra tests

  // Invalid option (text)
  is.str("Hello!\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid option (numerical)
  is.str("1\nb\n");
  os.clear();
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);
}

TEST_CASE("ComparisonMenu()", "[comparison_menu]") {
  std::vector<std::vector<cse::Datum>> test_grid(5, std::vector<cse::Datum>(1));

  test_grid[0][0] = cse::Datum(10.25);
  test_grid[1][0] = cse::Datum("test1");
  test_grid[2][0] = cse::Datum(150.50);
  test_grid[3][0] = cse::Datum("test2");
  test_grid[4][0] = cse::Datum(20.25);

  cse::DataGrid grid(test_grid);
  FinalApplication final_application;

  // ** Less Than **

  // Valid input (number)
  std::istringstream is("clt\n0\n50.50\n");
  std::ostringstream os;
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Valid input (string)
  is.str("clt\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("clt\n99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - negative int )
  is.str("clt\n-99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - double)
  is.str("clt\n99.99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (string)
  is.str("clt\ntest\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Less Than Or Equal **

  // Valid input (number)
  is.str("clte\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Valid input (string)
  is.str("clte\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("clte\n99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - negative int )
  is.str("clte\n-99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - double)
  is.str("clte\n99.99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (string)
  is.str("clte\ntest\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Greater Than **

  // Valid input (number)
  is.str("cgt\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Valid input (string)
  is.str("cgt\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("cgt\n99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - negative int )
  is.str("cgt\n-99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - double)
  is.str("cgt\n99.99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (string)
  is.str("cgt\ntest\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Greater Than Or Equal **

  // Valid input (number)
  is.str("cgte\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Valid input (string)
  is.str("cgte\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("cgte\n99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - negative int )
  is.str("cgte\n-99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - double)
  is.str("cgte\n99.99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (string)
  is.str("cgte\ntest\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Equal **

  // Valid input (number)
  is.str("ce\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Valid input (string)
  is.str("ce\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("ce\n99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - negative int )
  is.str("ce\n-99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - double)
  is.str("ce\n99.99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (string)
  is.str("ce\ntest\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // ** Not Equal **

  // Valid input (number)
  is.str("cne\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Valid input (string)
  is.str("cne\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than the given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("cne\n99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - negative int )
  is.str("cne\n-99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (number - double)
  is.str("cne\n99.99\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column (string)
  is.str("cne\ntest\n0\n50.50\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Extra tests

  // Invalid option (text)
  is.str("Hello!\nb\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid option (numerical - int)
  is.str("1\nb\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid option (numerical - double)
  is.str("1.5\nb\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);
}

TEST_CASE("ManipulateGridMenu()", "[Manipulate_grid_menu]") {
  std::vector<std::vector<cse::Datum>> test_grid(5, std::vector<cse::Datum>(1));

  test_grid[0][0] = cse::Datum(10.25);
  test_grid[1][0] = cse::Datum("test1");
  test_grid[2][0] = cse::Datum(150.50);
  test_grid[3][0] = cse::Datum("test2");
  test_grid[4][0] = cse::Datum(20.25);

  cse::DataGrid grid(test_grid);
  FinalApplication final_application;

  // Print Option
  std::istringstream is("1\n0\n0\n");
  std::ostringstream os;
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Print Options") != std::string::npos);

  // Edit Option
  is.str("2\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Edit Options") != std::string::npos);

  // Sort Option
  is.str("3\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Sort Options") != std::string::npos);

  // Add Option
  is.str("4\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Adding Options") != std::string::npos);

  // Delete Option
  is.str("5\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Deleting Options") != std::string::npos);

  // Resize Option
  is.str("6\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Resizing Options") != std::string::npos);

  // Extra

  // Invalid input (number - int)
  is.str("99\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Invalid choice. Must be between 0-6. Try again.") !=
        std::string::npos);

  // Invalid input (number - double)
  is.str("1.1\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Invalid choice. Must be between 0-6. Try again.") !=
        std::string::npos);

  // Invalid string
  is.str("test\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Invalid choice. Cannot be a string. Try again.") !=
        std::string::npos);
}

TEST_CASE("SortSubmenu()", "[sort_submenu]") {
  std::vector<std::vector<cse::Datum>> test_grid(5, std::vector<cse::Datum>(1));

  test_grid[0][0] = cse::Datum(10.25);
  test_grid[1][0] = cse::Datum("test1");
  test_grid[2][0] = cse::Datum(150.50);
  test_grid[3][0] = cse::Datum("test2");
  test_grid[4][0] = cse::Datum(20.25);

  cse::DataGrid grid(test_grid);
  FinalApplication final_application;


  // ** Sort Grid by Column **

  // Print Option
  // Valid input (descending)
  std::istringstream is("1\n0\n0\n0\n");
  std::ostringstream os;
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Grid rows sorted by column") != std::string::npos);

  // Valid input (ascending)
  is.str("1\n0\n1\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Grid rows sorted by column") != std::string::npos);

  // Invalid column input - out of range
  is.str("1\n99\n0\n0\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column input - double
  is.str("1\n99.99\n0\n0\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column input - negative
  is.str("1\n-1\n0\n0\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid column input - string
  is.str("1\nHello\n0\n0\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid option. Try again.") != std::string::npos);

  // Invalid input ascending/descending - out of bounds
  is.str("1\n0\n99\n1\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid input. The input must be 0 or 1") != std::string::npos);

  // Invalid input ascending/descending - negative
  is.str("1\n0\n-99\n1\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid input. The input must be 0 or 1") != std::string::npos);

  // Invalid input ascending/descending - string
  is.str("1\n0\ntester\n1\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid input. The input must be 0 or 1") != std::string::npos);


  // ** Sort entire grid **

  // Valid input (descending)
  is.str("2\n0\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Entire grid sorted.") != std::string::npos);

  // Valid input (ascending)
  is.str("2\n1\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Entire grid sorted.") != std::string::npos);

  // Invalid input ascending/descending - out of bounds
  is.str("2\n1\n99\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid input. The input must be 0 or 1") != std::string::npos);

  // Invalid input ascending/descending - negative
  is.str("2\n1\n-99\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid input. The input must be 0 or 1") != std::string::npos);

  // Invalid input ascending/descending - string
  is.str("2\n1\nHello\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid input. The input must be 0 or 1") != std::string::npos);


  // Extra

  // Invalid input (number - int)
  is.str("99\n1\n0\n0\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid choice. Input must be between 0-2.") !=
      std::string::npos);

  // Invalid input (number - negative)
  is.str("-99\n1\n0\n0\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid choice. Input must be between 0-2.") !=
      std::string::npos);

  // Invalid input (number - double)
  is.str("123.123\n1\n0\n0\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid choice. Input must be between 0-2.") !=
      std::string::npos);

  // Invalid string
  is.str("Hello!\n1\n0\n0\n0\n");
  os.clear();
  final_application.SortSubmenu(grid, os, is);
  CHECK(os.str().find("Invalid choice. Cannot be a string. Try again.") !=
      std::string::npos);
}

TEST_CASE("MainMenu()", "[main_menu]") {
  // ** Main Menu **
  // Valid Input

  // Valid input (export)
  FinalApplication final_application;
  std::istringstream is("t\nx\ntest.csv\nq\n");
  std::ostringstream os;
  final_application.MainMenu(os, is);
  CHECK(os.str().find("Exported to") != std::string::npos);

  // Invalid input (export)
  is.str("t\nx\ntest\ntest.csv\nq\n");
  os.clear();
  final_application.MainMenu(os, is);
  CHECK(os.str().find("Invalid filename. The file must end with .csv") != std::string::npos);

  // Valid input (Edit/Manipulation)
  is.str("t\ne\n0\nq\n");
  os.clear();
  final_application.MainMenu(os, is);
  CHECK(os.str().find("CSV Grid Manipulation Menu") != std::string::npos);

  // Valid input (Math)
  is.str("t\nm\nb\nq\n");
  os.clear();
  final_application.MainMenu(os, is);
  CHECK(os.str().find("Math Menu") != std::string::npos);

  // Valid input (Comparisons)
  is.str("t\nc\nb\nq\n");
  os.clear();
  final_application.MainMenu(os, is);
  CHECK(os.str().find("Comparison Menu") != std::string::npos);


  // Extra

  // Invalid input (number - int)
  is.str("t\n99\nq\n");
  os.clear();
  final_application.MainMenu(os, is);
  CHECK(os.str().find("Invalid option. Try again") !=
      std::string::npos);

  // Invalid input (number - negative)
  is.str("t\n-99\nq\n");
  os.clear();
  final_application.MainMenu(os, is);
  CHECK(os.str().find("Invalid option. Try again") !=
      std::string::npos);

  // Invalid input (number - double)
  is.str("t\n99.123\nq\n");
  os.clear();
  final_application.MainMenu(os, is);
  CHECK(os.str().find("Invalid option. Try again") !=
      std::string::npos);

  // Invalid string
  is.str("t\nHello!\nq\n");
  os.clear();
  final_application.MainMenu(os, is);
  CHECK(os.str().find("Invalid option. Try again.") !=
      std::string::npos);
}

TEST_CASE("PrintSubmenu handles bad then good input", "[PrintSubmenu]") {
  // reusing the 2×2 zero grid
  cse::DataGrid grid(2, 2, 0.0);

  SECTION("bad menu choice then exit") {
    std::istringstream is("9\n0\n");
    std::ostringstream os;
    FinalApplication::PrintSubmenu(grid, os, is);
    REQUIRE(os.str().find("Invalid choice") != std::string::npos);
  }

  SECTION("print whole grid then exit") {
    std::istringstream is("4\n0\n");
    std::ostringstream os;
    FinalApplication::PrintSubmenu(grid, os, is);
    // at least one "0 " in the printed grid
    REQUIRE(os.str().find("0 ") != std::string::npos);
  }

  SECTION("print single cell") {
    // 1 = print cell; row=0, col=1; then exit
    std::istringstream is("1\n0\n1\n0\n");
    std::ostringstream os;
    FinalApplication::PrintSubmenu(grid, os, is);
    REQUIRE(os.str().find("Cell (0, 1): 0") != std::string::npos);
  }
  
  SECTION("print row") {
    std::istringstream is("2\n1\n0\n");
    std::ostringstream os;
    FinalApplication::PrintSubmenu(grid, os, is);
    REQUIRE(os.str().find("Row 1: 0 0") != std::string::npos);
  }
  
  SECTION("print column") {
    std::istringstream is("3\n1\n0\n");
    std::ostringstream os;
    FinalApplication::PrintSubmenu(grid, os, is);
    REQUIRE(os.str().find("Column 1: 0 0") != std::string::npos);
  }
}

TEST_CASE("EditSubmenu handles bad then all three edit modes", "[EditSubmenu]") {
  cse::DataGrid grid(2, 2, 0.0);

  SECTION("invalid menu choice then exit") {
    std::istringstream is("7\n0\n");
    std::ostringstream os;
    FinalApplication::EditSubmenu(grid, os, is);
    REQUIRE(os.str().find("Invalid choice") != std::string::npos);
  }

  SECTION("edit single cell") {
    // 1 = edit cell, row=1,col=1,newVal=3.14, then 0 to quit
    std::istringstream is("1\n1\n1\n3.14\n0\n");
    std::ostringstream os;
    FinalApplication::EditSubmenu(grid, os, is);
    CHECK(grid.GetValue(1,1).AsDouble() == Approx(3.14));
    REQUIRE(os.str().find("Cell updated") != std::string::npos);
  }

  SECTION("edit entire row") {
    // 2 = row, row=0, values "7 8", then 0
    std::istringstream is("2\n0\n7 8\n0\n");
    std::ostringstream os;
    FinalApplication::EditSubmenu(grid, os, is);
    CHECK(grid.GetValue(0,0).AsDouble() == Approx(7.0));
    CHECK(grid.GetValue(0,1).AsDouble() == Approx(8.0));
    REQUIRE(os.str().find("Row updated") != std::string::npos);
  }

  SECTION("edit entire column") {
    // 3 = column, col=1, values "9 10", then 0
    std::istringstream is("3\n1\n9 10\n0\n");
    std::ostringstream os;
    FinalApplication::EditSubmenu(grid, os, is);
    CHECK(grid.GetValue(0,1).AsDouble() == Approx(9.0));
    CHECK(grid.GetValue(1,1).AsDouble() == Approx(10.0));
    REQUIRE(os.str().find("Column updated") != std::string::npos);
  }

  SECTION("edit single cell with non-numeric value") {
    // 1 = cell edit; row=0,col=0; bad value, leaves default 0.0; and then it will exit
    std::istringstream is("1\n0\n0\nfoo\n0\n");
    std::ostringstream os;
    FinalApplication::EditSubmenu(grid, os, is);
    // still 0.0, but with no crash
    CHECK(grid.GetValue(0,0).AsDouble() == Approx(0.0));
    REQUIRE(os.str().find("Cell updated") != std::string::npos);
  }
}