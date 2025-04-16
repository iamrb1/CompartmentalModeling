/**
 * @file main.cpp
 * @brief Entry point for the CSV Command Line Manipulator.
 *
 * Developed by: Max Krawec, Calen Green, Pedro Mitkiewicz, Shahaab Ali,
 * and Muhammad Asif Masood
 *
 * Portions of this code are made with the assistance of ChatGPT
 */

#include "src/Datum.h"
#include "src/DataGrid.h"

#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

cse::DataGrid GridMenu();
cse::DataGrid CreateGrid();
void ManipulateGridMenu(cse::DataGrid &grid);
void MathMenu(const cse::DataGrid &grid);
void ComparisonMenu(cse::DataGrid &grid);
void PrintSubmenu(cse::DataGrid &grid);
void EditSubmenu(cse::DataGrid &grid);
void SortSubmenu(cse::DataGrid &grid);
void AddSubmenu(cse::DataGrid &grid);
void DeleteSubmenu(cse::DataGrid &grid);
void ResizeSubmenu(cse::DataGrid &grid);

namespace {

// Returns an optional double parsed from the input string.
std::optional<double> IsValidDouble(const std::string &input) {
  try {
    std::size_t pos;
    double value = std::stod(input, &pos);
    if (pos == input.length()) {
      return value;
    }
  } catch (std::invalid_argument &) {
    // Fall through.
  }
  return std::nullopt;
}

// Returns an optional int parsed from the input string.
std::optional<int> IsValidInt(const std::string &input) {
  try {
    std::size_t pos;
    int value = std::stoi(input, &pos);
    if (pos == input.length()) {
      return value;
    }
  } catch (std::invalid_argument &) {}
  return std::nullopt;
}

// Helper function to repeatedly prompt for a valid column index.
int GetColumnIndex() {
  while (true) {
    std::cout << "Please enter column index: ";
    std::string index_str;
    std::cin >> index_str;
    if (auto index = IsValidInt(index_str)) {
      return index.value();
    }
    std::cout << "Invalid option. Try again." << std::endl;
  }
}

// Prompts and reads a Datum value from the user.
cse::Datum GetDataValue() {
  std::cout << "Please enter the value to compare: ";
  std::string value_str;
  std::cin >> value_str;
  if (auto d = IsValidDouble(value_str)) {
    return cse::Datum(d.value());
  }
  return cse::Datum(value_str);
}

// Prints the contents of a column (given as a ReferenceVector).
void PrintColumn(const cse::ReferenceVector<cse::Datum> &column) {
  for (const auto &datum : column) {
    if (datum.IsDouble()) {
      std::cout << datum.GetDouble() << " ";
    } else {
      std::cout << datum.GetString() << " ";
    }
  }
  std::cout << std::endl;
}

}  // namespace

/**
 * @brief Prompts the user to create a new DataGrid.
 * @return A newly created DataGrid.
 */
cse::DataGrid CreateGrid() {
  std::size_t num_rows = 0, num_columns = 0;
  std::string type_choice;

  // Prompt for number of rows.
  while (true) {
    std::cout << "Enter number of rows for your DataGrid: ";
    std::cin >> num_rows;
    if (std::cin.fail() || num_rows == 0) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a positive integer." << std::endl;
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    }
  }

  // Prompt for number of columns.
  while (true) {
    std::cout << "Enter number of columns for your DataGrid: ";
    std::cin >> num_columns;
    if (std::cin.fail() || num_columns == 0) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a positive integer." << std::endl;
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    }
  }

  // Determine default value type.
  while (true) {
    std::cout << "Would you like your default values to be numerical or strings? [n/s]: ";
    std::getline(std::cin >> std::ws, type_choice);
    if (type_choice == "n") {
      double default_value;
      while (true) {
        std::cout << "Enter a numeric default value for the DataGrid: ";
        std::cin >> default_value;
        if (std::cin.fail()) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cout << "Invalid input. Please enter a valid number." << std::endl;
        } else {
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          return cse::DataGrid(num_rows, num_columns, default_value);
        }
      }
    } else if (type_choice == "s") {
      std::string default_value;
      std::cout << "Enter a string default value for the DataGrid: ";
      std::getline(std::cin >> std::ws, default_value);
      return cse::DataGrid(num_rows, num_columns, default_value);
    } else {
      std::cout << "Invalid selection. Type 'n' for numeric or 's' for string." << std::endl;
    }
  }
}

/**
 * @brief Displays the grid menu and returns a DataGrid.
 * @return A DataGrid either created for testing or by user input.
 */
