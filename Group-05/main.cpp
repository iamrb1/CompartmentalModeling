/**
 * @file main.cpp
 *
 * @author Max Krawec
 */

#include "src/Datum.h"
#include "src/DataGrid.h"

#include <iostream>

/**
 * Helper function that determines if the user input is a double
 * @param string_value The user input
 * @return Double optional if the input is a double, otherwise an empty optional
 */
std::optional<double> IsValidDouble(const std::string &string_value) {
  // CITE: Used ChatGPT to write this code. Fixed a bug where strings starting with
  // numbers (e.g., "123Hello") were partially converted instead of being a string.
  // Also used in the IsValidInt() function
  try {
    std::size_t pos;
    double double_value = std::stod(string_value, &pos);
    if (pos == string_value.length()) {
      return double_value;
    }
  } catch(std::invalid_argument &e) {}
  return {};
}


/**
 * Helper function that determines if the user input is an int
 * @param string_value The user input
 * @return Int optional if the input is a int, otherwise an empty optional
 */
std::optional<int> IsValidInt(const std::string &string_value) {
  try {
    std::size_t pos;
    int int_value = std::stoi(string_value, &pos);
    if (pos == string_value.length()) {
      return int_value;
    }
  } catch(std::invalid_argument &e) {}
  return {};
}


/**
 * Helper function that gets the column index from the user
 * @return The column index the user specified
 */
