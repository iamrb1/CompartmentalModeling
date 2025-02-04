/**
 * @file StateGrid.h
 * @author Dominik Leisinger
 *
 *
 */

#pragma once

#include <vector>
#include <string>

namespace cse {
class StateGrid {
 private:
  ///Rows in grid
  size_t rows = 0;
  ///Rows in grid
  size_t cols = 0;
  ///Grid to represent game map
  std::vector<std::vector<std::string>> grid;

 public:
  StateGrid(size_t rows, size_t cols);

  void DisplayGrid();
};
}