cse::DataGrid GridMenu() {
  while (true) {
    std::cout << "Menu Option:" << std::endl;
    std::cout << "i: Import a CSV file (TODO)" << std::endl;
    std::cout << "e: Export to a CSV file (TODO)" << std::endl;
    std::cout << "t: Create a test grid" << std::endl;
    std::cout << "c: Create a new DataGrid" << std::endl;
    std::cout << "Enter an option: ";
    std::string option;
    std::cin >> option;

    if (option == "i") {
      // TODO: Implement CSV import.
      break;
    } else if (option == "e") {
      // TODO: Implement CSV export.
      break;
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
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }
  throw std::runtime_error("Unexpected exit from menu loop.");
}

/**
 * @brief Displays the math menu and outputs requested statistics.
 * @param grid The DataGrid on which to perform calculations.
 */
void MathMenu(const cse::DataGrid &grid) {
  while (true) {
    std::cout << "\nMath Menu:" << std::endl;
    std::cout << "cmean: Calculate the mean of a column" << std::endl;
    std::cout << "cmed: Calculate the median of a column" << std::endl;
    std::cout << "cmode: Calculate the mode of a column" << std::endl;
    std::cout << "csd: Calculate the standard deviation of a column" << std::endl;
    std::cout << "cmin: Calculate the min of a column" << std::endl;
    std::cout << "cmax: Calculate the max of a column" << std::endl;
    std::cout << "sum: Calculate all statistics for the grid" << std::endl;
    std::cout << "b: Go back" << std::endl;
    std::cout << "Enter option: ";
    std::string option;
    std::cin >> option;

    if (option == "cmean") {
      int index = GetColumnIndex();
      std::cout << "Mean at column " << index << ": " << grid.ColumnMean(index) << std::endl;
      return;
    } else if (option == "cmed") {
      int index = GetColumnIndex();
      std::cout << "Median at column " << index << ": " << grid.ColumnMedian(index) << std::endl;
      return;
    } else if (option == "csd") {
      int index = GetColumnIndex();
      std::cout << "Standard deviation at column " << index << ": "
                << grid.ColumnStandardDeviation(index) << std::endl;
      return;
    } else if (option == "cmin") {
      int index = GetColumnIndex();
      std::cout << "Min at column " << index << ": " << grid.ColumnMin(index) << std::endl;
      return;
    } else if (option == "cmax") {
      int index = GetColumnIndex();
      std::cout << "Max at column " << index << ": " << grid.ColumnMax(index) << std::endl;
      return;
    } else if (option == "cmode") {
      int index = GetColumnIndex();
      std::cout << "Mode(s) at column " << index << ": ";
      for (const double &mode_val : grid.ColumnMode(index)) {
        std::cout << mode_val << " ";
      }
      std::cout << std::endl;
      return;
    } else if (option == "sum") {
      auto summary = grid.CalculateDataGridMathSummary();
      std::cout << "Grid Summary:" << std::endl;
      std::cout << "Mean: " << summary.mean << std::endl;
      std::cout << "Median: " << summary.median << std::endl;
      std::cout << "Standard Deviation: " << summary.standardDeviation << std::endl;
      std::cout << "Min: " << summary.min << std::endl;
      std::cout << "Max: " << summary.max << std::endl;
      std::cout << "Mode(s): ";
      for (const double &mode_val : summary.mode) {
        std::cout << mode_val << " ";
      }
      std::cout << std::endl;
      return;
    } else if (option == "b") {
      return;
    } else {
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * @brief Displays the comparisons menu and outputs the result of the comparison.
 * @param grid The DataGrid to perform comparisons on.
 */
void ComparisonMenu(cse::DataGrid &grid) {
  while (true) {
    std::cout << "\nComparison Menu:" << std::endl;
    std::cout << "clt: Find values less than a given value for a column" << std::endl;
    std::cout << "clte: Find values less than or equal to a given value for a column" << std::endl;
    std::cout << "cgt: Find values greater than a given value for a column" << std::endl;
    std::cout << "cgte: Find values greater than or equal to a given value for a column" << std::endl;
    std::cout << "ce: Find values equal to a value for a column" << std::endl;
    std::cout << "cne: Find values not equal to a given value for a column" << std::endl;
    std::cout << "b: Go back" << std::endl;
    std::cout << "Enter option: ";
    std::string option;
    std::cin >> option;

    if (option == "clt") {
      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "Values less than given value:" << std::endl;
      PrintColumn(grid.ColumnLessThan(index, datum));
      return;
    } else if (option == "clte") {
      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "Values less than or equal to given value:" << std::endl;
      PrintColumn(grid.ColumnLessThanOrEqual(index, datum));
      return;
    } else if (option == "cgt") {
      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "Values greater than given value:" << std::endl;
      PrintColumn(grid.ColumnGreaterThan(index, datum));
      return;
    } else if (option == "cgte") {
      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "Values greater than or equal to given value:" << std::endl;
      PrintColumn(grid.ColumnGreaterThanOrEqual(index, datum));
      return;
    } else if (option == "ce") {
      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "Values equal to given value:" << std::endl;
      PrintColumn(grid.ColumnEqual(index, datum));
      return;
    } else if (option == "cne") {
      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "Values not equal to given value:" << std::endl;
      PrintColumn(grid.ColumnNotEqual(index, datum));
      return;
    } else if (option == "b") {
      return;
    } else {
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * @brief Displays the CSV grid manipulation menu.
 * @param grid The DataGrid to manipulate.
 */
void ManipulateGridMenu(cse::DataGrid &grid) {
  int choice = 0;
  do {
    std::cout << "\n=== CSV Grid Manipulation Menu ===" << std::endl;
    std::cout << "1. Print options" << std::endl;
    std::cout << "2. Edit options" << std::endl;
    std::cout << "3. Sort options" << std::endl;
    std::cout << "4. Adding options" << std::endl;
    std::cout << "5. Deleting options" << std::endl;
    std::cout << "6. Resizing options" << std::endl;
    std::cout << "0. Return to main menu" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
      case 1:
        PrintSubmenu(grid);
        break;
      case 2:
        EditSubmenu(grid);
        break;
      case 3:
        SortSubmenu(grid);
        break;
      case 4:
        AddSubmenu(grid);
        break;
      case 5:
        DeleteSubmenu(grid);
        break;
      case 6:
        ResizeSubmenu(grid);
        break;
      case 0:
        break;
      default:
        std::cout << "Invalid choice. Try again." << std::endl;
    }
  } while (choice != 0);
}

/**
 * @brief Displays the print submenu.
 * @param grid The DataGrid to print.
 */
void PrintSubmenu(cse::DataGrid &grid) {
  int choice = 0;
  do {
    std::cout << "\n--- Print Options ---" << std::endl;
    std::cout << "1. Print a cell value" << std::endl;
    std::cout << "2. Print a row" << std::endl;
    std::cout << "3. Print a column" << std::endl;
    std::cout << "4. Print entire grid" << std::endl;
    std::cout << "0. Return to previous menu" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    try {
      switch (choice) {
        case 1: {
          std::size_t row = 0, col = 0;
          std::cout << "Enter row index: ";
          std::cin >> row;
          std::cout << "Enter column index: ";
          std::cin >> col;
          cse::Datum value = grid.GetValue(row, col);
          if (value.IsDouble()) {
            std::cout << "Cell (" << row << ", " << col << "): " << value.GetDouble() << std::endl;
          } else if (value.IsString()) {
            std::cout << "Cell (" << row << ", " << col << "): " << value.GetString() << std::endl;
          }
          break;
        }
        case 2: {
          std::size_t row = 0;
          std::cout << "Enter row index: ";
          std::cin >> row;
          auto row_data = grid.GetRow(row);
          std::cout << "Row " << row << ": ";
          for (const auto &datum : row_data) {
            if (datum.IsDouble()) {
              std::cout << datum.GetDouble() << " ";
            } else if (datum.IsString()) {
              std::cout << datum.GetString() << " ";
            }
          }
          std::cout << std::endl;
          break;
        }
        case 3: {
          std::size_t col = 0;
          std::cout << "Enter column index: ";
          std::cin >> col;
          auto col_data = grid.GetColumn(col);
          std::cout << "Column " << col << ": ";
          for (const auto &datum : col_data) {
            if (datum.IsDouble()) {
              std::cout << datum.GetDouble() << " ";
            } else if (datum.IsString()) {
              std::cout << datum.GetString() << " ";
            }
          }
          std::cout << std::endl;
          break;
        }
        case 4:
          std::cout << grid;
          break;
        case 0:
          break;
        default:
          std::cout << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * @brief Displays the edit submenu.
 * @param grid The DataGrid to edit.
 */
void EditSubmenu(cse::DataGrid &grid) {
  int choice = 0;
  do {
    std::cout << "\n--- Edit Options ---" << std::endl;
    std::cout << "1. Edit a cell value" << std::endl;
    std::cout << "2. Edit an entire row's values" << std::endl;
    std::cout << "3. Edit an entire column's values" << std::endl;
    std::cout << "0. Return to previous menu" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    try {
      switch (choice) {
        case 1: {
          std::size_t row = 0, col = 0;
          std::cout << "Enter row index: ";
          std::cin >> row;
          std::cout << "Enter column index: ";
          std::cin >> col;
          std::cout << "Enter new value (number): ";
          double new_val = 0;
          std::cin >> new_val;
          grid.At(row, col) = cse::Datum(new_val);
          std::cout << "Cell updated." << std::endl;
          break;
        }
        case 2: {
          std::size_t row = 0;
          std::cout << "Enter row index to update: ";
          std::cin >> row;
          auto &row_data = grid.GetRow(row);
          std::cout << "Enter " << row_data.size() << " new values (numbers): ";
          for (auto &datum : row_data) {
            double val = 0;
            std::cin >> val;
            datum = cse::Datum(val);
          }
          std::cout << "Row updated." << std::endl;
          break;
        }
        case 3: {
          std::size_t col = 0;
          std::cout << "Enter column index to update: ";
          std::cin >> col;
          std::size_t num_rows = std::get<0>(grid.Shape());
          std::cout << "Enter " << num_rows << " new values (numbers): ";
          for (std::size_t i = 0; i < num_rows; ++i) {
            double val = 0;
            std::cin >> val;
            grid.At(i, col) = cse::Datum(val);
          }
          std::cout << "Column updated." << std::endl;
          break;
        }
        case 0:
          break;
        default:
          std::cout << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * @brief Displays the sort submenu.
 * @param grid The DataGrid to sort.
 */
void SortSubmenu(cse::DataGrid &grid) {
  int choice = 0;
  do {
    std::cout << "\n--- Sort Options ---" << std::endl;
    std::cout << "1. Sort grid rows by a specified column" << std::endl;
    std::cout << "2. Sort entire grid (using left-most columns as keys)" << std::endl;
    std::cout << "0. Return to previous menu" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    try {
      switch (choice) {
        case 1: {
          std::size_t col = 0;
          std::cout << "Enter column index to sort by: ";
          std::cin >> col;
          int order = 0;
          std::cout << "Enter 1 for ascending, 0 for descending: ";
          std::cin >> order;
          grid.SortColumn(col, order == 1);
          std::cout << "Grid rows sorted by column " << col << "." << std::endl;
          break;
        }
        case 2: {
          int order = 0;
          std::cout << "Enter 1 for ascending, 0 for descending: ";
          std::cin >> order;
          grid.Sort(order == 1);
          std::cout << "Entire grid sorted." << std::endl;
          break;
        }
        case 0:
          break;
        default:
          std::cout << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * @brief Displays the adding submenu.
 * @param grid The DataGrid to add rows/columns to.
 */
void AddSubmenu(cse::DataGrid &grid) {
  int choice = 0;
  do {
    std::cout << "\n--- Adding Options ---" << std::endl;
    std::cout << "1. Add row" << std::endl;
    std::cout << "2. Add column" << std::endl;
    std::cout << "3. Merge another grid" << std::endl;
    std::cout << "0. Return to previous menu" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    try {
      switch (choice) {
        case 1: {
          std::size_t num_cols = std::get<1>(grid.Shape());
          std::vector<cse::Datum> new_row;
          std::cout << "Enter " << num_cols << " values for the new row: ";
          for (std::size_t i = 0; i < num_cols; ++i) {
            double val = 0;
            std::cin >> val;
            new_row.push_back(cse::Datum(val));
          }
          grid.InsertRow(new_row);
          std::cout << "Row added." << std::endl;
          break;
        }
        case 2: {
          std::size_t num_rows = std::get<0>(grid.Shape());
          std::vector<cse::Datum> new_column;
          std::cout << "Enter " << num_rows << " values for the new column: ";
          for (std::size_t i = 0; i < num_rows; ++i) {
            double val = 0;
            std::cin >> val;
            new_column.push_back(cse::Datum(val));
          }
          grid.InsertColumn(new_column);
          std::cout << "Column added." << std::endl;
          break;
        }
        case 3: {
          int merge_type = 0;
          std::cout << "Enter merge type (1 for row append, 0 for column append): ";
          std::cin >> merge_type;
          std::size_t merge_rows = 0, merge_cols = 0;
          std::cout << "Enter number of rows for merging grid: ";
          std::cin >> merge_rows;
          std::cout << "Enter number of columns for merging grid: ";
          std::cin >> merge_cols;
          std::vector<std::vector<cse::Datum>> merge_data(merge_rows,
                                                          std::vector<cse::Datum>(merge_cols));
          std::cout << "Enter values for the merging grid:" << std::endl;
          for (std::size_t i = 0; i < merge_rows; ++i) {
            for (std::size_t j = 0; j < merge_cols; ++j) {
              double val = 0;
              std::cin >> val;
              merge_data[i][j] = cse::Datum(val);
            }
          }
          cse::DataGrid other_grid(merge_data);
          grid = grid.Merge(other_grid, merge_type == 1);
          std::cout << "Grids merged." << std::endl;
          break;
        }
        case 0:
          break;
        default:
          std::cout << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * @brief Displays the deleting submenu.
 * @param grid The DataGrid to delete from.
 */
void DeleteSubmenu(cse::DataGrid &grid) {
  int choice = 0;
  do {
    std::cout << "\n--- Deleting Options ---" << std::endl;
    std::cout << "1. Delete a row" << std::endl;
    std::cout << "2. Delete a column" << std::endl;
    std::cout << "3. Clear the entire grid" << std::endl;
    std::cout << "0. Return to previous menu" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    try {
      switch (choice) {
        case 1: {
          std::size_t row = 0;
          std::cout << "Enter row index to delete: ";
          std::cin >> row;
          grid.DeleteRow(row);
          std::cout << "Row deleted." << std::endl;
          break;
        }
        case 2: {
          std::size_t col = 0;
          std::cout << "Enter column index to delete: ";
          std::cin >> col;
          grid.DeleteColumn(col);
          std::cout << "Column deleted." << std::endl;
          break;
        }
        case 3: {
          grid.Clear();
          std::cout << "Grid cleared." << std::endl;
          break;
        }
        case 0:
          break;
        default:
          std::cout << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * @brief Displays the resizing submenu.
 * @param grid The DataGrid to resize.
 */
void ResizeSubmenu(cse::DataGrid &grid) {
  int choice = 0;
  do {
    std::cout << "\n--- Resizing Options ---" << std::endl;
    std::cout << "1. Resize grid (default type: double)" << std::endl;
    std::cout << "2. Resize grid (default type: string)" << std::endl;
    std::cout << "0. Return to previous menu" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    try {
      switch (choice) {
        case 1: {
          std::size_t new_rows = 0, new_cols = 0;
          double default_value = 0;
          std::cout << "Enter new number of rows: ";
          std::cin >> new_rows;
          std::cout << "Enter new number of columns: ";
          std::cin >> new_cols;
          std::cout << "Enter default value (number): ";
          std::cin >> default_value;
          grid.Resize(new_rows, new_cols, default_value);
          std::cout << "Grid resized." << std::endl;
          break;
        }
        case 2: {
          std::size_t new_rows = 0, new_cols = 0;
          std::string default_value;
          std::cout << "Enter new number of rows: ";
          std::cin >> new_rows;
          std::cout << "Enter new number of columns: ";
          std::cin >> new_cols;
          std::cout << "Enter default value (string): ";
          std::cin >> default_value;
          grid.Resize(new_rows, new_cols, default_value);
          std::cout << "Grid resized." << std::endl;
          break;
        }
        case 0:
          break;
        default:
          std::cout << "Invalid choice. Try again." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  } while (choice != 0);
}

/**
 * @brief Main entry point.
 */
int main() {
  std::cout << "Welcome to CSV Command Line Manipulator" << std::endl;
  std::cout << "Developed by: Max Krawec, Calen Green, Pedro Mitkiewicz, "
               "Shahaab Ali, and Muhammad Asif Masood"
            << std::endl;
  std::cout << std::endl;

  cse::DataGrid grid = GridMenu();

  while (true) {
    std::cout << "\nMenu Option:" << std::endl;
    std::cout << "e: Edit CSV Grid" << std::endl;
    std::cout << "m: Math Operations" << std::endl;
    std::cout << "c: Comparisons (WIP)" << std::endl;
    std::cout << "q: Quit" << std::endl;
    std::cout << "\nPlease enter an option from the list above: ";
    std::string option;
    std::cin >> option;

    if (option == "e") {
      ManipulateGridMenu(grid);
    } else if (option == "m") {
      MathMenu(grid);
    } else if (option == "c") {
      ComparisonMenu(grid);
    } else if (option == "q") {
      break;
    } else {
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }

  std::cout << std::endl << "Thank you" << std::endl;
  return 0;
}