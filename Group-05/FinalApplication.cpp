/**
 * @file FinalApplication.cpp
 * @brief The CSV Command Line Manipulator.
 *
 * @authors Max Krawec, Calen Green, Pedro Mitkiewicz, Shahaab Ali, and Muhammad
 * Asif Masood
 *
 *
 * This application allows users to create and manipulate data within a grid
 * through the command line. Users can create the grid by using a .csv file,
 * inputting a custom grid, or selecting a pre-made grid.
 *
 * Users can manipulate the grid in several ways:
 * - Edit specific cell values, rows, or columns
 * - Add default, manual, or custom rows/columns
 * - Delete specific rows/columns
 * - Sort the grid
 * - Resize the grid
 * - Print the grid
 * - Calculate the mean, median, standard deviation, mode, min, and max of
 * columns or the entire grid
 * - Use comparison operations (less than, less than or equal to, greater than,
 * greater than or equal to, equal to, and not equal to) to compare a given
 * value against values in a column
 *
 * When done, users can export the grid into a .csv file.
 *
 *
 * Portions of this code are made with the assistance of ChatGPT
 */

#include "FinalApplication.h"

#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "src/CSVfile.h"
#include "src/DataGrid.h"
#include "src/Datum.h"
#include "src/ExpressionParser.cpp"
#include "src/ExpressionParser.h"

/**
 * Checks if a string can be converted into a double
 * @param test_string The string to check
 * @return an optional double if the string can be converted, empty optional if
 * not
 */
std::optional<double> FinalApplication::IsValidDouble(
    const std::string &test_string) {
  try {
    std::size_t pos;
    double value = std::stod(test_string, &pos);
    if (pos == test_string.length()) {
      return value;
    }
  } catch (std::invalid_argument &) {
  }
  return std::nullopt;
}

/**
 * Checks if a string can be converted into an int
 * @param test_string The string to check
 * @return an optional int if the string can be converted, empty optional if not
 */
std::optional<int> FinalApplication::IsValidInt(
    const std::string &test_string) {
  try {
    std::size_t pos;
    int value = std::stoi(test_string, &pos);
    if (pos == test_string.length()) {
      return value;
    }
  } catch (std::invalid_argument &) {
  }
  return std::nullopt;
}

/**
 * Prompts the user for a valid column index
 * @param max_index The maximum index for the grid
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return A valid column index for the grid
 */
int FinalApplication::GetColumnIndex(int max_index,
                                     std::ostream &os = std::cout,
                                     std::istream &is = std::cin) {
  while (true) {
    os << "Please enter column index: ";
    std::string index_str;
    is >> index_str;
    auto index = IsValidInt(index_str);
    if (index && index.value() < max_index) {
      return index.value();
    }
    os << "Invalid option. Try again. The max index is: " << max_index
       << std::endl;
  }
}

/**
 * Prompts the user for a Datum value
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return A datum value
 */
cse::Datum FinalApplication::GetDataValue(std::ostream &os = std::cout,
                                          std::istream &is = std::cin) {
  os << "Please enter the value to compare: ";
  std::string value_str;
  is >> value_str;
  if (auto d = IsValidDouble(value_str)) {
    return {d.value()};
  }
  return {value_str};
}

/**
 * Prints the column
 * @param column a column vector for printing
 * @param os ostream used for output and testing
 */
void FinalApplication::PrintColumn(
    const cse::ReferenceVector<cse::Datum> &column,
    std::ostream &os = std::cout) {
  for (const auto &datum : column) {
    if (datum.IsDouble()) {
      os << datum.GetDouble() << " ";
    } else {
      os << datum.GetString() << " ";
    }
  }
  os << std::endl;
}

/**
 * Displays the create grid menu that prompts the user to create a custom
 * DataGrid
 * Cite: Made with the help of ChatGPT
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return The custom DataGrid
 */
