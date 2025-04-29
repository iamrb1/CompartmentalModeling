/**
 * @file FinalApplication.h
 *
 * @author Max Krawec, Shahaab Ali
 */

#pragma once

#include <iostream>

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
  // Common helper functions
  [[nodiscard]] static std::optional<double> IsValidDouble(
      const std::string &input) noexcept;
  [[nodiscard]] static std::optional<int> IsValidInt(const std::string &input) noexcept;
  [[nodiscard]] static bool IsValidCustomEquation(const std::string &input,
                                                  int max_number_value) noexcept;

  [[nodiscard]] static int GetIndex(int grid_size, std::ostream &os = std::cout,
                                    std::istream &is = std::cin,
                                    const std::string &label = "column") noexcept;
  [[nodiscard]] static cse::Datum GetDataValue(std::ostream &os = std::cout,
                                               std::istream &is = std::cin) noexcept;

  static void PrintColumn(const cse::ReferenceVector<cse::Datum> &column,
                          std::ostream &os) noexcept;


  // MathMenu helper functions
  void static MathMenuMean(const cse::DataGrid &grid,
                           std::ostream &os = std::cout,
                           std::istream &is = std::cin) noexcept;
  void static MathMenuMedian(const cse::DataGrid &grid,
                             std::ostream &os = std::cout,
                             std::istream &is = std::cin) noexcept;
  void static MathMenuStandardDeviation(const cse::DataGrid &grid,
                                        std::ostream &os = std::cout,
                                        std::istream &is = std::cin) noexcept;
  void static MathMenuMin(const cse::DataGrid &grid,
                          std::ostream &os = std::cout,
                          std::istream &is = std::cin) noexcept;
  void static MathMenuMax(const cse::DataGrid &grid,
                          std::ostream &os = std::cout,
                          std::istream &is = std::cin) noexcept;
  void static MathMenuMode(const cse::DataGrid &grid,
                           std::ostream &os = std::cout,
                           std::istream &is = std::cin) noexcept;
  void static MathMenuSummary(const cse::DataGrid &grid,
                              std::ostream &os = std::cout) noexcept;

  // ComparisonMenu helper functions
  void static ComparisonMenuLessThan(cse::DataGrid &grid,
                                     std::ostream &os = std::cout,
                                     std::istream &is = std::cin) noexcept;
  void static ComparisonMenuLessThanEqual(cse::DataGrid &grid,
                                          std::ostream &os = std::cout,
                                          std::istream &is = std::cin) noexcept;
  void static ComparisonMenuGreaterThan(cse::DataGrid &grid,
                                        std::ostream &os = std::cout,
                                        std::istream &is = std::cin) noexcept;
  void static ComparisonMenuGreaterThanEqual(cse::DataGrid &grid,
                                             std::ostream &os = std::cout,
                                             std::istream &is = std::cin) noexcept;
  void static ComparisonMenuEqual(cse::DataGrid &grid,
                                  std::ostream &os = std::cout,
                                  std::istream &is = std::cin) noexcept;
  void static ComparisonMenuNotEqual(cse::DataGrid &grid,
                                     std::ostream &os = std::cout,
                                     std::istream &is = std::cin) noexcept;

  // CreateGridMenu functions
  int static CreateGridMenuGetValue(const std::string &type,
                                    std::ostream &os = std::cout,
                                    std::istream &is = std::cin) noexcept;

  // GridMenu helper functions
  std::optional<cse::DataGrid> static GridMenuImport(
      std::ostream &os = std::cout, std::istream &is = std::cin);
  cse::DataGrid static GridMenuPreMadeGrid(std::ostream &os = std::cout) noexcept;

  // PrintMenu helper functions
  void static PrintSubmenuCell(const cse::DataGrid &grid,
                               std::ostream &os = std::cout,
                               std::istream &is = std::cin) noexcept;
  void static PrintSubmenuRow(const cse::DataGrid &grid,
                              std::ostream &os = std::cout,
                              std::istream &is = std::cin) noexcept;
  void static PrintSubmenuColumn(const cse::DataGrid &grid,
                                 std::ostream &os = std::cout,
                                 std::istream &is = std::cin) noexcept;
  std::optional<int> static PrintAndEditSubmenuGetIndex(
      int max_index, std::ostream &os = std::cout, std::istream &is = std::cin,
      const std::string &type = "column") noexcept;

  // EditSubMenu helper functions
  void static EditSubmenuCell(cse::DataGrid &grid, std::ostream &os = std::cout,
                              std::istream &is = std::cin) noexcept;
  void static EditSubmenuRow(cse::DataGrid &grid, std::ostream &os = std::cout,
                             std::istream &is = std::cin) noexcept;
  void static EditSubmenuColumn(cse::DataGrid &grid,
                                std::ostream &os = std::cout,
                                std::istream &is = std::cin) noexcept;

  // DeleteSubmenu helper functions
  void static DeleteSubmenuRow(cse::DataGrid &grid,
                               std::ostream &os = std::cout,
                               std::istream &is = std::cin) noexcept;
  void static DeleteSubmenuColumn(cse::DataGrid &grid,
                                  std::ostream &os = std::cout,
                                  std::istream &is = std::cin) noexcept;
  void static DeleteSubmenuGrid(cse::DataGrid &grid,
                                std::ostream &os = std::cout) noexcept;

  // SortSubMenu helper function
  void static SortSubmenuColumnOrGrid(cse::DataGrid &grid,
                                      const bool &is_column_sort,
                                      std::ostream &os = std::cout,
                                      std::istream &is = std::cin) noexcept;

  // ResizeSubmenu helper functions
  void static ResizeSubmenuResizeGrid(cse::DataGrid &grid,
                                      std::ostream &os = std::cout,
                                      std::istream &is = std::cin) noexcept;
  int static ResizeSubmenuGetValidInput(const std::string &type,
                                        std::ostream &os = std::cout,
                                        std::istream &is = std::cin) noexcept;

  // MainMenu helper function
  void static MainMenuExport(const cse::DataGrid &grid,
                             std::ostream &os = std::cout,
                             std::istream &is = std::cin);

 public:
  static void MainMenu(std::ostream &o = std::cout,
                       std::istream &is = std::cin) noexcept;

  static cse::DataGrid GridMenu(std::ostream &os = std::cout,
                                std::istream &is = std::cin) noexcept;
  static cse::DataGrid CreateGridMenu(std::ostream &os = std::cout,
                                      std::istream &is = std::cin) noexcept;

  static void ManipulateGridMenu(cse::DataGrid &grid,
                                 std::ostream &os = std::cout,
                                 std::istream &is = std::cin) noexcept;
  static void MathMenu(const cse::DataGrid &grid, std::ostream &os = std::cout,
                       std::istream &is = std::cin) noexcept;
  static void ComparisonMenu(cse::DataGrid &grid, std::ostream &os = std::cout,
                             std::istream &is = std::cin) noexcept;

  static void PrintSubmenu(const cse::DataGrid &grid,
                           std::ostream &os = std::cout,
                           std::istream &is = std::cin) noexcept;
  static void EditSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout,
                          std::istream &is = std::cin) noexcept;
  static void SortSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout,
                          std::istream &is = std::cin) noexcept;
  static void AddSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout,
                         std::istream &is = std::cin);
  static void DeleteSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout,
                            std::istream &is = std::cin);
  static void ResizeSubmenu(cse::DataGrid &grid, std::ostream &os = std::cout,
                            std::istream &is = std::cin) noexcept;
};