int GetColumnIndex() {
  while (true) {
    std::cout << "Please enter column index: ";
    std::string index_string;
    std::cin >> index_string;

    std::optional<int> int_value = IsValidInt(index_string);
    if (int_value.has_value()) {
      return int_value.value();
    }
    else {
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * Helper function that gets the data (Datum) value from the user
 * @return The data (Datum) value the user specified
 */
cse::Datum GetDataValue() {
  std::cout << "Please enter the value to compare: ";
  std::string compare_value;
  std::cin >> compare_value;

  std::optional<double> double_value = IsValidDouble(compare_value);
  if (double_value.has_value()) {
    return {double_value.value()};
  }

  return {compare_value};
}

/**
 * Helper function that prints a column
 * @param column_reference_vector The column
 */
void PrintColumn(const cse::ReferenceVector<cse::Datum> &column) {
  for (const cse::Datum &value : column) {
    if (value.IsDouble()) {
      std::cout << value.GetDouble() << " ";
    } else {
      std::cout << value.GetString() << " ";
    }
  }
  std::cout << "" << std::endl;
}

/**
 * Outputs options for the grid menu
 */
cse::DataGrid CreateGrid() {
  std::size_t number_of_columns = 0;
  std::size_t number_of_rows = 0;
  std::string default_numbers;

  // Prompt for number of columns
  while (true) {
    std::cout << "Enter number of rows for your DataGrid: ";
    std::cin >> number_of_rows;

    if (std::cin.fail() || number_of_rows <= 0) {
      std::cin.clear(); // clear error state
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
      std::cout << "Invalid input. Please enter a positive integer.\n";
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear rest of line
      break;
    }
  }

    // Prompt for number of rows
  while (true) {
    std::cout << "Enter number of columns for your DataGrid: ";
    std::cin >> number_of_columns;

    if (std::cin.fail() || number_of_columns <= 0) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a positive integer.\n";
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    }
  }

  // Ask if default values are numeric or strings
  while (true) {
    std::cout << "Would you like your default values to be numerical or strings? [n/s]: ";
    std::getline(std::cin >> std::ws, default_numbers);

    if (default_numbers == "n") {
      double default_value;

      while (true) {
        std::cout << "Enter a numeric default value for the DataGrid: ";
        std::cin >> default_value;

        if (std::cin.fail()) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cout << "Invalid input. Please enter a valid number.\n";
        } else {
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          return cse::DataGrid(number_of_rows, number_of_columns, default_value);
        }
      }

    } if (default_numbers == "s") {
      std::string default_value;
      std::cout << "Enter a string default value for the DataGrid: ";
      std::getline(std::cin >> std::ws, default_value);
      return { number_of_rows, number_of_columns, default_value };

    } else {
      std::cout << "Invalid selection. Type 'n' for numeric or 's' for string.\n";
    }
  }
}

cse::DataGrid GridMenu() {
  while (true) {
    std::cout << "Please enter an option from the list below" << std::endl;

    std::cout << "i: Import a CSV file (TODO)" << std::endl;
    std::cout << "e: Export to a CSV file (TODO)" << std::endl;
    std::cout << "** t: Creates a fake grid used for testing - will remove later**" << std::endl;
    std::cout << "c: Create new DataGrid" << std::endl;

    std::string option;
    std::cin >> option;

    if (option == "i") {
      // TODO
      break;
    } else if (option == "e") {
      // TODO
      break;
    }
    // TODO - remove
    else if (option == "t") {
      std::vector<std::vector<cse::Datum>> math_vector(5, std::vector<cse::Datum>(3));

      math_vector[0][0] = cse::Datum(5.0);
      math_vector[1][0] = cse::Datum(3.5);
      math_vector[2][0] = cse::Datum(1.25);
      math_vector[3][0] = cse::Datum(-15);
      math_vector[4][0] = cse::Datum(4.25);

      math_vector[0][1] = cse::Datum("test1");
      math_vector[1][1] = cse::Datum("test2");
      math_vector[2][1] = cse::Datum("test4");
      math_vector[3][1] = cse::Datum("test5");
      math_vector[4][1] = cse::Datum("test6");

      math_vector[0][2] = cse::Datum(10.25);
      math_vector[1][2] = cse::Datum("test3");
      math_vector[2][2] = cse::Datum(150.50);
      math_vector[3][2] = cse::Datum(200);
      math_vector[4][2] = cse::Datum(20.25);

      cse::DataGrid grid(math_vector);
      return grid;
    } else if (option == "c") {
      return CreateGrid();
    } else {
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }

  // CITE: Used ChatGPT for this idea.
  // TODO - Should be able to remove after people add their code
  throw std::runtime_error("Unexpected exit from menu loop");
}

/**
 * Outputs the options and results for the math menu
 */
void MathMenu(const cse::DataGrid& grid) {
  while(true) {
    std::cout << "Please enter a math option from the list below:" << std::endl;
    std::cout << "cmean: Calculate the mean of a column" << std::endl;
    std::cout << "cmed: Calculate the median of a column" << std::endl;
    std::cout << "cmode: Calculate the mode of a column" << std::endl;
    std::cout << "csd: Calculate the standard deviation of a column" << std::endl;
    std::cout << "cmin: Calculate the min of a column" << std::endl;
    std::cout << "cmax: Calculate the max of a column" << std::endl;
    std::cout << "sum: Calculate the mean, median, mode, standard deviation, min, and max of the data grid" << std::endl;
    std::cout << "b: Go back" << std::endl;

    std::string option;
    std::cin >> option;

    if (option == "cmean") {
      int index = GetColumnIndex();
      std::cout << "The mean at column " << index << " is: " << grid.ColumnMean(index) << std::endl;
      return;
    } else if (option == "cmed") {
      int index = GetColumnIndex();
      std::cout << "The median at column " << index << " is: " << grid.ColumnMedian(index) << std::endl;
      return;
    } else if (option == "csd") {
      int index = GetColumnIndex();
      std::cout << "The standard deviation at column " << index << " is: " << grid.ColumnStandardDeviation(index) << std::endl;
      return;
    } else if (option == "cmin") {
      int index = GetColumnIndex();
      std::cout << "The min at column " << index << " is: " << grid.ColumnMin(index) << std::endl;
      return;
    } else if (option == "cmax") {
      int index = GetColumnIndex();
      std::cout << "The max at column " << index << " is: " << grid.ColumnMax(index) << std::endl;
      return;
    } else if (option == "cmode") {
      int index = GetColumnIndex();
      std::cout << "The mode(s) at column " << index << " is/are: ";
      for (const double& mode_value : grid.ColumnMode(index)) {
        std::cout << mode_value << ' ';
      }
      std::cout << "" << std::endl;
      return;
    } else if (option == "sum") {
      auto summary = grid.CalculateDataGridMathSummary();
      std::cout << "The summary of the grid is:" << std::endl;
      std::cout << "Mean " << summary.mean << std::endl;
      std::cout << "Median " << summary.median << std::endl;
      std::cout << "Standard Deviation " << summary.standardDeviation << std::endl;
      std::cout << "Min " << summary.min << std::endl;
      std::cout << "Max " << summary.max << std::endl;

      std::cout << "Mode(s) ";
      for (const double& mode_value : summary.mode) {
        std::cout << mode_value << ' ';
      }
      std::cout << "" << std::endl;
      return;
    } else if (option == "b") {
      return;
    } else {
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }
}

/**
 * Outputs the options and results for the comparison menu
 * @param grid The DataGrid
 */
void ComparisonMenu(cse::DataGrid& grid) {
  while(true) {
    std::cout << "Please enter a comparison option from the list below:" << std::endl;
    std::cout << "clt: Find values less than a given value for a column" << std::endl;
    std::cout << "clte: Find values less than or equal to a given value for a column" << std::endl;
    std::cout << "cgt: Find values greater than a given value for a column" << std::endl;
    std::cout << "cgte: Find values greater than or equal to a given value for a column" << std::endl;
    std::cout << "ce: Find values equal to a value for a column" << std::endl;
    std::cout << "cne: Find values not equal to a given value for a column" << std::endl;
    std::cout << "b: Go back" << std::endl;

    std::string option;
    std::cin >> option;

    if (option=="clt") {
      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "The less than values are: " << std::endl;
      PrintColumn(grid.ColumnLessThan(index, datum));
      return;
    } else if (option=="clte") {

      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "The less than or equal values are: " << std::endl;
      PrintColumn(grid.ColumnLessThanOrEqual(index, datum));
      return;
    } else if (option=="cgt") {

      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "The greater than values are: " << std::endl;
      PrintColumn(grid.ColumnGreaterThan(index, datum));
      return;
    } else if (option=="cgte") {

      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "The greater than or equal values are: " << std::endl;
      PrintColumn(grid.ColumnGreaterThanOrEqual(index, datum));
      return;
    } else if (option=="ce") {

      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "The values that are equal are: " << std::endl;
      PrintColumn(grid.ColumnEqual(index, datum));
      return;
    } else if (option=="cne") {

      int index = GetColumnIndex();
      cse::Datum datum = GetDataValue();
      std::cout << "The values that are not equal are: " << std::endl;
      PrintColumn(grid.ColumnNotEqual(index, datum));
      return;
    } else if (option == "b") {
      return;
    } else {
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }
}


int main() {
  std::cout << "Welcome to CSV Command Line Manipulator" << std::endl;
  std::cout << "Developed by: Max Krawec, Calen Green, Pedro Mitkiewicz, Shahaab Ali, and Muhammad Asif Masood" << std::endl;
  std::cout << "" << std::endl;

  cse::DataGrid grid = GridMenu();

  while (true) {
    std::cout << "Please enter an option from the list below:" << std::endl;
    std::cout << "remove (TODO)" << std::endl;
    std::cout << "insert (TODO)" << std::endl;
    std::cout << "sort (TODO)" << std::endl;
    std::cout << "extra (TODO)" << std::endl;
    std::cout << "math" << std::endl;
    std::cout << "comparisons" << std::endl;
    std::cout << "print (TODO)" << std::endl;
    std::cout << "quit" << std::endl;

    std::string option;
    std::cin >> option;

    // TODO - Add more options here
    if (option == "comparisons") {
      ComparisonMenu(grid);
    } else if (option == "math") {
      MathMenu(grid);
    } else if (option == "quit") {
      break;
    } else {
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }

  std::cout << "" << std::endl;
  std::cout << "Thank you" << std::endl;

  return 0;
}
