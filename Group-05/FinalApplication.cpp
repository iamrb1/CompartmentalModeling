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
#include <regex>
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
  if (test_string.size() <= 9) {
    try {
      std::size_t pos;
      int value = std::stoi(test_string, &pos);
      if (pos == test_string.length()) {
        return value;
      }
    } catch (std::invalid_argument & ) {}
  }
  return std::nullopt;
}

/**
 * Prompts the user for a valid column index
 * @param max_index The maximum index for the grid
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @param label optional label for row or column, defaults to column
 * @return A valid column (or row) index for the grid
 */
int FinalApplication::GetColumnIndex(int max_index,
                                     std::ostream &os,
                                     std::istream &is,
                                     const std::string &label) {
  while (true) {
    // Outputs "row index" if necessary
    os << "Please enter " << label << " index: ";
    std::string index_str;
    std::getline(is, index_str);
    auto index = IsValidInt(index_str);
    if (index && index.value() < max_index && index.value() >= 0) {
      return index.value();
    }
    os << "Invalid option. Try again. The max index is: " << (max_index - 1)
       << std::endl;
  }
}

/**
 * Prompts the user for a Datum value
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return A datum value
 */
cse::Datum FinalApplication::GetDataValue(std::ostream &os,
                                          std::istream &is) {
  os << "Please enter the value to compare: ";
  std::string value_str;
  std::getline(is, value_str);
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

// CITE: Used claude.ai to generate this function (comments are my own) - Max Krawec
/**
 * Checks if a string is a valid custom equation. The requirements are:
 *  1) The numbers within the string must be surrounded by {}
 *  2) The numbers within the {} must be less than a certain value.
 *  3) The values outside the {} can only be +, -, *, /, and ^
 *  4) There can't be any other words/values
 * @param input The custom equation string
 * @param max_number_value The maximum value for a column or row
 * @return True if it is a valid custom string, false otherwise.
 */
bool FinalApplication::IsValidCustomEquation(const std::string& input, int max_number_value) {
  // I asked claude.ai to check if a string meets these conditions:
    // 1) The numbers within the string must be surrounded by {}
    // 2) The numbers within the {} must be less than a certain value.
    // 3) The values outside the {} can only be +, -, *, /, and ^
    // 4) There can't be any other words/values
  // It generated the regex pattern below, which checks for these requirements
  std::regex pattern(R"(^(\{(\d+)\}[\s]*([\+\-\*/\^])[\s]*)*\{(\d+)\}$)");

  // If the regex pattern doesn't match, return false
  if (!std::regex_match(input, pattern)) {
    return false;
  }

  // This section of the code performs the 2nd requirement:
  //   The numbers within the {} must be less than a certain value.

  // This retrieves all the numbers from the {}s
  std::regex number_pattern(R"(\{(\d+)\})");
  auto numbers_begin = std::sregex_iterator(input.begin(), input.end(), number_pattern);
  auto numbers_end = std::sregex_iterator();

  // This section of the code iterates through the numbers and checks if it's less than the max number value
  for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
    const std::smatch& match = *i;
    std::string number_str = match[1].str();
    int number = std::stoi(number_str);

    // There is a value that is too large
    if (number >= max_number_value) {
      return false;
    }
  }

  return true;
}

/**
 * Displays the create grid menu that prompts the user to create a custom
 * DataGrid
 * Cite: Made with the help of ChatGPT
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return The custom DataGrid
 */
cse::DataGrid FinalApplication::CreateGridMenu(std::ostream &os,
                                               std::istream &is) {
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
    os << "Enter number of rows for your DataGrid [1-" << MAX_ROWS << "]: ";
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
    os << "Enter number of columns for your DataGrid [1-" << MAX_COLS << "]: ";
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
        os << "Enter a string default value for the DataGrid (1-"
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
cse::DataGrid FinalApplication::GridMenu(std::ostream &os,
                                         std::istream &is) {
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
        cse::DataGrid data_grid = cse::CSVFile::LoadCsv(filename);
        os << "\nBelow is the grid you imported: \n";
        data_grid.Print(os);
        return data_grid;
      } catch (const std::exception &e) {
        os << "Import failed: " << e.what() << "\n" << std::endl;
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

      cse::DataGrid data_grid = cse::DataGrid(premade);
      os << "\nBelow is the pre-made grid: \n";
      data_grid.Print(os);
      return data_grid;
    } else if (choice == "c") {
      // Delegate to the robust CreateGridMenu
      cse::DataGrid data_grid = CreateGridMenu(os, is);
      os << "\nBelow is the grid you made: \n";
      data_grid.Print(os);
      return data_grid;
    } else {
      os << "Invalid option. Try again.\n" << std::endl;
    }
  }
}

// ** MathMenu Functions **

