/**
 * @file main.cpp
 *
 * @author Max Krawec
 */

#include "src/Datum.h"
#include "src/DataGrid.h"

#include <iostream>

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
  bool show_csv_menu = true;
  while (show_csv_menu) {
    std::cout << "Please enter an option from the list below" << std::endl;

    std::cout << "i: Import a CSV file (TODO)" << std::endl;
    std::cout << "e: Export to a CSV file (TODO)" << std::endl;
    std::cout << "** t: Creates a fake grid used for testing - will remove later**" << std::endl;
    std::cout << "c: Create new DataGrid" << std::endl;

    std::string option;
    std::cin >> option;

    if (option == "i") {
      // TODO
      show_csv_menu = false;
    } else if (option == "e") {
      // TODO
      show_csv_menu = false;
    } else if (option == "t") {
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

      math_vector[0][2] = cse::Datum(10.25);
      math_vector[1][2] = cse::Datum("test3");
      math_vector[2][2] = cse::Datum(150.50);
      math_vector[3][2] = cse::Datum(200);
      math_vector[4][1] = cse::Datum("test6");
      math_vector[4][2] = cse::Datum(20.25);

      cse::DataGrid grid(math_vector);
      return grid;
    } else if (option == "c") {
      return CreateGrid();
    } else {
      std::cout << "Invalid option. Try again." << std::endl;
    }
  }
  return cse::DataGrid(); // TODO - This should never be called. Maybe do something else here?
}

/**
 * Outputs options for the math menu
 */
void MathMenu(const cse::DataGrid& grid) {
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

  // Used ChatGPT to help write a lambda to simplify the code
  auto GetColumnIndex = []() {
    std::cout << "Please enter column index: ";
    int index;
    std::cin >> index;
    return index;
  };

  if (option == "cmean") {
    int value = GetColumnIndex();
    std::cout << "The mean at column " << value << " is: " << grid.ColumnMean(value) << std::endl;
  } else if (option == "cmed") {
    int value = GetColumnIndex();
    std::cout << "The median at column " << value << " is: " << grid.ColumnMedian(value) << std::endl;
  } else if (option == "csd") {
    int value = GetColumnIndex();
    std::cout << "The standard deviation at column " << value << " is: " << grid.ColumnStandardDeviation(value) << std::endl;
  } else if (option == "cmin") {
    int value = GetColumnIndex();
    std::cout << "The min at column " << value << " is: " << grid.ColumnMin(value) << std::endl;
  } else if (option == "cmax") {
    int value = GetColumnIndex();
    std::cout << "The max at column " << value << " is: " << grid.ColumnMax(value) << std::endl;
  } else if (option == "cmode") {
    int value = GetColumnIndex();
    std::cout << "The mode(s) at column " << value << " is/are: ";
    for (const double& mode_value : grid.ColumnMode(value)) {
      std::cout << mode_value << ' ';
    }
    std::cout << "" << std::endl;
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
  } else if (option != "b") {
    std::cout << "Invalid option. Try again." << std::endl;
  }
  std::cout << "" << std::endl;
}

// TODO - WIP (const) - Max K.
void ComparisonMenu(cse::DataGrid& grid) {
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

  auto GetCompareValue = []() {
    std::cout << "Do you want to compare a number (double) or text (string): " << std::endl;
    std::cout << "n: double (number) " << std::endl;
    std::cout << "s: string (text) " << std::endl;
    std::string datum_type_value;
    std::cin >> datum_type_value;

    while (true) {
      std::string value;
      if (datum_type_value == "n") {
        std::cout << "Please enter the value to compare: ";
        std::cin >> value;
        return cse::Datum(value);
      } else if (datum_type_value == "t") {
        std::cout << "Please enter the value to compare: ";
        std::cin >> value;
        return cse::Datum(value);
      } else {
        std::cout << "Invalid option. Try again." << std::endl;
      }
    }
  };

  auto GetColumnIndex = []() {
    std::cout << "Please enter column index: ";
    int index;
    std::cin >> index;
    return index;
  };
}

int main() {
  std::cout << "Welcome to CSV Command Line Manipulator" << std::endl; // TODO - Determine name
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
    std::cout << "comparisons (WIP)" << std::endl;
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
