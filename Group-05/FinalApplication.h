/**
 * @file FinalApplication.h
 *
 * @author Max Krawec
 */

#pragma once

#include "src/DataGrid.h"

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