/**
 * Displays the math menu and outputs the results from the mathematical
 * operation based on the user input. Supported operations: Mean, Median, Mode,
 * Standard Deviation, Min, Max, and Summary of the grid
 * @param grid The DataGrid used for the calculations
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenu(const cse::DataGrid &grid,
                                std::ostream &os,
                                std::istream &is) {
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
    std::getline(is, option);

    if (option == "cmean") {
      MathMenuMean(grid, os, is);
      return;
    } else if (option == "cmed") {
      MathMenuMedian(grid, os, is);
      return;
    } else if (option == "csd") {
      MathMenuStandardDeviation(grid, os, is);
      return;
    } else if (option == "cmin") {
      MathMenuMin(grid, os, is);
      return;
    } else if (option == "cmax") {
      MathMenuMax(grid, os, is);
      return;
    } else if (option == "cmode") {
      MathMenuMode(grid, os, is);
      return;
    } else if (option == "sum") {
      MathMenuSummary(grid, os);
      return;
    } else if (option == "b") {
      return;
    } else {
      os << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * Calculates and displays the mean of a specified column
 * @param grid The DataGrid used for the mean calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuMean(const cse::DataGrid &grid,
                                    std::ostream &os,
                                    std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);
  os << "\nMean at column " << index << ": " << grid.ColumnMean(index) << std::endl;
}

/**
 * Calculates and displays the median of a specified column
 * @param grid The DataGrid used for the median calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuMedian(const cse::DataGrid &grid,
                                    std::ostream &os,
                                    std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);
  os << "\nMedian at column " << index << ": " << grid.ColumnMedian(index) << std::endl;
}

/**
 * Calculates and displays the standard deviation of a specified column
 * @param grid The DataGrid used for the standard deviation calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuStandardDeviation(const cse::DataGrid &grid,
                                    std::ostream &os,
                                    std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);
  os << "\nStandard deviation at column " << index << ": "
     << grid.ColumnStandardDeviation(index) << std::endl;
}

/**
 * Calculates and displays the min of a specified column
 * @param grid The DataGrid used for the min calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuMin(const cse::DataGrid &grid,
                                   std::ostream &os,
                                   std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);
  os << "\nMin at column " << index << ": " << grid.ColumnMin(index) << std::endl;
}

/**
 * Calculates and displays the max of a specified column
 * @param grid The DataGrid used for the max calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuMax(const cse::DataGrid &grid,
                                   std::ostream &os,
                                   std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);
  os << "\nMax at column " << index << ": " << grid.ColumnMax(index) << std::endl;
}

/**
 * Calculates and displays the mode of a specified column
 * @param grid The DataGrid used for the mode calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuMode(const cse::DataGrid &grid,
                                   std::ostream &os,
                                   std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);

  os << "\nMode(s) at column " << index << ": ";
  for (const double &mode_val : grid.ColumnMode(index)) {
    os << mode_val << " ";
  }
  os << std::endl;
}

/**
 * Calculates and displays the mean, median, standard deviation, mode, min, and max of the entire grid
 * @param grid The DataGrid used for the calculations
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuSummary(const cse::DataGrid &grid,
                                    std::ostream &os) {
  os << "\nBelow is the grid: \n";
  grid.Print(os);

  auto summary = grid.CalculateDataGridMathSummary();
  os << "\nGrid Summary:" << std::endl;
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
}


// ** ComparisonMenu Functions **

/**
 * Displays the comparison menu and outputs the results from the comparison
 * based on the user input. Supported comparisons: Less than, Less than or
 * equal, Greater than, Greater than or equal, equal, not equal
 * @param grid The DataGrid used for comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenu(cse::DataGrid &grid,
                                      std::ostream &os,
                                      std::istream &is) {
  while (true) {
    os << "\nComparison Menu:" << std::endl;
    os << "clt: Find values less than a given value for a column" << std::endl;
    os << "clte: Find values less than or equal to a given value for a column" << std::endl;
    os << "cgt: Find values greater than a given value for a column" << std::endl;
    os << "cgte: Find values greater than or equal to a given value for a column" << std::endl;
    os << "ce: Find values equal to a value for a column" << std::endl;
    os << "cne: Find values not equal to a given value for a column" << std::endl;
    os << "b: Go back" << std::endl;
    os << "Enter option: ";
    std::string option;
    std::getline(is, option);

    if (option == "clt") {
      ComparisonMenuLessThan(grid, os, is);
      return;
    } else if (option == "clte") {
      ComparisonMenuLessThanEqual(grid, os, is);
      return;
    } else if (option == "cgt") {
      ComparisonMenuGreaterThan(grid, os, is);
      return;
    } else if (option == "cgte") {
      ComparisonMenuGreaterThanEqual(grid, os, is);
      return;
    } else if (option == "ce") {
      ComparisonMenuEqual(grid, os, is);
      return;
    } else if (option == "cne") {
      ComparisonMenuNotEqual(grid, os, is);
      return;
    } else if (option == "b") {
      return;
    } else {
      os << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * Displays the  results from the less than comparison of a specified column
 * @param grid The DataGrid used for less than comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenuLessThan(cse::DataGrid &grid,
                                              std::ostream &os,
                                              std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  cse::Datum datum = GetDataValue(os, is);

  os << "\nThe column values: \n";
  PrintColumn((grid.GetColumn(index)));

  os << "\nValues less than the given value:" << std::endl;
  PrintColumn(grid.ColumnLessThan(index, datum));
}

/**
 * Displays the  results from the less than or equal comparison of a specified column
 * @param grid The DataGrid used for less than or equal comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenuLessThanEqual(cse::DataGrid &grid,
                                              std::ostream &os,
                                              std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  cse::Datum datum = GetDataValue(os, is);

  os << "\nThe column values: \n";
  PrintColumn((grid.GetColumn(index)));

  os << "\nValues less than or equal to the given value:" << std::endl;
  PrintColumn(grid.ColumnLessThanOrEqual(index, datum));
}

/**
 * Displays the  results from the greater than comparison of a specified column
 * @param grid The DataGrid used for greater than comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenuGreaterThan(cse::DataGrid &grid,
                                                   std::ostream &os,
                                                   std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  cse::Datum datum = GetDataValue(os, is);

  os << "\nThe column values: \n";
  PrintColumn((grid.GetColumn(index)));

  os << "\nValues greater than the given value:" << std::endl;
  PrintColumn(grid.ColumnGreaterThan(index, datum));
}

/**
 * Displays the  results from the greater than or equal comparison of a specified column
 * @param grid The DataGrid used for greater than or equal comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenuGreaterThanEqual(cse::DataGrid &grid,
                                                 std::ostream &os,
                                                 std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  cse::Datum datum = GetDataValue(os, is);

  os << "\nThe column values: \n";
  PrintColumn((grid.GetColumn(index)));

  os << "\nValues greater than or equal to the given value:" << std::endl;
  PrintColumn(grid.ColumnGreaterThanOrEqual(index, datum));
}

/**
 * Displays the  results from the equal comparison of a specified column
 * @param grid The DataGrid used for equal comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenuEqual(cse::DataGrid &grid,
                                                 std::ostream &os,
                                                 std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  cse::Datum datum = GetDataValue(os, is);

  os << "\nThe column values: \n";
  PrintColumn((grid.GetColumn(index)));

  os << "\nValues equal to the given value:" << std::endl;
  PrintColumn(grid.ColumnEqual(index, datum));
}

/**
 * Displays the  results from the not equal comparison of a specified column
 * @param grid The DataGrid used for not equal comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenuNotEqual(cse::DataGrid &grid,
                                                 std::ostream &os,
                                                 std::istream &is) {
  int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  cse::Datum datum = GetDataValue(os, is);

  os << "\nThe column values: \n";
  PrintColumn((grid.GetColumn(index)));

  os << "\nValues not equal to the given value:" << std::endl;
  PrintColumn(grid.ColumnNotEqual(index, datum));
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
                                          std::ostream &os,
                                          std::istream &is) {
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
    std::getline(is, input);
    std::optional<int> input_int = IsValidInt(input);

    if (input_int.has_value()) {
      choice = input_int.value();
      switch (choice) {
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
                                    std::ostream &os,
                                    std::istream &is) {
  while (true) {
    os << "\n--- Print Options ---" << std::endl;
    os << "1. Print a cell value" << std::endl;
    os << "2. Print a row" << std::endl;
    os << "3. Print a column" << std::endl;
    os << "4. Print entire grid" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    
    std::string raw;
    if (!(std::getline(is, raw))) return;               // no more inputs for this
    auto mi = IsValidInt(raw);
    if (!mi) {
      os << "Invalid choice. Input must be an int. Try again." << std::endl;
      continue;
    }
    int choice = mi.value();

      switch (choice) {
        // Print a cell value
        case 1: {
          PrintSubmenuCell(grid, os, is);
          break;
        }
        // Print a row
        case 2: {
          PrintSubmenuRow(grid, os, is);
          break;
        }
        // Print a column
        case 3: {
          PrintSubmenuColumn(grid, os, is);
          break;
        }
        // Print the entire DataGrid
        case 4:
          grid.Print(os);
          break;
      case 0:
        return;
      default:
        os << "Invalid choice. Input must be between 0-4. Try again." << std::endl;
    }
  }
}

/**
 * Prints the specified cell value
 * @param grid The DataGrid to print from
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::PrintSubmenuCell(const cse::DataGrid &grid,
                                        std::ostream &os,
                                        std::istream &is) {
  std::optional<int> row = PrintSubmenuGetRow(
      static_cast<int>(std::get<0>(grid.Shape())), os, is);
  if (!row) return;

  std::optional<int> column = PrintSubmenuGetColumn(
      static_cast<int>(std::get<1>(grid.Shape())), os, is);
  if (!column) return;

  const cse::Datum &value = grid.GetValue(row.value(), column.value());
  if (value.IsDouble()) {
    os << "\nCell (" << row.value() << ", " << column.value() << "): "
       << value.GetDouble() << std::endl;
  } else {
    os << "\nCell (" << row.value() << ", " << column.value() << "): "
       << value.GetString() << std::endl;
  }
}

/**
 * Prints the specified row
 * @param grid The DataGrid to print from
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::PrintSubmenuRow(const cse::DataGrid &grid,
                                       std::ostream &os,
                                       std::istream &is) {
  std::optional<int> row = PrintSubmenuGetRow(
      static_cast<int>(std::get<0>(grid.Shape())), os, is);
  if (!row) return;

  auto row_data = grid.GetRow(row.value());
  os << "\nRow " << row.value() << ": ";
  for (const auto &d : row_data) {
    if (d.IsDouble()) os << d.GetDouble() << " ";
    else os << d.GetString() << " ";
  }
  os << std::endl;
}

/**
 * Prints the specified column
 * @param grid The DataGrid to print from
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::PrintSubmenuColumn(const cse::DataGrid &grid,
                                          std::ostream &os,
                                          std::istream &is) {
  std::optional<int> column = PrintSubmenuGetColumn(
      static_cast<int>(std::get<1>(grid.Shape())), os, is);
  if (!column) return;

  auto col_data = grid.GetColumn(column.value());
  os << "\nColumn " << column.value() << ": ";
  for (const auto &d : col_data) {
    if (d.IsDouble()) os << d.GetDouble() << " ";
    else os << d.GetString() << " ";
  }
  os << std::endl;
}

/**
 * Gets the row value from the user.
 * @param max_row The maximum row value
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return Optional int if it's a valid row value, empty optional if not
 */
