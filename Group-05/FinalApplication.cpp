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
 * @param input The string to check
 * @return an optional double if the string can be converted, empty optional if
 */
std::optional<double> FinalApplication::ValidDouble(
    const std::string &input) noexcept {
  try {
    // CITE: Used ChatGPT for the string length check
    std::size_t pos;
    double value = std::stod(input, &pos);
    if (pos == input.length()) {
      return value;
    }
  } catch (std::invalid_argument &) {
  }
  return std::nullopt;
}

/**
 * Checks if a string can be converted into an int
 * @param input The string to check
 * @return an optional int if the string can be converted, empty optional if not
 */
std::optional<int> FinalApplication::ValidInt(
    const std::string &input) noexcept {
  static constexpr int MAX_VALUE = 8;
  if (input.size() <= MAX_VALUE) {
    try {
      std::size_t pos;
      int value = std::stoi(input, &pos);
      if (pos == input.length()) {
        return value;
      }
    } catch (std::invalid_argument &) {
    }
  }
  return std::nullopt;
}

/**
 * Prompts the user for a valid column index
 * @param grid_size The maximum index for the grid
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @param label optional label for row or column, defaults to column
 * @return A valid column (or row) index for the grid
 */
int FinalApplication::GetIndex(int grid_size, std::ostream &os,
                               std::istream &is,
                               const std::string &label) noexcept {
  // Continue to prompt user until valid input
  while (true) {
    os << "Please enter " << label << " index: ";
    std::string index_str;
    std::getline(is, index_str);
    if (auto index = ValidInt(index_str);
        index && index.value() < grid_size && index.value() >= 0) {
      return index.value();
    }
    os << "Invalid option. Try again. The max index is: " << (grid_size - 1)
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
                                          std::istream &is) noexcept {
  os << "Please enter the value to compare: ";
  std::string value_str;
  std::getline(is, value_str);
  if (auto d = ValidDouble(value_str)) {
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
    std::ostream &os = std::cout) noexcept {
  for (const auto &datum : column) {
    if (datum.IsDouble()) {
      os << datum.GetDouble() << " ";
    } else {
      os << datum.GetString() << " ";
    }
  }
  os << std::endl;
}

// CITE: Used claude.ai to generate this function (comments are my own) - Max
// Krawec
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
bool FinalApplication::IsValidCustomEquation(const std::string &input,
                                             int max_number_value) noexcept {
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
  auto numbers_begin =
      std::sregex_iterator(input.begin(), input.end(), number_pattern);
  auto numbers_end = std::sregex_iterator();

  // This section of the code iterates through the numbers and checks if it's
  // less than the max number value
  for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
    const std::smatch &match = *i;
    std::string number_str = match[1].str();
    int number = std::stoi(number_str);

    // There is a value that is too large
    if (number >= max_number_value) {
      return false;
    }
  }

  return true;
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
void FinalApplication::MathMenu(const cse::DataGrid &grid, std::ostream &os,
                                std::istream &is) noexcept {
  // Continue to prompt user until 'b' is input
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
    }
    if (option == "cmed") {
      MathMenuMedian(grid, os, is);
      return;
    }
    if (option == "csd") {
      MathMenuStandardDeviation(grid, os, is);
      return;
    }
    if (option == "cmin") {
      MathMenuMin(grid, os, is);
      return;
    }
    if (option == "cmax") {
      MathMenuMax(grid, os, is);
      return;
    }
    if (option == "cmode") {
      MathMenuMode(grid, os, is);
      return;
    }
    if (option == "sum") {
      MathMenuSummary(grid, os);
      return;
    }
    if (option == "b") {
      return;
    }
    os << "Invalid option. Try again." << std::endl;
  }
}

