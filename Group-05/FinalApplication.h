/**
 * @file FinalApplication.h
 *
 * @author Max Krawec
 */

#pragma once

#include "src/DataGrid.h"

/**
 * This application allows users to create and manipulate data within a grid through the command line.
 * Users can create the grid by using a .csv file, inputting a custom grid, or selecting a pre-made grid.
 *
 * Users can manipulate the grid in several ways:
 * - Edit specific cell values, rows, or columns
 * - Add default, manual, or custom rows/columns
 * - Delete specific rows/columns
 * - Sort the grid
 * - Resize the grid
 * - Print the grid
 * - Calculate the mean, median, standard deviation, mode, min, and max of columns or the entire grid
 * - Use comparison operations (less than, less than or equal to, greater than, greater than or equal to,
 *   equal to, and not equal to) to compare a given value against values in a column
 *
 * When done, users can export the grid into a .csv file.
 */
class FinalApplication {
 private:
  [[nodiscard]] std::optional<double> IsValidDouble(const std::string &input) const;
  [[nodiscard]] std::optional<int> IsValidInt(const std::string &input) const;

  int GetColumnIndex(int grid_size, std::ostream &os, std::istream &is) const;
  cse::Datum GetDataValue(std::ostream &os, std::istream &is) const;

  void PrintColumn(const cse::ReferenceVector<cse::Datum> &column, std::ostream &os) const;
 public:
  void MainMenu(std::ostream &os, std::istream &is) const;

  cse::DataGrid GridMenu(std::ostream &os, std::istream &is) const;
  cse::DataGrid CreateGridMenu(std::ostream &os, std::istream &is) const;

  void ManipulateGridMenu(cse::DataGrid &grid, std::ostream &os, std::istream &is) const;
  void MathMenu(const cse::DataGrid &grid, std::ostream &os, std::istream &is) const;
  void ComparisonMenu(cse::DataGrid &grid, std::ostream &os, std::istream &is) const;

  void PrintSubmenu(const cse::DataGrid &grid, std::ostream &os, std::istream &is) const;
  void EditSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is) const;
  void SortSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is) const;
  void AddSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is) const;
  void DeleteSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is) const;
  void ResizeSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is) const;
};