std::optional<int> FinalApplication::PrintSubmenuGetRow(const int max_row,
                                                        std::ostream &os,
                                                        std::istream &is) {
  os << "Enter row index: ";
  std::string row_string;
  if (!(std::getline(is, row_string))) return {};

  std::optional<int> is_valid_int = IsValidInt(row_string);
  if (!is_valid_int || is_valid_int.value() < 0 || is_valid_int.value() >= max_row) {
    os << "Invalid row. " << "The max row is: " << max_row - 1 << ". Try again." << std::endl;
    return {};
  }

  return is_valid_int.value();
}
/**
 * Gets the column value from the user.
 * @param max_col The maximum row value
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return Optional int if it's a valid column value, empty optional if not
 */
std::optional<int> FinalApplication::PrintSubmenuGetColumn(const int max_col,
                                                           std::ostream &os,
                                                           std::istream &is) {
  os << "Enter column index: ";
  std::string column_string;
  if (!(std::getline(is, column_string))) return {};

  std::optional<int> is_valid_int = IsValidInt(column_string);
  if (!is_valid_int || is_valid_int.value() < 0 || is_valid_int.value() >= max_col) {
    os << "Invalid column. " << "The max column is: " << max_col - 1 << ". Try again." << std::endl;
    return {};
  }

  return is_valid_int.value();
}

