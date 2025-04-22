/**
 * @file FinalApplicationTests.cpp
 *
 * @author Max Krawec
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../Group-05/FinalApplication.cpp"

#include "../../Group-05/src/DataGrid.cpp"
#include "../../Group-05/src/Datum.cpp"
#include "../../Group-05/src/CSVfile.cpp"

TEST_CASE("MathMenu()", "[math_menu]") {
  std::vector<std::vector<cse::Datum>> test_grid(
      5, std::vector<cse::Datum>(1));

  test_grid[0][0] = cse::Datum(10.25);
  test_grid[1][0] = cse::Datum("test1");
  test_grid[2][0] = cse::Datum(150.50);
  test_grid[3][0] = cse::Datum("test2");
  test_grid[4][0] = cse::Datum(20.25);

  cse::DataGrid grid(test_grid);
  FinalApplication final_application;


  // ** Mean **

  // Valid input
  // CITE: Used ChatGPT to learn how to write the os and is code. The test cases were manually written
  std::istringstream is("cmean\n0\nb\n");
  std::ostringstream os;
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Mean at column 0") != std::string::npos);

  // Invalid grid index - int
  is.str("cmean\n99\n0\nb\n");
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
  std::vector<std::vector<cse::Datum>> test_grid(
      5, std::vector<cse::Datum>(1));

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
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Valid input (string)
  is.str("clt\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("clt\n99\n0\n50.50\n");
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
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Valid input (string)
  is.str("clte\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("clte\n99\n0\n50.50\n");
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
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Valid input (string)
  is.str("cgt\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("cgt\n99\n0\n50.50\n");
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
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Valid input (string)
  is.str("cgte\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("cgte\n99\n0\n50.50\n");
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
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Valid input (string)
  is.str("ce\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("ce\n99\n0\n50.50\n");
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
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Valid input (string)
  is.str("cne\n0\ntest2\n");
  os.clear();
  final_application.ComparisonMenu(grid, os, is);
  CHECK(os.str().find("Values less than given value") != std::string::npos);

  // Invalid column (number - int)
  is.str("cne\n99\n0\n50.50\n");
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
  std::vector<std::vector<cse::Datum>> test_grid(
      5, std::vector<cse::Datum>(1));

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
  CHECK(os.str().find("Invalid choice. Must be between 0-6. Try again.") != std::string::npos);

  // Invalid input (number - double)
  is.str("1.1\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Invalid choice. Must be between 0-6. Try again.") != std::string::npos);

  // Invalid string
  is.str("test\n0\n0\n");
  os.clear();
  final_application.ManipulateGridMenu(grid, os, is);
  CHECK(os.str().find("Invalid choice. Cannot be a string. Try again.") != std::string::npos);
}