/**
 * @file FinalApplication.cpp
 * @brief The CSV Command Line Manipulator.
 *
 * Developed by: Max Krawec, Calen Green, Pedro Mitkiewicz, Shahaab Ali,
 * and Muhammad Asif Masood
 *
 * Portions of this code are made with the assistance of ChatGPT
 */


#include "FinalApplication.h"
#include "src/Datum.h"
#include "src/DataGrid.h"
#include "src/ExpressionParser.cpp"
#include "src/ExpressionParser.h"
#include "src/CSVfile.h"


#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

// Returns an optional double from the input string
std::optional<double> FinalApplication::IsValidDouble(const std::string &input) {
  try {
    std::size_t pos;
    double value = std::stod(input, &pos);
    if (pos == input.length()) {
      return value;
    }
  } catch (std::invalid_argument &) {}
  return std::nullopt;
}

// Returns an optional int from the input string
std::optional<int> FinalApplication::IsValidInt(const std::string &input) {
  try {
    std::size_t pos;
    int value = std::stoi(input, &pos);
    if (pos == input.length()) {
      return value;
    }
  } catch (std::invalid_argument &) {}
  return std::nullopt;
}

// Repeatedly prompts for valid column index
int FinalApplication::GetColumnIndex(int max_index, std::ostream &os = std::cout, std::istream &is = std::cin) {
  while (true) {
    os << "Please enter column index: ";
    std::string index_str;
    is >> index_str;
    auto index = IsValidInt(index_str);
    if (index && index.value() < max_index) {
      return index.value();
    }
    os << "Invalid option. Try again." << std::endl;
  }
}

// Requests and reads a Datum value
cse::Datum FinalApplication::GetDataValue(std::ostream &os = std::cout, std::istream &is = std::cin) {
  os << "Please enter the value to compare: ";
  std::string value_str;
  is >> value_str;
  if (auto d = IsValidDouble(value_str)) {
    return {d.value()};
  }
  return {value_str};
}

// Prints a column
void FinalApplication::PrintColumn(const cse::ReferenceVector<cse::Datum> &column, std::ostream &os = std::cout) {
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
 * @brief Creates DataGrid from user input
 * @return DataGrid
 */
cse::DataGrid FinalApplication::CreateGrid(std::ostream &os = std::cout, std::istream &is = std::cin) {
  std::size_t num_rows = 0, num_columns = 0;
  std::string type_choice;

  while (true) {
    os << "Enter number of rows for your DataGrid: ";
    is >> num_rows;
    if (is.fail() || num_rows == 0) {

      // Clears error flags and input buffer
      is.clear();
      is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      os << "Invalid input. Please enter a positive integer." << std::endl;
    } else {
      // Clears input buffer
      is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    }
  }

  while (true) {
    os << "Enter number of columns for your DataGrid: ";
    is >> num_columns;
    if (is.fail() || num_columns == 0) {

      // Clears error flags and input buffer
      is.clear();
      is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      os << "Invalid input. Please enter a positive integer." << std::endl;
    } else {
      // Clears input buffer
      is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    }
  }

  while (true) {
    os << "Would you like your default values to be numerical or strings? [n/s]: ";
    std::getline(is >> std::ws, type_choice);
    if (type_choice == "n") {
      double default_value;
      while (true) {
        os << "Enter a numeric default value for the DataGrid: ";
        is >> default_value;
        if (is.fail()) {

          // Clears error flags and input buffer
          is.clear();
          is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

          os << "Invalid input. Please enter a valid number." << std::endl;
        } else {
          // Clears input buffer
          is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          return cse::DataGrid(num_rows, num_columns, default_value);
        }
      }
    } else if (type_choice == "s") {
      std::string default_value;
      os << "Enter a string default value for the DataGrid: ";
      std::getline(is >> std::ws, default_value);
      return {num_rows, num_columns, default_value};
    } else {
      os << "Invalid selection. Type 'n' for numeric or 's' for string." << std::endl;
    }
  }
}

/**
 * @brief Displays the grid menu and returns a DataGrid
 * @return DataGrid
 */
cse::DataGrid FinalApplication::GridMenu(std::ostream &os = std::cout, std::istream &is = std::cin) {
  while (true) {
    os << "Menu Option:" << std::endl;
    os << "i: Import a CSV file" << std::endl;
    os << "t: Create a test grid" << std::endl;
    os << "c: Create a new DataGrid" << std::endl;
    os << "Enter an option: ";
    std::string option;
    is >> option;

    if (option == "i") {
      std::string filename;
      os << "Enter CSV filename to import: ";
      is >> filename;
      try {
        cse::DataGrid grid = cse::CSVFile::LoadCsv(filename);
        return grid;
      } catch (const std::exception &e) {
        std::cerr << "Import failed: " << e.what() << "\n";
      }
    } else if (option == "t") {
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

      return cse::DataGrid(test_grid);
    } else if (option == "c") {
      return CreateGrid();
    } else {
      os << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * @brief Displays the math menu and outputs requested statistics
 * @param grid DataGrid to perform calculations on
 */
void FinalApplication::MathMenu(const cse::DataGrid &grid, std::ostream &os = std::cout, std::istream &is = std::cin) {
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
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Mean at column " << index << ": " << grid.ColumnMean(index) << std::endl;
      return;
    } else if (option == "cmed") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Median at column " << index << ": " << grid.ColumnMedian(index) << std::endl;
      return;
    } else if (option == "csd") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Standard deviation at column " << index << ": "
                << grid.ColumnStandardDeviation(index) << std::endl;
      return;
    } else if (option == "cmin") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Min at column " << index << ": " << grid.ColumnMin(index) << std::endl;
      return;
    } else if (option == "cmax") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
      os << "Max at column " << index << ": " << grid.ColumnMax(index) << std::endl;
      return;
    } else if (option == "cmode") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())), os, is);
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
 * @brief Displays the comparisons menu and outputs the comparisons
 * @param grid The DataGrid to perform comparisons on
 */