/**
 * Displays the edit grid menu and edits the DataGrid based on the user input.
 * Supported options: Edit a cell, row, or column
 * @param grid The DataGrid to edit
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::EditSubmenu(cse::DataGrid &grid,
                                   std::ostream &os,
                                   std::istream &is) {
  const auto [maxRows, maxCols] = grid.Shape();
  while (true) {
    os << "\n--- Edit Options ---" << std::endl;
    os << "1. Edit a cell value" << std::endl;
    os << "2. Edit an entire row's values" << std::endl;
    os << "3. Edit an entire column's values" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";

    // read and validate menu choice
    std::string raw_choice;
    if (!(std::getline(is, raw_choice))) return;
    auto mi = IsValidInt(raw_choice);
    if (!mi || *mi < 0) {
      os << "Invalid choice. Input must be an int. Try again." << std::endl;
      continue;
    }
    int choice = *mi;

      switch (choice) {
        // Edit a cell value
        case 1: {
          os << "Enter row index: ";
          std::string rraw;
          if (!(std::getline(is,rraw))) return;
          auto ri = IsValidInt(rraw);
          if (!ri || *ri < 0 || static_cast<std::size_t>(*ri) >= maxRows) {
            os << "Invalid row. " << "The max row is: " << maxRows - 1 << ". Try again." << std::endl;
            continue;
          }
          std::size_t row = *ri;

          // column index
          os << "Enter column index: ";
          std::string craw;
          if (!(std::getline(is,craw))) return;
          auto ci = IsValidInt(craw);
          if (!ci || *ci < 0 || static_cast<std::size_t>(*ci) >= maxCols) {
            os << "Invalid column. " << "The max column is: " << maxCols - 1 << ". Try again." << std::endl;
            continue;
          }
          std::size_t col = *ci;

          // new value (string or double)
          os << "Enter new value: ";
          std::string valstr;
          if (!(std::getline(is, valstr))) return;
          if (auto d = IsValidDouble(valstr)) {
            grid.At(row, col) = cse::Datum(d.value());
          } else {
            grid.At(row, col) = cse::Datum(valstr);
          }
          os << "\nCell updated." << std::endl;

          os << "\nThe Grid is now:" << std::endl;
          grid.Print(os);
          break;
        }
        // Edits an entire row's value
        case 2: {
          os << "Enter row index to update: ";
          std::string rraw;
          if (!(std::getline(is, rraw))) return;
          auto ri = IsValidInt(rraw);
          if (!ri || *ri < 0 || static_cast<std::size_t>(*ri) >= maxRows) {
            os << "Invalid row. " << "The max row is: " << maxRows - 1 << ". Try again." << std::endl;
            continue;
          }
          std::size_t row = *ri;

          auto &rowData = grid.GetRow(row);
          os << "Enter " << rowData.size() << " new values one at a time: \n";
          for (std::size_t i = 0; i < rowData.size(); ++i) {
            os << "Value " << i + 1 << ":" << std::endl;
            std::string v;
            if (!(std::getline(is, v))) return;
            if (auto d = IsValidDouble(v)) {
              rowData[i] = cse::Datum(d.value());
            } else {
              rowData[i] = cse::Datum(v);
            }
          }
          os << "\nRow updated." << std::endl;

          os << "\nThe Grid is now:" << std::endl;
          grid.Print(os);
          break;
        }
          // Edits an entire column's value
        case 3: {
          os << "Enter column index to update: ";
          std::string craw;
          if (!(std::getline(is, craw))) return;
          auto ci = IsValidInt(craw);
          if (!ci || *ci < 0 || static_cast<std::size_t>(*ci) >= maxCols) {
            os << "Invalid column. " << "The max column is: " << maxCols - 1 << ". Try again." << std::endl;
            continue;
          }
          std::size_t col = *ci;

          std::size_t nrows = maxRows;
          os << "Enter " << nrows << " new values one at a time: \n";
          for (std::size_t i = 0; i < nrows; ++i) {
            os << "Value " << i + 1 << ":" << std::endl;
            std::string v;
            if (!(std::getline(is, v))) return;
            if (auto d = IsValidDouble(v)) {
              grid.At(i, col) = cse::Datum(d.value());
            } else {
              grid.At(i, col) = cse::Datum(v);
            }
          }
          os << "\nColumn updated." << std::endl;

          os << "\nThe Grid is now:" << std::endl;
          grid.Print(os);
          break;
        }
        case 0:
          return;
        default:
          os << "Invalid choice. Input must be between 0-3. Try again." << std::endl;
      }
  }  
}

/**
 * Displays the sort menu and sorts the DataGrid based on the user input.
 * Supported options: Sort column or the entire data grid.
 * @param grid The DataGrid to sort
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::SortSubmenu(cse::DataGrid &grid,
                                   std::ostream &os,
                                   std::istream &is) {
  int choice = -1;
  do {
    os << "\n--- Sort Options ---" << std::endl;
    os << "1. Sort grid rows by a specified column" << std::endl;
    os << "2. Sort entire grid (using left-most columns as keys)" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    std::string input;
    std::getline(is, input);

    std::optional<int> input_int = IsValidInt(input);

    if (input_int.has_value()) {
      choice = input_int.value();
      switch (choice) {
        // Sort grid rows by a specified column
        case 1: {
          std::size_t col = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
          std::string order;
          while (true) {
            os << "Enter 1 for ascending, 0 for descending: ";
            std::getline(is, order);
            std::optional<int> ordering_input = IsValidInt(order);
            if (ordering_input && (ordering_input.value() == 0 || ordering_input.value() == 1)) {
              grid.SortColumn(col, ordering_input.value());
              os << "Grid rows sorted by column " << col << ".\n" << std::endl;
              os << "The Grid is now:" << std::endl;
              grid.Print(os);
              break;
            } else {
              os << "Invalid input. The input must be 0 or 1" << std::endl;
            }
          }
          break;
        }
        // Sort entire grid (using left-most columns as keys)
        case 2: {
          std::string order;
          while (true) {
            os << "Enter 1 for ascending, 0 for descending: ";
            std::getline(is, order);
            std::optional<int> ordering_input = IsValidInt(order);
            if (ordering_input && (ordering_input.value() == 0 || ordering_input.value() == 1)) {
              grid.Sort(ordering_input.value());
              os << "Entire grid sorted.\n" << std::endl;
              os << "The Grid is now:" << std::endl;
              grid.Print(os);
              break;
            } else {
              os << "Invalid input. The input must be 0 or 1" << std::endl;
            }
          }
          break;
        }
        case 0:
          break;
        default:
          os << "Invalid choice. Input must be between 0-2.\n" << std::endl;
      }
    }
    else {
      os << "Invalid choice. Cannot be a string. Try again.\n";
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
                                  std::ostream &os,
                                  std::istream &is) {
  int choice = -1;
  do {
    os << "\n--- Adding Options ---" << std::endl;
    os << "1. Add row" << std::endl;
    os << "2. Add column" << std::endl;
    os << "3. Merge another grid" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    std::string main_input;
    std::getline(is, main_input);

    std::optional<int> input_int = IsValidInt(main_input);

    if (input_int.has_value()) {
      choice = input_int.value();
      try {
        switch (choice) {
          // Add row
          case 1: {
            std::size_t num_cols = std::get<1>(grid.Shape());

            std::string method;
            while(true) {
              os << "Add default row (d), with an equation (e), or enter manually (m)? ";
              std::getline(is, method);
              if (method == "d" || method == "e" || method == "m") {
                break;
              } else {
                os << "Invalid option. Must be 'd', 'e', or 'm'.\n" << std::endl;
              }
            }

            if (method == "d") {
              os << "\nEnter default value (number or string): ";
              std::string input;
              std::getline(is, input);
              auto num = IsValidDouble(input);
              if (num.has_value()) {
                grid.InsertDefaultRow(cse::kNoIndex, num.value());
              } else {
                grid.InsertDefaultRow(cse::kNoIndex, input);
              }
              os << "Default row added." << std::endl;

              os << "\nBelow is the new grid: \n";
              grid.Print(os);

              // Add a row manually
            } else if (method == "m") {
              std::vector<cse::Datum> new_row;
              os << "\nEnter " << num_cols << " values for the new row one at a time: \n";
              for (std::size_t i = 0; i < num_cols; ++i) {
                os << "Value " << i + 1 << ":" << std::endl;
                std::string val;
                std::getline(is, val);
                auto num = IsValidDouble(val);
                new_row.emplace_back(num.has_value() ? cse::Datum(num.value())
                                         : cse::Datum(val));
              }
              grid.InsertRow(new_row);
              os << "Row added." << std::endl;

              os << "\nBelow is the new grid: \n";
              grid.Print(os);
              // Add a custom row with an equation
            } else if (method == "e") {
              std::vector<cse::Datum> new_row;

              std::string equation;
              while (true) {
                os << "\nEnter equation:" << std::endl;
                os << "Supported operators: +, -, /, *, and ^" << std::endl;
                os << "Insert row indexes in the curly braces {}. For example, {0}" << std::endl;
                os << "Separate each action with a single space" << std::endl;
                os << "Examples: \n1) {0} + {1}  \n2) {0} * {0} / {1} \n3) {0} ^ {0} - {1} \n";
                std::getline(is, equation);
                bool is_valid_equation = IsValidCustomEquation(equation, static_cast<int>(std::get<0>(grid.Shape())));
                if (is_valid_equation) {
                  break;
                } else {
                  os << "Invalid equation. Try again. \n";
                }
              }

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

              os << "\nBelow is the new grid: \n";
              grid.Print(os);
            } else {
              os << "Invalid option. Must be 'd', 'e', or 'm'.";
            }
            break;
          }
            // Add column
          case 2: {
            std::size_t num_rows = std::get<0>(grid.Shape());

            std::string method;
            while(true) {
              os << "Add default row (d), with an equation (e), or enter manually (m)? ";
              std::getline(is, method);
              if (method == "d" || method == "e" || method == "m") {
                break;
              } else {
                os << "Invalid option. Must be 'd', 'e', or 'm'.\n" << std::endl;
              }
            }

            // Add a default column
            if (method == "d") {
              os << "\nEnter default value (number or string): ";
              std::string input;
              std::getline(is, input);
              auto num = IsValidDouble(input);
              if (num.has_value()) {
                grid.InsertDefaultColumn(cse::kNoIndex, num.value());
              } else {
                grid.InsertDefaultColumn(cse::kNoIndex, input);
              }
              os << "Default column added." << std::endl;

              os << "\nBelow is the new grid: \n";
              grid.Print(os);
              // Add a column manually
            } else if (method == "m") {
              std::vector<cse::Datum> new_column;
              os << "\nEnter " << num_rows << " values for the new column one at a time: \n";
              for (std::size_t i = 0; i < num_rows; ++i) {
                os << "Value " << i + 1 << ":" << std::endl;
                std::string val;
                std::getline(is, val);
                auto num = IsValidDouble(val);
                new_column.emplace_back(num.has_value() ? cse::Datum(num.value())
                                            : cse::Datum(val));
              }
              grid.InsertColumn(new_column);

              os << "Column added." << std::endl;

              os << "\nBelow is the new grid: \n";
              grid.Print(os);

              // Add a custom column by an equation
            } else if (method == "e") {
              std::vector<cse::Datum> new_col;

              std::string equation;
              while (true) {
                os << "\nEnter equation:" << std::endl;
                os << "Supported operators: +, -, /, *, and ^" << std::endl;
                os << "Insert column indexes in the curly braces {}. For example, {0}" << std::endl;
                os << "Separate each action with a single space" << std::endl;
                os << "Examples: \n1) {0} + {1}  \n2) {0} * {0} / {1} \n3) {0} ^ {0} - {1} \n";
                std::getline(is, equation);
                bool is_valid_equation = IsValidCustomEquation(equation, static_cast<int>(std::get<1>(grid.Shape())));
                if (is_valid_equation) {
                  break;
                } else {
                  os << "Invalid equation. Try again. \n";
                }
              }

              cse::ExpressionParser<std::vector<cse::Datum>> parser;
              std::vector<Datum> row;
              size_t index = 0;
              auto func = parser.MakeFunc(equation, 0, index);
              for (size_t i = 0; i < num_rows; ++i) {
                row = grid.GetRow(i);
                new_col.push_back(func(row));
              }
              grid.InsertColumn(new_col);
              os << "Column Added." << std::endl;

              os << "\nBelow is the new grid: \n";
              grid.Print(os);
            } else {
              os << "Invalid option. Must be 'd', 'e', or 'm'." << std::endl;
            }
            break;
          }
            // Merge DataGrids
          case 3: {
            std::string merge_type;
            while (true) {
              os << "Enter merge type (1 for row append, 0 for column append): ";
              std::getline(is, merge_type);
              if (merge_type == "1" || merge_type == "0") {
                break;
              } else {
                os << "Invalid input. Must be 0 or 1 \n" << std::endl;
              }
            }
            int merge_type_int = std::stoi(merge_type);

            std::string merge_rows;
            while (true) {
              os << "\nEnter number of rows for merging grid: ";
              std::getline(is, merge_rows);
              std::optional<int> rows_int = IsValidInt(merge_rows);
              if (rows_int.has_value() && rows_int.value() > 0) {
                if (merge_type == "1" || rows_int.value() == static_cast<int>(std::get<0>(grid.Shape()))) {
                  break;
                } else {
                  os << "Invalid row size. The row must equal your current grid's row size:" << std::get<0>(grid.Shape()) << "\n" << std::endl;
                }
              } else {
                os << "Invalid input. Must be an valid int \n" << std::endl;
              }
            }
            int merge_rows_int = std::stoi(merge_rows);

            std::string merge_cols;
            while (true) {
              os << "\nEnter number of columns for merging grid: ";
              std::getline(is, merge_cols);
              std::optional<int> col_int = IsValidInt(merge_cols);
              if (col_int.has_value() && col_int.value() > 0) {
                if (merge_type == "0" || col_int.value() == static_cast<int>(std::get<1>(grid.Shape()))) {
                  break;
                } else {
                  os << "Invalid column size. The row must equal the current grid's column size:" << std::get<1>(grid.Shape()) << "\n" << std::endl;
                }
              } else {
                os << "Invalid input. Must be an valid int \n" << std::endl;
              }
            }
            int merge_col_int = std::stoi(merge_cols);

            std::vector<std::vector<cse::Datum>> merge_data(merge_rows_int, std::vector<cse::Datum>(merge_col_int));
            os << "Enter " << merge_rows_int * merge_col_int << " values for the merging grid:" << std::endl;
            int counter = 1;
            for (int i = 0; i < merge_rows_int; ++i) {
              for (int j = 0; j < merge_col_int; ++j) {
                os << "Value (" << counter << "/" << merge_rows_int * merge_col_int << "):" << std::endl;
                std::string val;
                std::getline(is, val);
                auto num = IsValidDouble(val);
                merge_data[i][j] = num.has_value() ? cse::Datum(num.value()) : cse::Datum(val);
                counter += 1;
              }
            }

            cse::DataGrid other_grid(merge_data);
            grid = grid.Merge(other_grid, merge_type_int == 1);
            os << "Grids merged." << std::endl;

            os << "\nBelow is the new grid: \n";
            grid.Print(os);
            break;
          }

          case 0:
            break;

          default:
            os << "Invalid choice. Input must be between 0-3. Try again." << std::endl;
        }
      } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
    else {
      os << "Invalid choice. Cannot be a string. Try again.\n";
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
                                     std::ostream &os,
                                     std::istream &is) {
  int choice = -1;
  do {
    os << "\n--- Deleting Options ---" << std::endl;
    os << "1. Delete a row" << std::endl;
    os << "2. Delete a column" << std::endl;
    os << "3. Clear the entire grid" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    std::string input;
    std::getline(is, input);

    std::optional<int> input_int = IsValidInt(input);

    if (input_int.has_value()) {
      choice = input_int.value();
      try {
        switch (choice) {
          // Delete a row
          case 1: {
            grid.DeleteRow(GetColumnIndex(static_cast<int>(std::get<0>(grid.Shape())), os, is, "row"));
            os << "Row deleted." << std::endl;

            os << "\nBelow is the new grid: \n";
            grid.Print(os);
            break;
          }
            // Delete a column
          case 2: {
            grid.DeleteColumn(GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is));
            os << "Column deleted." << std::endl;

            os << "\nBelow is the new grid: \n";
            grid.Print(os);
            break;
          }
            // Clear the entire grid
          case 3: {
            grid.Clear();
            os << "Grid cleared." << std::endl;

            os << "\nBelow is the new grid: \n";
            grid.Print(os);
            break;
          }
          case 0:
            break;
          default:
            os << "Invalid choice. The value must be between 0-3." << std::endl;
        }
      } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
    else {
      os << "Invalid choice. Cannot be a string. Try again.\n";
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
                                     std::ostream &os,
                                     std::istream &is) {
  int choice = -1;
  do {
    os << "\n--- Resizing Options ---" << std::endl;
    os << "1. Resize grid" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    std::string input;
    std::getline(is, input);

    std::optional<int> input_int = IsValidInt(input);

    if (input_int.has_value()) {
      choice = input_int.value();
      switch (choice) {
        // Resize the grid (double)
        case 1: {
          std::string new_rows;
          std::string new_cols;
          std::string default_value;

          while (true) {
            os << "Enter new number of rows (1-1000): ";
            std::getline(is, new_rows);

            std::optional new_rows_int = IsValidInt(new_rows);
            if (new_rows_int.has_value() && new_rows_int.value() >= 1 && new_rows_int.value() <= 1000) {
              break;
            } else {
              os << "Invalid input. Please enter a number between 1 and 1000.\n";
            }
          }
          while (true) {
            os << "Enter new number of columns (1-1000): ";
            std::getline(is, new_cols);

            std::optional new_cols_int = IsValidInt(new_cols);
            if (new_cols_int.has_value() && new_cols_int.value() >= 1 && new_cols_int.value() <= 1000) {
              break;
            } else {
              os << "Invalid input. Please enter a number between 1 and 1000.\n";
            }
          }
          os << "Enter default value: ";
          std::getline(is, default_value);
          if (IsValidDouble(default_value)) {
            grid.Resize(std::stoi(new_rows), std::stoi(new_cols), std::stod(default_value));
          } else {
            grid.Resize(std::stoi(new_rows), std::stoi(new_cols), default_value);
          }
          os << "Grid resized." << std::endl;

          os << "\nBelow is the new grid: \n";
          grid.Print(os);
          break;
        }
        case 0:
          break;
        default:
          os << "Invalid resize option. Must be between 0-2. Try again." << std::endl;
      }
    } else {
      os << "Invalid choice. Cannot be a string. Try again.\n";
    }
  } while (choice != 0);
}

/**
 * Displays the main menu and displays the possible options.
 * Supported options: Export, Edit, Math, Comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MainMenu(std::ostream &os,
                                std::istream &is) {
  os << "\nWelcome to CSV Command Line Manipulator!" << std::endl;
  os << "This is a command line application that allows users to create, load, "
        "manipulate, analyze and save CSV file data\n"
     << std::endl;
  os << "Developed by: Max Krawec, Calen Green, Pedro Mitkiewicz, "
        "Shahaab Ali, and Muhammad Asif Masood"
     << std::endl;
  os << std::endl;

  cse::DataGrid grid = GridMenu(os, is);

  while (true) {
    os << "\nMenu Option:" << std::endl;
    os << "x: Export to a CSV file" << std::endl;
    os << "e: Edit CSV Grid" << std::endl;
    os << "m: Math Operations" << std::endl;
    os << "c: Comparisons" << std::endl;
    os << "q: Quit" << std::endl;
    os << "\nPlease enter an option from the list above: ";
    std::string option;
    std::getline(is, option);

    if (option == "x") {
      std::string filename;
      while (true) {
        os << "Enter CSV filename to export: ";
        std::getline(is, filename);
        if (filename.ends_with(".csv")) {
          break;
        } else {
          os << "Invalid filename. The file must end with .csv" << "\n";
        }
      }
      try {
        if (!cse::CSVFile::ExportCsv(filename, grid)) {
          std::cerr << "Export failed: unknown error\n";
        } else {
          os << "\nExported to " << filename << "\n";
        }
      } catch (const std::exception &e) {
        std::cerr << "Export failed: " << e.what() << "\n";
      }
    } else if (option == "e") {
      ManipulateGridMenu(grid, os, is);
    } else if (option == "m") {
      MathMenu(grid, os, is);
    } else if (option == "c") {
      ComparisonMenu(grid, os, is);
    } else if (option == "q") {
      break;
    } else {
      os << "\nInvalid option. Try again." << std::endl;
    }
  }

  os << std::endl << "Thank you" << std::endl;
}