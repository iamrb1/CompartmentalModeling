/**
 * @file DataGrid.cpp
 * @author Shahaab Ali
 */

#include "DataGrid.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <vector>

#include "Datum.h"

namespace cse {

/**
 * @brief Returns the shape of the DataGrid
 * @return tuple of (# of rows, # of columns)
 */
std::tuple<const std::size_t, const std::size_t> DataGrid::shape() const {
  const std::size_t num_of_rows = grid_.size();

  // Source: ChatGPT
  const std::size_t num_of_col =
      num_of_rows == 0
          ? 0
          : std::max_element(grid_.begin(), grid_.end(),
                             [](const auto& row1, const auto& row2) {
                               return row1.size() < row2.size();
                             })
                ->size();

  return std::make_tuple(num_of_rows, num_of_col);
}

/**
 * @brief Index a column from DataGrid
 * @param column_index_ Column index to retrieve
 * @return Indexed vector column from DataGrid
 */
std::vector<Datum> DataGrid::getColumn(const std::size_t column_index_) const {
  std::vector<Datum> column;

  for (const auto& row : grid_) {
    if (column_index_ < row.size()) {
      column.push_back(row[column_index_]);
    }
  }
  return column;
}

/**
 * @brief Inserts a row at given index
 * @param row_index_ Index at which to insert the row
 */
void DataGrid::insertRow(const std::size_t row_index_) {
  if (row_index_ > grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }
  grid_.insert(grid_.begin() + row_index_, std::vector<Datum>());
}

/**
 * @brief Inserts a column at given index
 * @param column_index_ Index at which to insert the column
 */
void DataGrid::insertColumn(const std::size_t column_index_) {
  if (!grid_.empty() && column_index_ > grid_[0].size()) {
    throw std::out_of_range("Column index out of bounds");
  }
  for (auto& row : grid_) {
    row.insert(row.begin() + column_index_, Datum(""));
  }
}

/**
 * @brief Deletes a row at given index
 * @param row_index_ Index of the row to delete
 */
void DataGrid::deleteRow(const std::size_t row_index_) {
  if (row_index_ >= grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }
  grid_.erase(grid_.begin() + row_index_);
}

/**
 * @brief Deletes a column at given index
 * @param column_index_ Index of the column to delete
 */
void DataGrid::deleteColumn(const std::size_t column_index_) {
  if (grid_.empty() || column_index_ >= grid_[0].size()) {
    throw std::out_of_range("Column index out of bounds");
  }
  for (auto& row : grid_) {
    row.erase(row.begin() + column_index_);
  }
}

/**
 * @brief Outputs DataGrid values to output stream for clear visualization
 * @param os_ Output stream reference
 * @return Modified output stream
 */
std::ostream& DataGrid::print(std::ostream& os) const {
  auto [rows, cols] = this->shape();
  os << "Grid Shape: " << rows << " x " << cols << "\n";

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < cols; ++j) {
      try {
        auto value = this->getValue(i, j);
        if (value.IsString()) {
          os << value.GetString().value() << " ";
        } else if (value.IsDouble()) {
          os << value.GetDouble().value() << " ";
        }
      } catch (const std::out_of_range&) {
        os << "- ";
      }
    }
    os << "\n";
  }
  return os;
}

}  // namespace cse
