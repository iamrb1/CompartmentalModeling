/**
 * @file FinalApplication.h
 *
 * @author Max Krawec
 */

#pragma once

#include "src/DataGrid.h"

class FinalApplication {
 private:
  std::optional<double> IsValidDouble(const std::string &input);
  std::optional<int> IsValidInt(const std::string &input);

  int GetColumnIndex(int grid_size, std::ostream &os, std::istream &is);
  cse::Datum GetDataValue(std::ostream &os, std::istream &is);

  void PrintColumn(const cse::ReferenceVector<cse::Datum> &column, std::ostream &os);
 public:
  void StartingMenu(std::ostream &os, std::istream &is);
  cse::DataGrid GridMenu(std::ostream &os, std::istream &is);
  cse::DataGrid CreateGrid(std::ostream &os, std::istream &is);
  void ManipulateGridMenu(cse::DataGrid &grid, std::ostream &os, std::istream &is);
  void MathMenu(const cse::DataGrid &grid, std::ostream &os, std::istream &is);
  void ComparisonMenu(cse::DataGrid &grid, std::ostream &os, std::istream &is);
  void PrintSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is);
  void EditSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is);
  void SortSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is);
  void AddSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is);
  void DeleteSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is);
  void ResizeSubmenu(cse::DataGrid &grid, std::ostream &os, std::istream &is);

};