cse::DataGrid FinalApplication::CreateGridMenu(std::ostream &os = std::cout,
                                               std::istream &is = std::cin) {
  static constexpr int MAX_ROWS = 1000;
  static constexpr int MAX_COLS = 1000;
  static constexpr int MAX_DEFAULT_STR_LEN = 100;
  static constexpr double MAX_DEFAULT_VALUE = 1e20;

  auto trim = [&](const std::string &s) {
    const auto ws = " \t\r\n";
    auto first = s.find_first_not_of(ws);
    if (first == std::string::npos) return std::string{};
    auto last = s.find_last_not_of(ws);
    return s.substr(first, last - first + 1);
  };

  std::size_t num_rows = 0, num_columns = 0;
  std::string line, token, type_choice;

  // Read number of rows
  while (true) {
    os << "Enter number of rows for your DataGrid [1–" << MAX_ROWS << "]: ";
    if (!std::getline(is, line)) {
      is.clear();
      continue;
    }
    token = trim(line);
    // must be all digits
    if (token.empty() || !std::ranges::all_of(token, ::isdigit)) {
      os << "Invalid input. Please enter a positive integer." << std::endl;
      continue;
    }
    try {
      auto v = std::stoi(token);
      if (v == 0 || v > MAX_ROWS) {
        os << "Invalid input. Please enter an integer between 1 and "
           << MAX_ROWS << "." << std::endl;
        continue;
      }
      num_rows = v;
      break;
    } catch (...) {
      os << "Invalid input. Please enter a positive integer." << std::endl;
    }
  }

  // Read number of columns
  while (true) {
    os << "Enter number of columns for your DataGrid [1–" << MAX_COLS << "]: ";
    if (!std::getline(is, line)) {
      is.clear();
      continue;
    }
    token = trim(line);
    if (token.empty() || !std::ranges::all_of(token, ::isdigit)) {
      os << "Invalid input. Please enter a positive integer." << std::endl;
      continue;
    }
    try {
      auto v = std::stoi(token);
      if (v == 0 || v > MAX_COLS) {
        os << "Invalid input. Please enter an integer between 1 and "
           << MAX_COLS << "." << std::endl;
        continue;
      }
      num_columns = v;
      break;
    } catch (...) {
      os << "Invalid input. Please enter a positive integer." << std::endl;
    }
  }

  // Choose default value type
  while (true) {
    os << "Would you like your default values to be numerical or strings? "
          "[n/s]: ";
    if (!std::getline(is, line)) {
      is.clear();
      continue;
    }
    type_choice = trim(line);
    if (type_choice == "n") {
      // numeric default
      while (true) {
        os << "Enter a numeric default value for the DataGrid: ";
        if (!std::getline(is, line)) {
          is.clear();
          continue;
        }
        token = trim(line);

        std::istringstream ss(token);
        double d;
        if (!(ss >> d) || !(ss >> std::ws).eof()) {
          os << "Invalid input. Please enter a valid number." << "\n";
          continue;
        }

        if (std::fabs(d) > MAX_DEFAULT_VALUE) {
          os << "Invalid input. Please enter a number between "
             << -MAX_DEFAULT_VALUE << " and " << MAX_DEFAULT_VALUE << ".\n";
          continue;
        }

        return cse::DataGrid(num_rows, num_columns, d);
      }
    } else if (type_choice == "s") {
      // string default
      while (true) {
        os << "Enter a string default value for the DataGrid (1–"
           << MAX_DEFAULT_STR_LEN << " chars): ";
        if (!std::getline(is, line)) {
          is.clear();
          continue;
        }
        token = trim(line);
        if (token.empty()) {
          os << "Invalid input. Please enter a non-empty string." << std::endl;
          continue;
        }
        if (token.size() > MAX_DEFAULT_STR_LEN) {
          os << "Invalid input. String too long (max " << MAX_DEFAULT_STR_LEN
             << " characters)." << std::endl;
          continue;
        }
        return {num_rows, num_columns, token};
      }
    } else {
      os << "Invalid selection. Type 'n' for numeric or 's' for string."
         << std::endl;
    }
  }
}

/**
 * Displays the grid menu and prompts the user for a DataGrid.
 * The DataGrid can be made by using a csv file, custom creation, or pre-made
 * example
 * Cite: Made with the help of ChatGPT
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return A DataGrid to use
 */
