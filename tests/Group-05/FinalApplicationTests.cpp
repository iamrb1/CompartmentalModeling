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

// CITE: Used ChatGPT to help write with the os and is code
TEST_CASE("MathMenu()", "[math_menu]") {
  std::vector<std::vector<cse::Datum>> test_grid(
      5, std::vector<cse::Datum>(3));

  test_grid[0][0] = cse::Datum(5.0);
  test_grid[1][0] = cse::Datum(3.5);
  test_grid[2][0] = cse::Datum(1.25);
  test_grid[3][0] = cse::Datum(-15);
  test_grid[4][0] = cse::Datum(4.25);

  test_grid[0][1] = cse::Datum("test1");
  test_grid[1][1] = cse::Datum("test2");
  test_grid[2][1] = cse::Datum("test4");
  test_grid[3][1] = cse::Datum("test5");
  test_grid[4][1] = cse::Datum("test6");

  test_grid[0][2] = cse::Datum(10.25);
  test_grid[1][2] = cse::Datum("test3");
  test_grid[2][2] = cse::Datum(150.50);
  test_grid[3][2] = cse::Datum(200);
  test_grid[4][2] = cse::Datum(20.25);

  cse::DataGrid grid(test_grid);

  FinalApplication final_application;


  // ** Mean **

  // Valid input
  std::istringstream is("cmean\n0\nb\n");
  std::ostringstream os;
  final_application.MathMenu(grid, os, is);
  CHECK(os.str().find("Mean at column 0") != std::string::npos);

  // Invalid grid index
  is.str("cmean\n99\n0\nb\n");
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

  // Invalid grid index
  is.str("cmed\n99\n0\nb\n");
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

  // Invalid grid index
  is.str("csd\n99\n0\nb\n");
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

  // Invalid grid index
  is.str("cmin\n99\n0\nb\n");
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

  // Invalid grid index
  is.str("cmax\n99\n0\nb\n");
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

  // Invalid grid index
  is.str("cmode\n99\n0\nb\n");
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

  // Invalid grid index
  is.str("sum\n99\n0\nb\n");
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