/**
 * Calculates and displays the mean of a specified column
 * @param grid The DataGrid used for the mean calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuMean(const cse::DataGrid &grid, std::ostream &os,
                                    std::istream &is) noexcept {
  int index = GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);
  os << "\nMean at column " << index << ": " << grid.ColumnMean(index)
     << std::endl;
}

/**
 * Calculates and displays the median of a specified column
 * @param grid The DataGrid used for the median calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuMedian(const cse::DataGrid &grid,
                                      std::ostream &os,
                                      std::istream &is) noexcept {
  int index = GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);
  os << "\nMedian at column " << index << ": " << grid.ColumnMedian(index)
     << std::endl;
}

/**
 * Calculates and displays the standard deviation of a specified column
 * @param grid The DataGrid used for the standard deviation calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuStandardDeviation(const cse::DataGrid &grid,
                                                 std::ostream &os,
                                                 std::istream &is) noexcept {
  int index = GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

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
void FinalApplication::MathMenuMin(const cse::DataGrid &grid, std::ostream &os,
                                   std::istream &is) noexcept {
  int index = GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);
  os << "\nMin at column " << index << ": " << grid.ColumnMin(index)
     << std::endl;
}

/**
 * Calculates and displays the max of a specified column
 * @param grid The DataGrid used for the max calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuMax(const cse::DataGrid &grid, std::ostream &os,
                                   std::istream &is) noexcept {
  int index = GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);
  os << "\nMax at column " << index << ": " << grid.ColumnMax(index)
     << std::endl;
}

/**
 * Calculates and displays the mode of a specified column
 * @param grid The DataGrid used for the mode calculation
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MathMenuMode(const cse::DataGrid &grid, std::ostream &os,
                                    std::istream &is) noexcept {
  int index = GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);

  os << "\nBelow is the grid: \n";
  grid.Print(os);

  os << "\nMode(s) at column " << index << ": ";
  for (const double &mode_val : grid.ColumnMode(index)) {
    os << mode_val << " ";
  }
  os << std::endl;
}

/**
 * Calculates and displays the mean, median, standard deviation, mode, min, and
 * max of the entire grid
 * @param grid The DataGrid used for the calculations
 * @param os ostream used for output and testing
 */