void FinalApplication::ComparisonMenu(cse::DataGrid &grid, std::ostream &os = std::cout, std::istream &is = std::cin) {
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
    is >> option;

    if (option == "clt") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())));
      cse::Datum datum = GetDataValue();
      os << "Values less than given value:" << std::endl;
      PrintColumn(grid.ColumnLessThan(index, datum));
      return;
    } else if (option == "clte") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())));
      cse::Datum datum = GetDataValue();
      os << "Values less than or equal to given value:" << std::endl;
      PrintColumn(grid.ColumnLessThanOrEqual(index, datum));
      return;
    } else if (option == "cgt") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())));
      cse::Datum datum = GetDataValue();
      os << "Values greater than given value:" << std::endl;
      PrintColumn(grid.ColumnGreaterThan(index, datum));
      return;
    } else if (option == "cgte") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())));
      cse::Datum datum = GetDataValue();
      os << "Values greater than or equal to given value:" << std::endl;
      PrintColumn(grid.ColumnGreaterThanOrEqual(index, datum));
      return;
    } else if (option == "ce") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())));
      cse::Datum datum = GetDataValue();
      os << "Values equal to given value:" << std::endl;
      PrintColumn(grid.ColumnEqual(index, datum));
      return;
    } else if (option == "cne") {
      int index = GetColumnIndex(static_cast<int>(std::get<1>(grid.Shape())));
      cse::Datum datum = GetDataValue();
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
 * @brief Displays the CSV grid manipulation menu
 * @param grid DataGrid
 */
void FinalApplication::ManipulateGridMenu(cse::DataGrid &grid, std::ostream &os = std::cout, std::istream &is = std::cin) {
  int choice = 0;
  do {
    os << "\n=== CSV Grid Manipulation Menu ===" << std::endl;
    os << "1. Print options" << std::endl;
    os << "2. Edit options" << std::endl;
    os << "3. Sort options" << std::endl;
    os << "4. Adding options" << std::endl;
    os << "5. Deleting options" << std::endl;
    os << "6. Resizing options" << std::endl;
    os << "7. Custom Row" << std::endl;
    os << "8. Custom Column" << std::endl;
    os << "0. Return to main menu" << std::endl;
    os << "Enter your choice: ";
    is >> choice;

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
        os << "Invalid choice. Try again." << std::endl;
    }
  } while (choice != 0);
}

/**
 * @brief Displays print submenu options
 * @param grid
 */