cse::DataGrid FinalApplication::GridMenu(std::ostream &os = std::cout,
                                         std::istream &is = std::cin) {
  // Helper to trim whitespace
  auto trim = [&](const std::string &s) {
    const auto ws = " \t\r\n";
    auto first = s.find_first_not_of(ws);
    if (first == std::string::npos) return std::string{};
    auto last = s.find_last_not_of(ws);
    return s.substr(first, last - first + 1);
  };

  std::string line;
  while (true) {
    os << "Menu Option:\n"
       << "i: Import a CSV file\n"
       << "c: Create a new DataGrid\n"
       << "t: Use a pre-made DataGrid\n"
       << "Enter an option: ";
    if (!std::getline(is, line)) {
      is.clear();
      continue;
    }
    std::string choice = trim(line);

    if (choice == "i") {
      // Import CSV path
      os << "Enter CSV filename to import: ";
      if (!std::getline(is, line)) {
        is.clear();
        continue;
      }
      const auto filename = trim(line);
      try {
        return cse::CSVFile::LoadCsv(filename);
      } catch (const std::exception &e) {
        os << "Import failed: " << e.what() << std::endl;
        // loop back to menu
      }
    } else if (choice == "t") {
      // Pre-made 5×3 grid
      std::vector<std::vector<cse::Datum>> premade(5,
                                                   std::vector<cse::Datum>(3));
      premade[0][0] = 5.0;
      premade[1][0] = 3.5;
      premade[2][0] = 1.25;
      premade[3][0] = -15;
      premade[4][0] = 4.25;
      premade[0][1] = "test1";
      premade[1][1] = "test2";
      premade[2][1] = "test4";
      premade[3][1] = "test5";
      premade[4][1] = "test6";
      premade[0][2] = 10.25;
      premade[1][2] = "test3";
      premade[2][2] = 150.50;
      premade[3][2] = 200;
      premade[4][2] = 20.25;
      return cse::DataGrid(premade);
    } else if (choice == "c") {
      // Delegate to the robust CreateGridMenu
      return CreateGridMenu();
    } else {
      os << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * Displays the math menu and outputs the results from the mathematical
 * operation based on the user input. Supported operations: Mean, Median, Mode,
 * Standard Deviation, Min, Max, and Summary of the grid
 * @param grid The DataGrid used for the calculations
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenu(const cse::DataGrid &grid,
                                std::ostream &os = std::cout,
                                std::istream &is = std::cin) {
  while (true) {
    os << "\nMath Menu:" << std::endl;
    os << "cmean: Calculate the mean of a column" << std::endl;
    os << "cmed: Calculate the median of a column" << std::endl;
    os << "cmode: Calculate the mode of a column" << std::endl;
    os << "csd: Calculate the standard deviation of a column" << std::endl;
    os << "cmin: Calculate the min of a column" << std::endl;
    os << "cmax: Calculate the max of a column" << std::endl;
    os << "sum: Calculate all statistics for the grid" << std::endl;
    os << "b: Go back" << std::endl;
    os << "Enter option: ";
    std::string option;
    is >> option;

    if (option == "cmean") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Mean at column " << index << ": " << grid.ColumnMean(index)
         << std::endl;
      return;
    } else if (option == "cmed") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Median at column " << index << ": " << grid.ColumnMedian(index)
         << std::endl;
      return;
    } else if (option == "csd") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Standard deviation at column " << index << ": "
         << grid.ColumnStandardDeviation(index) << std::endl;
      return;
    } else if (option == "cmin") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Min at column " << index << ": " << grid.ColumnMin(index)
         << std::endl;
      return;
    } else if (option == "cmax") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Max at column " << index << ": " << grid.ColumnMax(index)
         << std::endl;
      return;
    } else if (option == "cmode") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Mode(s) at column " << index << ": ";
      for (const double &mode_val : grid.ColumnMode(index)) {
        os << mode_val << " ";
      }
      os << std::endl;
      return;
    } else if (option == "sum") {
      auto summary = grid.CalculateDataGridMathSummary();
      os << "Grid Summary:" << std::endl;
      os << "Mean: " << summary.mean << std::endl;
      os << "Median: " << summary.median << std::endl;
      os << "Standard Deviation: " << summary.standardDeviation << std::endl;
      os << "Min: " << summary.min << std::endl;
      os << "Max: " << summary.max << std::endl;
      os << "Mode(s): ";
      for (const double &mode_val : summary.mode) {
        os << mode_val << " ";
      }
      os << std::endl;
      return;
    } else if (option == "b") {
      return;
    } else {
      os << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * Displays the comparison menu and outputs the results from the comparison
 * based on the user input. Supported comparisons: Less than, Less than or
 * equal, Greater than, Greater than or equal, equal, not equal
 * @param grid The DataGrid used for the calculations
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenu(cse::DataGrid &grid,
                                      std::ostream &os = std::cout,
                                      std::istream &is = std::cin) {
  while (true) {
    os << "\nComparison Menu:" << std::endl;
    os << "clt: Find values less than a given value for a column" << std::endl;
    os << "clte: Find values less than or equal to a given value for a column"
       << std::endl;
    os << "cgt: Find values greater than a given value for a column"
       << std::endl;
    os << "cgte: Find values greater than or equal to a given value for a "
          "column"
       << std::endl;
    os << "ce: Find values equal to a value for a column" << std::endl;
    os << "cne: Find values not equal to a given value for a column"
       << std::endl;
    os << "b: Go back" << std::endl;
    os << "Enter option: ";
    std::string option;
    is >> option;

    if (option == "clt") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      cse::Datum datum = GetDataValue(os, is);
      os << "Values less than given value:" << std::endl;
      PrintColumn(grid.ColumnLessThan(index, datum));
      return;
    } else if (option == "clte") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      cse::Datum datum = GetDataValue(os, is);
      os << "Values less than or equal to given value:" << std::endl;
      PrintColumn(grid.ColumnLessThanOrEqual(index, datum));
      return;
    } else if (option == "cgt") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      cse::Datum datum = GetDataValue(os, is);
      os << "Values greater than given value:" << std::endl;
      PrintColumn(grid.ColumnGreaterThan(index, datum));
      return;
    } else if (option == "cgte") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      cse::Datum datum = GetDataValue(os, is);
      os << "Values greater than or equal to given value:" << std::endl;
      PrintColumn(grid.ColumnGreaterThanOrEqual(index, datum));
      return;
    } else if (option == "ce") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      cse::Datum datum = GetDataValue(os, is);
      os << "Values equal to given value:" << std::endl;
      PrintColumn(grid.ColumnEqual(index, datum));
      return;
    } else if (option == "cne") {
      int index =
          GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      cse::Datum datum = GetDataValue(os, is);
      os << "Values not equal to given value:" << std::endl;
      PrintColumn(grid.ColumnNotEqual(index, datum));
      return;
    } else if (option == "b") {
      return;
    } else {
      os << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * Displays the manipulate grid menu and performs the changes on the DataGrid
 * based on the user input. Supported options: Print, Edit, Sort, Add, Delete,
 * and Resize
 * @param grid The DataGrid that gets manipulated
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ManipulateGridMenu(cse::DataGrid &grid,
                                          std::ostream &os = std::cout,
                                          std::istream &is = std::cin) {
  int choice = -1;
  do {
    os << "\n=== CSV Grid Manipulation Menu ===" << std::endl;
    os << "1. Print options" << std::endl;
    os << "2. Edit options" << std::endl;
    os << "3. Sort options" << std::endl;
    os << "4. Adding options" << std::endl;
    os << "5. Deleting options" << std::endl;
    os << "6. Resizing options" << std::endl;
    os << "0. Return to main menu" << std::endl;
    os << "Enter your choice: ";
    std::string input;
    is >> input;

    std::optional<int> input_int = IsValidInt(input);

    if (input_int.has_value()) {
      choice = input_int.value();
      switch (input_int.value()) {
        case 1:
          PrintSubmenu(grid, os, is);
          break;
        case 2:
          EditSubmenu(grid, os, is);
          break;
        case 3:
          SortSubmenu(grid, os, is);
          break;
        case 4:
          AddSubmenu(grid, os, is);
          break;
        case 5:
          DeleteSubmenu(grid, os, is);
          break;
        case 6:
          ResizeSubmenu(grid, os, is);
          break;
        case 0:
          break;
        default:
          os << "Invalid choice. Must be between 0-6. Try again." << std::endl;
      }
    } else {
      os << "Invalid choice. Cannot be a string. Try again." << std::endl;
    }
  } while (choice != 0);
}

/**
 * Displays the print menu and outputs the results from the print based on the
 * user input. Supported options: Print cell value, row, column, or entire grid
 * @param grid The DataGrid to print
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::PrintSubmenu(const cse::DataGrid &grid,
                                    std::ostream &os = std::cout,
                                    std::istream &is = std::cin) {
  int choice = 0;
  do {
    os << "\n--- Print Options ---" << std::endl;
    os << "1. Print a cell value" << std::endl;
    os << "2. Print a row" << std::endl;
    os << "3. Print a column" << std::endl;
    os << "4. Print entire grid" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    is >> choice;

    try {
      switch (choice) {
        // Print a cell value
        case 1: {
          std::size_t row = 0, col = 0;
          os << "Enter row index: ";
          is >> row;
          os << "Enter column index: ";
          is >> col;
          const cse::Datum &value = grid.GetValue(row, col);
          if (value.IsDouble()) {
            os << "Cell (" << row << ", " << col << "): " << value.GetDouble()
               << std::endl;
          } else if (value.IsString()) {
            os << "Cell (" << row << ", " << col << "): " << value.GetString()
               << std::endl;
          }
          break;
        }
        // Print a row
        case 2: {
          std::size_t row = 0;
          os << "Enter row index: ";
          is >> row;
          auto row_data = grid.GetRow(row);
          os << "Row " << row << ": ";
          for (const auto &datum : row_data) {
            if (datum.IsDouble()) {
              os << datum.GetDouble() << " ";
            } else if (datum.IsString()) {
              os << datum.GetString() << " ";
            }
          }
          os << std::endl;
          break;
        }
        // Print a column
        case 3: {
          std::size_t col = 0;
          os << "Enter column index: ";
          is >> col;
          auto col_data = grid.GetColumn(col);
          os << "Column " << col << ": ";
          for (const auto &datum : col_data) {
            if (datum.IsDouble()) {
              os << datum.GetDouble() << " ";
            } else if (datum.IsString()) {
              os << datum.GetString() << " ";
            }
          }
          os << std::endl;
          break;
        }
        // Print the entire DataGrid
        case 4:
          grid.Print(os);
          break;
        case 0:
          break;
        default:
          os << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * Displays the edit grid menu and edits the DataGrid based on the user input.
 * Supported options: Edit a cell, row, or column
 * @param grid The DataGrid to edit
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::EditSubmenu(cse::DataGrid &grid,
                                   std::ostream &os = std::cout,
                                   std::istream &is = std::cin) {
  int choice = 0;
  do {
    os << "\n--- Edit Options ---" << std::endl;
    os << "1. Edit a cell value" << std::endl;
    os << "2. Edit an entire row's values" << std::endl;
    os << "3. Edit an entire column's values" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    is >> choice;

    try {
      switch (choice) {
        // Edit a cell value
        case 1: {
          std::size_t row = 0, col = 0;
          os << "Enter row index: ";
          is >> row;
          os << "Enter column index: ";
          is >> col;
          os << "Enter new value (number): ";
          double new_val = 0;
          is >> new_val;
          grid.At(row, col) = cse::Datum(new_val);
          os << "Cell updated." << std::endl;
          break;
        }
        // Edits an entire row's value
        case 2: {
          std::size_t row = 0;
          os << "Enter row index to update: ";
          is >> row;
          auto &row_data = grid.GetRow(row);
          os << "Enter " << row_data.size() << " new values (numbers): ";
          for (auto &datum : row_data) {
            double val = 0;
            is >> val;
            datum = cse::Datum(val);
          }
          os << "Row updated." << std::endl;
          break;
        }
          // Edits an entire column's value
        case 3: {
          std::size_t col = 0;
          os << "Enter column index to update: ";
          is >> col;
          std::size_t num_rows = std::get<0>(grid.Shape());
          os << "Enter " << num_rows << " new values (numbers): ";
          for (std::size_t i = 0; i < num_rows; ++i) {
            double val = 0;
            is >> val;
            grid.At(i, col) = cse::Datum(val);
          }
          os << "Column updated." << std::endl;
          break;
        }
        case 0:
          break;
        default:
          os << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * Displays the sort menu and sorts the DataGrid based on the user input.
 * Supported options: Sort column or the entire data grid.
 * @param grid The DataGrid to sort
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::SortSubmenu(cse::DataGrid &grid,
                                   std::ostream &os = std::cout,
                                   std::istream &is = std::cin) {
  int choice = 0;
  do {
    os << "\n--- Sort Options ---" << std::endl;
    os << "1. Sort grid rows by a specified column" << std::endl;
    os << "2. Sort entire grid (using left-most columns as keys)" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    is >> choice;

    try {
      switch (choice) {
        // Sort grid rows by a specified column
        case 1: {
          std::size_t col = 0;
          os << "Enter column index to sort by: ";
          is >> col;
          int order = 0;
          os << "Enter 1 for ascending, 0 for descending: ";
          is >> order;
          grid.SortColumn(col, order == 1);
          os << "Grid rows sorted by column " << col << "." << std::endl;
          break;
        }
        // Sort entire grid (using left-most columns as keys)
        case 2: {
          int order = 0;
          os << "Enter 1 for ascending, 0 for descending: ";
          is >> order;
          grid.Sort(order == 1);
          os << "Entire grid sorted." << std::endl;
          break;
        }
        case 0:
          break;
        default:
          os << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * Displays the add menu and adds to the DataGrid based on the user input.
 * Supported options: Add a row, column, or merge DataGrids
 * @param grid The DataGrid to add to
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::AddSubmenu(cse::DataGrid &grid,
                                  std::ostream &os = std::cout,
                                  std::istream &is = std::cin) {
  int choice = 0;
  do {
    os << "\n--- Adding Options ---" << std::endl;
    os << "1. Add row" << std::endl;
    os << "2. Add column" << std::endl;
    os << "3. Merge another grid" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    is >> choice;

    try {
      switch (choice) {
        // Add row
        case 1: {
          std::size_t num_cols = std::get<1>(grid.Shape());
          os << "Add default row (d), with an equation (e), or enter manually "
                "(m)? ";
          std::string method;
          is >> method;
          // Add a default row
          if (method == "d") {
            os << "Enter default value (number or string): ";
            std::string input;
            is >> input;
            auto num = IsValidDouble(input);
            if (num.has_value()) {
              grid.InsertDefaultRow(cse::kNoIndex, num.value());
            } else {
              grid.InsertDefaultRow(cse::kNoIndex, input);
            }
            os << "Default row added." << std::endl;
            // Add a row manually
          } else if (method == "m") {
            std::vector<cse::Datum> new_row;
            os << "Enter " << num_cols << " values for the new row: ";
            for (std::size_t i = 0; i < num_cols; ++i) {
              std::string val;
              is >> val;
              auto num = IsValidDouble(val);
              new_row.emplace_back(num.has_value() ? cse::Datum(num.value())
                                                   : cse::Datum(val));
            }
            grid.InsertRow(new_row);
            os << "Row added." << std::endl;
            // Add a custom row with an equation
          } else if (method == "e") {
            std::vector<cse::Datum> new_row;
            os << "Enter equation. Supported operators +-/*^, indexes in curly "
                  "braces {}, seperate with a single space: ";
            std::string equation;
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(is, equation);
            os << equation << std::endl;
            cse::ExpressionParser<cse::ReferenceVector<Datum>> parser;
            cse::ReferenceVector<Datum> col;
            size_t index = 0;
            auto func = parser.MakeFunc(equation, 0, index);
            for (size_t i = 0; i < num_cols; ++i) {
              col = grid.GetColumn(i);
              new_row.push_back(func(col));
            }
            grid.InsertRow(new_row);
            os << "Row Added" << std::endl;
          } else {
            os << "Invalid option. Must be 'd', 'e', or 'm'.";
          }
          break;
        }
        // Add column
        case 2: {
          std::size_t num_rows = std::get<0>(grid.Shape());
          os << "Add default column (d), column by equation (e), or enter "
                "manually (m)? ";
          std::string method;
          is >> method;
          // Add a default column
          if (method == "d") {
            os << "Enter default value (number or string): ";
            std::string input;
            is >> input;
            auto num = IsValidDouble(input);
            if (num.has_value()) {
              grid.InsertDefaultColumn(cse::kNoIndex, num.value());
            } else {
              grid.InsertDefaultColumn(cse::kNoIndex, input);
            }
            os << "Default column added." << std::endl;
            // Add a column manually
          } else if (method == "m") {
            std::vector<cse::Datum> new_column;
            os << "Enter " << num_rows << " values for the new column: ";
            for (std::size_t i = 0; i < num_rows; ++i) {
              std::string val;
              is >> val;
              auto num = IsValidDouble(val);
              new_column.emplace_back(num.has_value() ? cse::Datum(num.value())
                                                      : cse::Datum(val));
            }
            grid.InsertColumn(new_column);
            os << "Column added." << std::endl;
            // Add a custom column by an equation
          } else if (method == "e") {
            std::vector<cse::Datum> new_col;
            os << "Enter equation. Supported operators +-/*^, indexes in curly "
                  "braces {}, seperate with a single space: ";
            std::string equation;
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(is, equation);
            cse::ExpressionParser<std::vector<cse::Datum>> parser;
            std::vector<Datum> row;
            size_t index = 0;
            auto func = parser.MakeFunc(equation, 0, index);
            for (size_t i = 0; i < num_rows; ++i) {
              row = grid.GetRow(i);
              new_col.push_back(func(row));
            }
            grid.InsertColumn(new_col);
            os << "Col Added" << std::endl;
          } else {
            os << "Invalid option. Must be 'd', 'e', or 'm'." << std::endl;
          }
          break;
        }
        // Merge DataGrids
        case 3: {
          int merge_type = 0;
          os << "Enter merge type (1 for row append, 0 for column append): ";
          is >> merge_type;
          std::size_t merge_rows = 0, merge_cols = 0;
          os << "Enter number of rows for merging grid: ";
          is >> merge_rows;
          os << "Enter number of columns for merging grid: ";
          is >> merge_cols;

          std::vector<std::vector<cse::Datum>> merge_data(
              merge_rows, std::vector<cse::Datum>(merge_cols));
          os << "Enter values for the merging grid:" << std::endl;
          for (std::size_t i = 0; i < merge_rows; ++i) {
            for (std::size_t j = 0; j < merge_cols; ++j) {
              std::string val;
              is >> val;
              auto num = IsValidDouble(val);
              merge_data[i][j] =
                  num.has_value() ? cse::Datum(num.value()) : cse::Datum(val);
            }
          }

          cse::DataGrid other_grid(merge_data);
          grid = grid.Merge(other_grid, merge_type == 1);
          os << "Grids merged." << std::endl;
          break;
        }

        case 0:
          break;

        default:
          os << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * Displays the delete menu and deletes parts of the DataGrid based on the user
 * input. Supported options: Delete a row, column, or entire DataGrid
 * @param grid The DataGrid to delete from
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::DeleteSubmenu(cse::DataGrid &grid,
                                     std::ostream &os = std::cout,
                                     std::istream &is = std::cin) {
  int choice = 0;
  do {
    os << "\n--- Deleting Options ---" << std::endl;
    os << "1. Delete a row" << std::endl;
    os << "2. Delete a column" << std::endl;
    os << "3. Clear the entire grid" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    is >> choice;

    try {
      switch (choice) {
        // Delete a row
        case 1: {
          std::size_t row = 0;
          os << "Enter row index to delete: ";
          is >> row;
          grid.DeleteRow(row);
          os << "Row deleted." << std::endl;
          break;
        }
        // Delete a column
        case 2: {
          std::size_t col = 0;
          os << "Enter column index to delete: ";
          is >> col;
          grid.DeleteColumn(col);
          os << "Column deleted." << std::endl;
          break;
        }
        // Clear the entire grid
        case 3: {
          grid.Clear();
          os << "Grid cleared." << std::endl;
          break;
        }
        case 0:
          break;
        default:
          os << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * Displays the resize menu and resizes the DataGrid based on the user input.
 * Supported options: Resize via double or string
 * @param grid The DataGrid to resize
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ResizeSubmenu(cse::DataGrid &grid,
                                     std::ostream &os = std::cout,
                                     std::istream &is = std::cin) {
  int choice = 0;
  do {
    os << "\n--- Resizing Options ---" << std::endl;
    os << "1. Resize grid (default type: double)" << std::endl;
    os << "2. Resize grid (default type: string)" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    is >> choice;

    try {
      switch (choice) {
        // Resize the grid (double)
        case 1: {
          std::size_t new_rows = 0, new_cols = 0;
          double default_value = 0;
          os << "Enter new number of rows: ";
          is >> new_rows;
          os << "Enter new number of columns: ";
          is >> new_cols;
          os << "Enter default value (number): ";
          is >> default_value;
          grid.Resize(new_rows, new_cols, default_value);
          os << "Grid resized." << std::endl;
          break;
        }
          // Resize the grid (strings)
        case 2: {
          std::size_t new_rows = 0, new_cols = 0;
          std::string default_value;
          os << "Enter new number of rows: ";
          is >> new_rows;
          os << "Enter new number of columns: ";
          is >> new_cols;
          os << "Enter default value (string): ";
          is >> default_value;
          grid.Resize(new_rows, new_cols, default_value);
          os << "Grid resized." << std::endl;
          break;
        }
        case 0:
          break;
        default:
          os << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * Displays the main menu and displays the possible options.
 * Supported options: Export, Edit, Math, Comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MainMenu(std::ostream &os = std::cout,
                                std::istream &is = std::cin) {
  os << "Welcome to CSV Command Line Manipulator" << std::endl;
  os << "This is a command line application that allows users to create, load, "
        "manipulate, analyze and save CSV file data"
     << std::endl;
  os << "Developed by: Max Krawec, Calen Green, Pedro Mitkiewicz, "
        "Shahaab Ali, and Muhammad Asif Masood"
     << std::endl;
  os << std::endl;

  cse::DataGrid grid = GridMenu();

  while (true) {
    os << "\nMenu Option:" << std::endl;
    os << "x: Export to a CSV file" << std::endl;
    os << "e: Edit CSV Grid" << std::endl;
    os << "m: Math Operations" << std::endl;
    os << "c: Comparisons" << std::endl;
    os << "q: Quit" << std::endl;
    os << "\nPlease enter an option from the list above: ";
    std::string option;
    is >> option;

    if (option == "x") {
      std::string filename;
      os << "Enter CSV filename to export: ";
      is >> filename;
      try {
        if (!cse::CSVFile::ExportCsv(filename, grid)) {
          std::cerr << "Export failed: unknown error\n";
        } else {
          os << "Exported to " << filename << "\n";
        }
      } catch (const std::exception &e) {
        std::cerr << "Export failed: " << e.what() << "\n";
      }
    } else if (option == "e") {
      ManipulateGridMenu(grid);
    } else if (option == "m") {
      MathMenu(grid);
    } else if (option == "c") {
      ComparisonMenu(grid);
    } else if (option == "q") {
      break;
    } else {
      os << "Invalid option. Try again." << std::endl;
    }
  }

  os << std::endl << "Thank you" << std::endl;
}