void FinalApplication::MathMenuSummary(const cse::DataGrid &grid,
                                       std::ostream &os) noexcept {
  os << "\nBelow is the grid: \n";
  grid.Print(os);

  auto [mean, median, mode, standardDeviation, min, max] =
      grid.CalculateDataGridMathSummary();
  os << "\nGrid Summary:" << std::endl;
  os << "Mean: " << mean << std::endl;
  os << "Median: " << median << std::endl;
  os << "Standard Deviation: " << standardDeviation << std::endl;
  os << "Min: " << min << std::endl;
  os << "Max: " << max << std::endl;
  os << "Mode(s): ";
  for (const double &mode_val : mode) {
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
void FinalApplication::ComparisonMenu(cse::DataGrid &grid, std::ostream &os,
                                      std::istream &is) noexcept {
  // Continue to prompt user until 'b' is input
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
    std::getline(is, option);

    if (option == "clt") {
      ComparisonMenuLessThan(grid, os, is);
      return;
    }
    if (option == "clte") {
      ComparisonMenuLessThanEqual(grid, os, is);
      return;
    }
    if (option == "cgt") {
      ComparisonMenuGreaterThan(grid, os, is);
      return;
    }
    if (option == "cgte") {
      ComparisonMenuGreaterThanEqual(grid, os, is);
      return;
    }
    if (option == "ce") {
      ComparisonMenuEqual(grid, os, is);
      return;
    }
    if (option == "cne") {
      ComparisonMenuNotEqual(grid, os, is);
      return;
    }
    if (option == "b") {
      return;
    }
    os << "Invalid option. Try again." << std::endl;
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
                                              std::istream &is) noexcept {
  const int index =
      GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  const Datum datum = GetDataValue(os, is);

  os << "\nThe column values: \n";
  PrintColumn((grid.GetColumn(index)));

  os << "\nValues less than the given value:" << std::endl;
  PrintColumn(grid.ColumnLessThan(index, datum));
}

/**
 * Displays the  results from the less than or equal comparison of a specified
 * column
 * @param grid The DataGrid used for less than or equal comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenuLessThanEqual(cse::DataGrid &grid,
                                                   std::ostream &os,
                                                   std::istream &is) noexcept {
  const int index =
      GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  const Datum datum = GetDataValue(os, is);

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
                                                 std::istream &is) noexcept {
  const int index =
      GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  const Datum datum = GetDataValue(os, is);

  os << "\nThe column values: \n";
  PrintColumn((grid.GetColumn(index)));

  os << "\nValues greater than the given value:" << std::endl;
  PrintColumn(grid.ColumnGreaterThan(index, datum));
}

/**
 * Displays the  results from the greater than or equal comparison of a
 * specified column
 * @param grid The DataGrid used for greater than or equal comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ComparisonMenuGreaterThanEqual(
    cse::DataGrid &grid, std::ostream &os, std::istream &is) noexcept {
  const int index =
      GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  const Datum datum = GetDataValue(os, is);

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
                                           std::istream &is) noexcept {
  const int index =
      GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  const Datum datum = GetDataValue(os, is);

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
                                              std::istream &is) noexcept {
  const int index =
      GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  const Datum datum = GetDataValue(os, is);

  os << "\nThe column values: \n";
  PrintColumn((grid.GetColumn(index)));

  os << "\nValues not equal to the given value:" << std::endl;
  PrintColumn(grid.ColumnNotEqual(index, datum));
}

// ** GridMenu functions **

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
                                         std::istream &is) noexcept {
  // Continue to prompt user until valid input
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

    if (line == "i") {
      if (std::optional<cse::DataGrid> grid = GridMenuImport(os, is))
        return grid.value();
    } else if (line == "t") {
      return GridMenuPreMadeGrid(os);
    } else if (line == "c") {
      cse::DataGrid data_grid = CreateGridMenu(os, is);
      os << "\nBelow is the grid you made: \n";
      data_grid.Print(os);
      return data_grid;
    } else {
      os << "Invalid option. Try again.\n" << std::endl;
    }
  }
}

/**
 * Creates a grid using a .csv file
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return Optional DataGrid if the import worked, otherwise an empty optional
 */
std::optional<cse::DataGrid> FinalApplication::GridMenuImport(
    std::ostream &os, std::istream &is) {
  // Import CSV path
  std::string line;
  os << "Enter CSV filename to import: ";
  if (!std::getline(is, line)) {
    is.clear();
    return {};
  }

  try {
    cse::DataGrid data_grid = cse::CSVFile::LoadCsv(line);

    os << "\nBelow is the grid you imported: \n";
    data_grid.Print(os);

    return data_grid;
  } catch (const std::exception &e) {
    os << "Import failed: " << e.what() << "\n" << std::endl;
  }
  return {};
}

/**
 * Creates a pre-made grid
 * @param os ostream used for output and testing
 * @return A pre-made grid
 */
cse::DataGrid FinalApplication::GridMenuPreMadeGrid(std::ostream &os) noexcept {
  // Pre-made 5×3 grid
  std::vector premade(5, std::vector<Datum>(3));

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

  auto data_grid = cse::DataGrid(premade);
  os << "\nBelow is the pre-made grid: \n";
  data_grid.Print(os);
  return data_grid;
}

// ** CreateGridMenu **

/**
 * Displays the create grid menu that prompts the user to create a custom
 * DataGrid Cite: Made with the help of ChatGPT
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return The custom DataGrid
 */
cse::DataGrid FinalApplication::CreateGridMenu(std::ostream &os,
                                               std::istream &is) noexcept {
  static constexpr int MAX_DEFAULT_STR_LEN = 100;
  static constexpr double MAX_DEFAULT_VALUE = 1e20;

  std::size_t num_rows = CreateGridMenuGetValue("rows", os, is);
  std::size_t num_columns = CreateGridMenuGetValue("columns", os, is);

  std::string line;
  // Choose default value type
  // Continue to prompt user until valid input
  while (true) {
    os << "\nWould you like your default values to be numerical or strings? "
          "[n/s]: ";
    if (!std::getline(is, line)) {
      is.clear();
      continue;
    }

    if (line == "n") {
      // numeric default
      // Continue to prompt user until valid input
      while (true) {
        os << "Enter a numeric default value for the DataGrid: ";
        if (!std::getline(is, line)) {
          is.clear();
          continue;
        }

        std::istringstream ss(line);
        double d;
        if (!(ss >> d) || !(ss >> std::ws).eof()) {
          os << "\nInvalid input. Please enter a valid number." << "\n";
          continue;
        }

        if (std::fabs(d) > MAX_DEFAULT_VALUE) {
          os << "\nInvalid input. Please enter a number between "
             << -MAX_DEFAULT_VALUE << " and " << MAX_DEFAULT_VALUE << ".\n";
          continue;
        }

        return cse::DataGrid(num_rows, num_columns, d);
      }
    }
    if (line == "s") {
      // string default
      // Continue to prompt user until valid input
      while (true) {
        os << "Enter a string default value for the DataGrid (1-"
           << MAX_DEFAULT_STR_LEN << " chars): ";
        if (!std::getline(is, line)) {
          is.clear();
          continue;
        }

        if (line.empty()) {
          os << "Invalid input. Please enter a non-empty string." << std::endl;
          continue;
        }
        if (line.size() > MAX_DEFAULT_STR_LEN) {
          os << "Invalid input. String too long (max " << MAX_DEFAULT_STR_LEN
             << " characters)." << std::endl;
          continue;
        }
        return {num_rows, num_columns, line};
      }
    }
    os << "\nInvalid selection. Type 'n' for numeric or 's' for string."
       << std::endl;
  }
}

/**
 * Obtains the number of row or columns to make
 * @param type The grid type (row or column)
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return
 */
int FinalApplication::CreateGridMenuGetValue(const std::string &type,
                                             std::ostream &os,
                                             std::istream &is) noexcept {
  // Continue to prompt user until valid input
  while (true) {
    constexpr int min_index = 1;
    constexpr int max_index = 1000;
    os << "Enter number of " << type << " for your DataGrid [" << min_index
       << "-" << max_index << "]: ";
    std::string value_str;
    std::getline(is, value_str);
    auto value = ValidInt(value_str);

    if (value && value.value() <= max_index && value.value() >= min_index) {
      return value.value();
    }

    os << "\nInvalid option. Try again. The min value is: " << min_index
       << ". The max value is: " << max_index << std::endl;
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
void FinalApplication::ManipulateGridMenu(cse::DataGrid &grid, std::ostream &os,
                                          std::istream &is) noexcept {
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
    std::optional<int> input_int = ValidInt(input);

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
void FinalApplication::PrintSubmenu(const cse::DataGrid &grid, std::ostream &os,
                                    std::istream &is) noexcept {
  // Continue to prompt user until valid input
  while (true) {
    os << "\n--- Print Options ---" << std::endl;
    os << "1. Print a cell value" << std::endl;
    os << "2. Print a row" << std::endl;
    os << "3. Print a column" << std::endl;
    os << "4. Print entire grid" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";

    std::string raw;
    if (!(std::getline(is, raw))) return;  // no more inputs for this
    auto mi = ValidInt(raw);
    if (!mi) {
      os << "Invalid choice. Input must be an int. Try again." << std::endl;
      continue;
    }

    switch (mi.value()) {
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
        os << "Invalid choice. Input must be between 0-4. Try again."
           << std::endl;
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
                                        std::istream &is) noexcept {
  std::optional<int> row = PrintAndEditSubmenuGetIndex(
      static_cast<int>(std::get<0>(grid.Shape())), os, is, "row");
  if (!row) return;

  std::optional<int> column = PrintAndEditSubmenuGetIndex(
      static_cast<int>(std::get<1>(grid.Shape())), os, is);
  if (!column) return;

  if (const Datum &value = grid.GetValue(row.value(), column.value());
      value.IsDouble()) {
    os << "\nCell (" << row.value() << ", " << column.value()
       << "): " << value.GetDouble() << std::endl;
  } else {
    os << "\nCell (" << row.value() << ", " << column.value()
       << "): " << value.GetString() << std::endl;
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
                                       std::istream &is) noexcept {
  std::optional<int> row = PrintAndEditSubmenuGetIndex(
      static_cast<int>(std::get<0>(grid.Shape())), os, is, "row");
  if (!row) return;

  auto row_data = grid.GetRow(row.value());
  os << "\nRow " << row.value() << ": ";
  for (const auto &d : row_data) {
    if (d.IsDouble())
      os << d.GetDouble() << " ";
    else
      os << d.GetString() << " ";
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
                                          std::istream &is) noexcept {
  std::optional<int> column = PrintAndEditSubmenuGetIndex(
      static_cast<int>(std::get<1>(grid.Shape())), os, is);
  if (!column) return;

  auto col_data = grid.GetColumn(column.value());
  os << "\nColumn " << column.value() << ": ";
  for (const auto &d : col_data) {
    if (d.IsDouble())
      os << d.GetDouble() << " ";
    else
      os << d.GetString() << " ";
  }
  os << std::endl;
}

/**
 * Gets the row or column value from the user.
 * @param max_index The maximum value a row or column can be
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @param type The type of index (row or column)
 * @return Optional int if it's a valid row value, empty optional if not
 */
std::optional<int> FinalApplication::PrintAndEditSubmenuGetIndex(
    const int max_index, std::ostream &os, std::istream &is,
    const std::string &type) noexcept {
  os << "Enter " << type << " index: ";
  std::string input;
  if (!(std::getline(is, input))) return {};

  std::optional<int> is_valid_int = ValidInt(input);
  if (!is_valid_int || is_valid_int.value() < 0 ||
      is_valid_int.value() >= max_index) {
    os << "Invalid " << type << ". " << "The max " << type
       << " is: " << max_index - 1 << ". Try again." << std::endl;
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
void FinalApplication::EditSubmenu(cse::DataGrid &grid, std::ostream &os,
                                   std::istream &is) noexcept {
  // Continue to prompt user until valid input
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

    auto mi = ValidInt(raw_choice);
    if (!mi) {
      os << "Invalid choice. Input must be an int. Try again." << std::endl;
      continue;
    }

    switch (mi.value()) {
      // Edit a cell value
      case 1: {
        EditSubmenuCell(grid, os, is);
        break;
      }
      // Edits an entire row's value
      case 2: {
        EditSubmenuRow(grid, os, is);
        break;
      }
      // Edits an entire column's value
      case 3: {
        EditSubmenuColumn(grid, os, is);
        break;
      }
      case 0:
        return;
      default:
        os << "Invalid choice. Input must be between 0-3. Try again."
           << std::endl;
    }
  }
}

/**
 * Edits a cell based on the user input
 * @param grid The DataGrid to edit
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::EditSubmenuCell(cse::DataGrid &grid, std::ostream &os,
                                       std::istream &is) noexcept {
  std::optional<int> row = PrintAndEditSubmenuGetIndex(
      static_cast<int>(std::get<0>(grid.Shape())), os, is, "row");
  if (!row) return;

  std::optional<int> column = PrintAndEditSubmenuGetIndex(
      static_cast<int>(std::get<1>(grid.Shape())), os, is);
  if (!column) return;

  // new value (string or double)
  os << "Enter new value: ";
  std::string value_input;

  if (!(std::getline(is, value_input))) return;
  if (std::optional<double> datum = ValidDouble(value_input)) {
    grid.At(row.value(), column.value()) = Datum(datum.value());
  } else {
    grid.At(row.value(), column.value()) = Datum(value_input);
  }

  os << "\nCell updated." << std::endl;

  os << "\nThe Grid is now:" << std::endl;
  grid.Print(os);
}

/**
 * Edits a row based on the user input
 * @param grid The DataGrid to edit
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::EditSubmenuRow(cse::DataGrid &grid, std::ostream &os,
                                      std::istream &is) noexcept {
  std::optional<int> row = PrintAndEditSubmenuGetIndex(
      static_cast<int>(std::get<0>(grid.Shape())), os, is, "row");
  if (!row) return;

  auto &rowData = grid.GetRow(row.value());

  os << "Enter " << rowData.size() << " new values one at a time: \n";
  for (std::size_t i = 0; i < rowData.size(); ++i) {
    os << "Value " << i + 1 << ":" << std::endl;
    std::string value;

    if (!(std::getline(is, value))) return;

    if (auto datum = ValidDouble(value)) {
      rowData[i] = Datum(datum.value());
    } else {
      rowData[i] = Datum(value);
    }
  }
  os << "\nRow updated." << std::endl;

  os << "\nThe Grid is now:" << std::endl;
  grid.Print(os);
}

/**
 * Edits a column based on the user input
 * @param grid The DataGrid to edit
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::EditSubmenuColumn(cse::DataGrid &grid, std::ostream &os,
                                         std::istream &is) noexcept {
  std::optional<int> column = PrintAndEditSubmenuGetIndex(
      static_cast<int>(std::get<1>(grid.Shape())), os, is);
  if (!column) return;

  std::size_t column_size = static_cast<int>(std::get<1>(grid.Shape()));
  os << "Enter " << column_size << " new values one at a time: \n";
  for (std::size_t i = 0; i < column_size; ++i) {
    os << "Value " << i + 1 << ":" << std::endl;
    std::string value;

    if (!(std::getline(is, value))) return;

    if (auto datum = ValidDouble(value)) {
      grid.At(i, column.value()) = Datum(datum.value());
    } else {
      grid.At(i, column.value()) = Datum(value);
    }
  }
  os << "\nColumn updated." << std::endl;

  os << "\nThe Grid is now:" << std::endl;
  grid.Print(os);
}

// ** SortSubmenu **

/**
 * Displays the sort menu and sorts the DataGrid based on the user input.
 * Supported options: Sort column or the entire data grid.
 * @param grid The DataGrid to sort
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::SortSubmenu(cse::DataGrid &grid, std::ostream &os,
                                   std::istream &is) noexcept {
  int choice = -1;
  do {
    os << "\n--- Sort Options ---" << std::endl;
    os << "1. Sort grid rows by a specified column" << std::endl;
    os << "2. Sort entire grid (using left-most columns as keys)" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    std::string input;
    std::getline(is, input);

    std::optional<int> input_int = ValidInt(input);

    if (input_int.has_value()) {
      choice = input_int.value();
      switch (choice) {
        // Sort grid rows by a specified column
        case 1: {
          SortSubmenuColumnOrGrid(grid, true, os, is);
          break;
        }
        // Sort entire grid (using left-most columns as keys)
        case 2: {
          SortSubmenuColumnOrGrid(grid, false, os, is);
          break;
        }
        case 0:
          break;
        default:
          os << "Invalid choice. Input must be between 0-2.\n" << std::endl;
      }
    } else {
      os << "Invalid choice. Cannot be a string. Try again.\n";
    }
  } while (choice != 0);
}

/**
 * Sorts the DataGrid based on the user input.
 * Supported options: Sort column or the entire data grid.
 * @param grid The DataGrid to sort
 * @param is_column_sort
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::SortSubmenuColumnOrGrid(cse::DataGrid &grid,
                                               const bool &is_column_sort,
                                               std::ostream &os,
                                               std::istream &is) noexcept {
  int column = 0;
  if (is_column_sort) {
    column = GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
  }

  std::string order;
  // Continue to prompt user until valid input
  while (true) {
    os << "Enter 1 for ascending, 0 for descending: ";
    std::getline(is, order);
    std::optional<int> ordering_input = ValidInt(order);

    if (ordering_input &&
        (ordering_input.value() == 0 || ordering_input.value() == 1)) {
      if (is_column_sort) {
        grid.SortColumn(column, ordering_input.value());
        os << "Grid rows sorted by column " << column << ".\n" << std::endl;
      }
      grid.Sort(ordering_input.value());
      os << "Entire grid sorted.\n" << std::endl;

      os << "The Grid is now:" << std::endl;
      grid.Print(os);
      break;
    }
    os << "Invalid input. The input must be 0 or 1" << std::endl;
  }
}

// ** AddSubmenu **

/**
 * Displays the add menu and adds to the DataGrid based on the user input.
 * Supported options: Add a row, column, or merge DataGrids
 * @param grid The DataGrid to add to
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::AddSubmenu(cse::DataGrid &grid, std::ostream &os,
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

    std::optional<int> input_int = ValidInt(main_input);

    if (input_int.has_value()) {
      choice = input_int.value();
      try {
        switch (choice) {
          // Add row
          case 1: {
            std::size_t num_cols = std::get<1>(grid.Shape());

            std::string method;

            // Continue to prompt user until valid input
            while (true) {
              os << "Add default row (d), with an equation (e), or enter "
                    "manually (m)? ";
              std::getline(is, method);
              if (method == "d" || method == "e" || method == "m") {
                break;
              }
              os << "Invalid option. Must be 'd', 'e', or 'm'.\n" << std::endl;
            }

            if (method == "d") {
              os << "\nEnter default value (number or string): ";
              std::string input;
              std::getline(is, input);
              auto num = ValidDouble(input);
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
              std::vector<Datum> new_row;
              os << "\nEnter " << num_cols
                 << " values for the new row one at a time: \n";
              for (std::size_t i = 0; i < num_cols; ++i) {
                os << "Value " << i + 1 << ":" << std::endl;
                std::string val;
                std::getline(is, val);
                auto num = ValidDouble(val);
                new_row.emplace_back(num.has_value() ? Datum(num.value())
                                                     : Datum(val));
              }
              grid.InsertRow(new_row);
              os << "Row added." << std::endl;

              os << "\nBelow is the new grid: \n";
              grid.Print(os);
              // Add a custom row with an equation
            } else if (method == "e") {
              std::vector<Datum> new_row;

              // Continue to prompt user until valid input
              std::string equation;
              while (true) {
                os << "\nEnter equation:" << std::endl;
                os << "Supported operators: +, -, /, *, and ^" << std::endl;
                os << "Insert row indexes in the curly braces {}. For example, "
                      "{0}"
                   << std::endl;
                os << "Separate each action with a single space" << std::endl;
                os << "Examples: \n1) {0} + {1}  \n2) {0} * {0} / {1} \n3) {0} "
                      "^ {0} - {1} \n";
                std::getline(is, equation);
                if (IsValidCustomEquation(
                        equation,
                        static_cast<int>(std::get<0>(grid.Shape())))) {
                  break;
                }
                os << "Invalid equation. Try again. \n";
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

            // Continue to prompt user until valid input
            std::string method;
            while (true) {
              os << "Add default row (d), with an equation (e), or enter "
                    "manually (m)? ";
              std::getline(is, method);
              if (method == "d" || method == "e" || method == "m") {
                break;
              }
              os << "Invalid option. Must be 'd', 'e', or 'm'.\n" << std::endl;
            }

            // Add a default column
            if (method == "d") {
              os << "\nEnter default value (number or string): ";
              std::string input;
              std::getline(is, input);
              auto num = ValidDouble(input);
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
              std::vector<Datum> new_column;
              os << "\nEnter " << num_rows
                 << " values for the new column one at a time: \n";
              for (std::size_t i = 0; i < num_rows; ++i) {
                os << "Value " << i + 1 << ":" << std::endl;
                std::string val;
                std::getline(is, val);
                auto num = ValidDouble(val);
                new_column.emplace_back(num.has_value() ? Datum(num.value())
                                                        : Datum(val));
              }
              grid.InsertColumn(new_column);

              os << "Column added." << std::endl;

              os << "\nBelow is the new grid: \n";
              grid.Print(os);

              // Add a custom column by an equation
            } else if (method == "e") {
              std::vector<Datum> new_col;

              // Continue to prompt user until valid input
              std::string equation;
              while (true) {
                os << "\nEnter equation:" << std::endl;
                os << "Supported operators: +, -, /, *, and ^" << std::endl;
                os << "Insert column indexes in the curly braces {}. For "
                      "example, {0}"
                   << std::endl;
                os << "Separate each action with a single space" << std::endl;
                os << "Examples: \n1) {0} + {1}  \n2) {0} * {0} / {1} \n3) {0} "
                      "^ {0} - {1} \n";
                std::getline(is, equation);
                if (IsValidCustomEquation(
                        equation,
                        static_cast<int>(std::get<1>(grid.Shape())))) {
                  break;
                }
                os << "Invalid equation. Try again. \n";
              }

              cse::ExpressionParser<std::vector<Datum>> parser;
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
            // Continue to prompt user until valid input
            std::string merge_type;
            while (true) {
              os << "Enter merge type (1 for row append, 0 for column "
                    "append): ";
              std::getline(is, merge_type);
              if (merge_type == "1" || merge_type == "0") {
                break;
              }
              os << "Invalid input. Must be 0 or 1 \n" << std::endl;
            }
            int merge_type_int = std::stoi(merge_type);

            std::string merge_rows;
            // Continue to prompt user until valid input
            while (true) {
              os << "\nEnter number of rows for merging grid: ";
              std::getline(is, merge_rows);
              if (std::optional<int> rows_int = ValidInt(merge_rows);
                  rows_int.has_value() && rows_int.value() > 0) {
                if (merge_type == "1" ||
                    rows_int.value() ==
                        static_cast<int>(std::get<0>(grid.Shape()))) {
                  break;
                }
                os << "Invalid row size. The row must equal your current "
                      "grid's row size:"
                   << std::get<0>(grid.Shape()) << "\n"
                   << std::endl;
              } else {
                os << "Invalid input. Must be an valid int \n" << std::endl;
              }
            }
            int merge_rows_int = std::stoi(merge_rows);

            std::string merge_cols;
            // Continue to prompt user until valid input
            while (true) {
              os << "\nEnter number of columns for merging grid: ";
              std::getline(is, merge_cols);
              if (std::optional<int> col_int = ValidInt(merge_cols);
                  col_int.has_value() && col_int.value() > 0) {
                if (merge_type == "0" ||
                    col_int.value() ==
                        static_cast<int>(std::get<1>(grid.Shape()))) {
                  break;
                }
                os << "Invalid column size. The row must equal the current "
                      "grid's column size:"
                   << std::get<1>(grid.Shape()) << "\n"
                   << std::endl;
              } else {
                os << "Invalid input. Must be an valid int \n" << std::endl;
              }
            }
            int merge_col_int = std::stoi(merge_cols);

            std::vector merge_data(merge_rows_int,
                                   std::vector<Datum>(merge_col_int));
            os << "Enter " << merge_rows_int * merge_col_int
               << " values for the merging grid:" << std::endl;
            int counter = 1;
            for (int i = 0; i < merge_rows_int; ++i) {
              for (int j = 0; j < merge_col_int; ++j) {
                os << "Value (" << counter << "/"
                   << merge_rows_int * merge_col_int << "):" << std::endl;
                std::string val;
                std::getline(is, val);
                auto num = ValidDouble(val);
                merge_data[i][j] =
                    num.has_value() ? Datum(num.value()) : Datum(val);
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
            os << "Invalid choice. Input must be between 0-3. Try again."
               << std::endl;
        }
      } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    } else {
      os << "Invalid choice. Cannot be a string. Try again.\n";
    }
  } while (choice != 0);
}

// ** DeleteSubMenu **

/**
 * Displays the delete menu and deletes parts of the DataGrid based on the user
 * input. Supported options: Delete a row, column, or entire DataGrid
 * @param grid The DataGrid to delete from
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::DeleteSubmenu(cse::DataGrid &grid, std::ostream &os,
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

    std::optional<int> input_int = ValidInt(input);

    if (input_int.has_value()) {
      choice = input_int.value();
      try {
        switch (choice) {
          // Delete a row
          case 1: {
            DeleteSubmenuRow(grid, os, is);
            break;
          }
            // Delete a column
          case 2: {
            DeleteSubmenuColumn(grid, os, is);
            break;
          }
            // Clear the entire grid
          case 3: {
            DeleteSubmenuGrid(grid, os);
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
    } else {
      os << "Invalid choice. Cannot be a string. Try again.\n";
    }
  } while (choice != 0);
}

/**
 * Deletes a row from the grid based on the user input
 * @param grid The grid to delete the row from
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::DeleteSubmenuRow(cse::DataGrid &grid, std::ostream &os,
                                        std::istream &is) noexcept {
  grid.DeleteRow(
      GetIndex(static_cast<int>(std::get<0>(grid.Shape())), os, is, "row"));
  os << "Row deleted." << std::endl;

  os << "\nBelow is the new grid: \n";
  grid.Print(os);
}

/**
 * Deletes a column from the grid based on the user input
 * @param grid The grid to delete the column from
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::DeleteSubmenuColumn(cse::DataGrid &grid,
                                           std::ostream &os,
                                           std::istream &is) noexcept {
  grid.DeleteColumn(
      GetIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is));
  os << "Column deleted." << std::endl;

  os << "\nBelow is the new grid: \n";
  grid.Print(os);
}

/**
 * Deletes the entire grid
 * @param grid The grid to delete
 * @param os ostream used for output and testing
 */
void FinalApplication::DeleteSubmenuGrid(cse::DataGrid &grid,
                                         std::ostream &os) noexcept {
  grid.Clear();
  os << "Grid cleared." << std::endl;

  os << "\nBelow is the new grid: \n";
  grid.Print(os);
}

// ** ResizeSubMenu **

/**
 * Displays the resize menu and resizes the DataGrid based on the user input.
 * Supported options: Resize via double or string
 * @param grid The DataGrid to resize
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ResizeSubmenu(cse::DataGrid &grid, std::ostream &os,
                                     std::istream &is) noexcept {
  int choice = -1;
  do {
    os << "\n--- Resizing Options ---" << std::endl;
    os << "1. Resize grid" << std::endl;
    os << "0. Return to previous menu" << std::endl;
    os << "Enter your choice: ";
    std::string input;
    std::getline(is, input);

    std::optional<int> input_int = ValidInt(input);

    if (input_int.has_value()) {
      choice = input_int.value();
      switch (choice) {
        // Resize the grid (double)
        case 1: {
          ResizeSubmenuResizeGrid(grid, os, is);
          break;
        }
        case 0:
          break;
        default:
          os << "Invalid resize option. Must be between 0-2. Try again."
             << std::endl;
      }
    } else {
      os << "Invalid choice. Cannot be a string. Try again.\n";
    }
  } while (choice != 0);
}

/**
 * Resizes and displays the new grid based on the user input
 * @param grid The grid to resize
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::ResizeSubmenuResizeGrid(cse::DataGrid &grid,
                                               std::ostream &os,
                                               std::istream &is) noexcept {
  int row_index = ResizeSubmenuGetValidInput("row", os, is);
  int column_index = ResizeSubmenuGetValidInput("column", os, is);

  std::string default_value;
  os << "Enter default value: ";
  std::getline(is, default_value);
  if (ValidDouble(default_value)) {
    grid.Resize(row_index, column_index, std::stod(default_value));
  } else {
    grid.Resize(row_index, column_index, default_value);
  }
  os << "Grid resized." << std::endl;

  os << "\nBelow is the new grid: \n";
  grid.Print(os);
}

/**
 * Retrieves row and column input from the user for resizing the grid
 * @param type The type of input (row or column)
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 * @return
 */
int FinalApplication::ResizeSubmenuGetValidInput(const std::string &type,
                                                 std::ostream &os,
                                                 std::istream &is) noexcept {
  static constexpr int MAX_VALUE = 1000;
  static constexpr int MIN_VALUE = 1;

  // Continue to prompt user until valid input
  std::string input;
  while (true) {
    os << "Enter new number of " << type << " (" << MIN_VALUE << "-"
       << MAX_VALUE << "):";
    std::getline(is, input);

    std::optional new_rows_int = ValidInt(input);
    if (new_rows_int.has_value() && new_rows_int.value() >= MIN_VALUE &&
        new_rows_int.value() <= MAX_VALUE) {
      return new_rows_int.value();
    }
    os << "Invalid input. Please enter a number between " << MIN_VALUE
       << " and " << MAX_VALUE << ".\n";
  }
}

// ** MainMenu **

/**
 * Displays the main menu and displays the possible options.
 * Supported options: Export, Edit, Math, Comparisons
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MainMenu(std::ostream &os, std::istream &is) noexcept {
  os << "\nWelcome to CSV Command Line Manipulator!" << std::endl;
  os << "This is a command line application that allows users to create, load, "
        "manipulate, analyze and save CSV file data\n"
     << std::endl;
  os << "Developed by: Max Krawec, Calen Green, Pedro Mitkiewicz, "
        "Shahaab Ali, and Muhammad Asif Masood"
     << std::endl;
  os << std::endl;

  cse::DataGrid grid = GridMenu(os, is);

  // User must select 'q' to stop
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
      MainMenuExport(grid, os, is);
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

/**
 * Exports datagrid to a .csv file
 * @param grid The grid to export
 * @param os ostream used for output and testing
 * @param is istream used for input and testing
 */
void FinalApplication::MainMenuExport(const cse::DataGrid &grid,
                                      std::ostream &os, std::istream &is) {
  // Continue to prompt user until valid file name
  std::string filename;
  while (true) {
    os << "Enter CSV filename to export: ";
    std::getline(is, filename);
    if (filename.ends_with(".csv")) {
      break;
    }
    os << "Invalid filename. The file must end with .csv" << "\n";
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
}