void FinalApplication::PrintSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout, std::istream &is = std::cin) {
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
        case 1: {
          std::size_t row = 0, col = 0;
          os << "Enter row index: ";
          is >> row;
          os << "Enter column index: ";
          is >> col;
          const cse::Datum& value = grid.GetValue(row, col);
          if (value.IsDouble()) {
            os << "Cell (" << row << ", " << col << "): " << value.GetDouble() << std::endl;
          } else if (value.IsString()) {
            os << "Cell (" << row << ", " << col << "): " << value.GetString() << std::endl;
          }
          break;
        }
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
 * @brief Displays edit submenu options
 * @param grid
 */
void FinalApplication::EditSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout, std::istream &is = std::cin) {
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
 * @brief Displays the sort submenu options
 * @param grid DataGrid
 */
void FinalApplication::SortSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout, std::istream &is = std::cin) {
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
 * @brief Displays the adding submenu options
 * @param grid DataGrid
 */
void FinalApplication::AddSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout, std::istream &is = std::cin) {
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
        case 1: {
          std::size_t num_cols = std::get<1>(grid.Shape());
          os << "Add default row (d), with an equation (e), or enter manually (m)? ";
          std::string method;
          is >> method;

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

          } else if (method == "m") {
            std::vector<cse::Datum> new_row;
            os << "Enter " << num_cols << " values for the new row: ";
            for (std::size_t i = 0; i < num_cols; ++i) {
              std::string val;
              is >> val;
              auto num = IsValidDouble(val);
              new_row.emplace_back(num.has_value() ? cse::Datum(num.value()) : cse::Datum(val));
            }
            grid.InsertRow(new_row);
            os << "Row added." << std::endl;

          } else if (method == "e"){
            std::vector<cse::Datum> new_row;
            os << "Enter equation. Supported operators +-/*^, indexes in curly braces {}, seperate with a single space: ";
            std::string equation;
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(is, equation);
            os<<equation<<std::endl;
            cse::ExpressionParser<cse::ReferenceVector<Datum>> parser;
            cse::ReferenceVector<Datum> col;
            size_t index=0;
            auto func = parser.MakeFunc(equation, 0, index);
            for(size_t i=0;i<num_cols;++i){
              col= grid.GetColumn(i);
              new_row.push_back(func(col));
            }
            grid.InsertRow(new_row);
            os<<"Row Added" <<std::endl;
          }
          else {
            os << "Invalid option. Must be 'd', 'e', or 'm'.";
          }
          break;
        }

        case 2: {
          std::size_t num_rows = std::get<0>(grid.Shape());
          os << "Add default column (d), column by equation (e), or enter manually (m)? ";
          std::string method;
          is >> method;

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

          } else if (method == "m") {
            std::vector<cse::Datum> new_column;
            os << "Enter " << num_rows << " values for the new column: ";
            for (std::size_t i = 0; i < num_rows; ++i) {
              std::string val;
              is >> val;
              auto num = IsValidDouble(val);
              new_column.emplace_back(num.has_value() ? cse::Datum(num.value()) : cse::Datum(val));
            }
            grid.InsertColumn(new_column);
            os << "Column added." << std::endl;

          }else if (method == "e"){
            std::vector<cse::Datum> new_col;
            os << "Enter equation. Supported operators +-/*^, indexes in curly braces {}, seperate with a single space: ";
            std::string equation;
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(is, equation);
            cse::ExpressionParser<std::vector<cse::Datum>> parser;
            std::vector<Datum> row;
            size_t index=0;
            auto func = parser.MakeFunc(equation, 0, index);
            for(size_t i=0;i<num_rows;++i){
              row = grid.GetRow(i);
              new_col.push_back(func(row));
            }
            grid.InsertColumn(new_col);
            os<<"Col Added" <<std::endl;
          }
          else {
            os << "Invalid option. Must be 'd', 'e', or 'm'." << std::endl;
          }
          break;
        }

        case 3: {
          int merge_type = 0;
          os << "Enter merge type (1 for row append, 0 for column append): ";
          is >> merge_type;
          std::size_t merge_rows = 0, merge_cols = 0;
          os << "Enter number of rows for merging grid: ";
          is >> merge_rows;
          os << "Enter number of columns for merging grid: ";
          is >> merge_cols;

          std::vector<std::vector<cse::Datum>> merge_data(merge_rows,
                                                          std::vector<cse::Datum>(merge_cols));
          os << "Enter values for the merging grid:" << std::endl;
          for (std::size_t i = 0; i < merge_rows; ++i) {
            for (std::size_t j = 0; j < merge_cols; ++j) {
              std::string val;
              is >> val;
              auto num = IsValidDouble(val);
              merge_data[i][j] = num.has_value() ? cse::Datum(num.value()) : cse::Datum(val);
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
 * @brief Displays the delete submenu options
 * @param grid DataGrid
 */
void FinalApplication::DeleteSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout, std::istream &is = std::cin) {
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
        case 1: {
          std::size_t row = 0;
          os << "Enter row index to delete: ";
          is >> row;
          grid.DeleteRow(row);
          os << "Row deleted." << std::endl;
          break;
        }
        case 2: {
          std::size_t col = 0;
          os << "Enter column index to delete: ";
          is >> col;
          grid.DeleteColumn(col);
          os << "Column deleted." << std::endl;
          break;
        }
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
 * @brief Displays the resizing submenu options
 * @param grid DataGrid
 */
void FinalApplication::ResizeSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout, std::istream &is = std::cin) {
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
 * @brief main (Program Entry Point)
 */
void FinalApplication::StartingMenu(std::ostream &os = std::cout, std::istream &is = std::cin) {
  os << "Welcome to CSV Command Line Manipulator" << std::endl;
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
    }
    else if (option == "e") {
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