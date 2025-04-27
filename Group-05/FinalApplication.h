/**
 * @file FinalApplication.h
 *
 * @author Max Krawec, Shahaab Ali
 */

#pragma once

#include "src/DataGrid.h"

/**
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
 */
class FinalApplication {
 private:
  [[nodiscard]] static std::optional<double> IsValidDouble(
      const std::string &input);
  [[nodiscard]] static std::optional<int> IsValidInt(const std::string &input);
  [[nodiscard]] static bool IsValidCustomEquation(const std::string& input, int max_number_value);

  [[nodiscard]] static int GetColumnIndex(int grid_size, std::ostream &os, std::istream &is);
  [[nodiscard]] static cse::Datum GetDataValue(std::ostream &os, std::istream &is);

  static void PrintColumn(const cse::ReferenceVector<cse::Datum> &column,
                          std::ostream &os);

 public:
  static void MainMenu(std::ostream &os, std::istream &is);

  static cse::DataGrid GridMenu(std::ostream &os, std::istream &is);
  static cse::DataGrid CreateGridMenu(std::ostream &os, std::istream &is);

  static void ManipulateGridMenu(cse::DataGrid &grid, std::ostream &os,
                                 std::istream &is);
  static void MathMenu(const cse::DataGrid &grid, std::ostream &os,
                       std::istream &is);
  static void ComparisonMenu(cse::DataGrid &grid, std::ostream &os,
                             std::istream &is);

  static void PrintSubmenu(const cse::DataGrid &grid, std::ostream &os,
                           std::istream &is);
  static void EditSubmenu(cse::DataGrid &grid, std::ostream &os,
                          std::istream &is);
  static void SortSubmenu(cse::DataGrid &grid, std::ostream &os,
                          std::istream &is);
  static void AddSubmenu(cse::DataGrid &grid, std::ostream &os,
                         std::istream &is);
  static void DeleteSubmenu(cse::DataGrid &grid, std::ostream &os,
                            std::istream &is);
  static void ResizeSubmenu(cse::DataGrid &grid, std::ostream &os,
                            std::istream &is);
};
