/**
 * @file DataGrid.cpp
 *
 * @author Shahaab Ali and Max Krawec
 */

#include "DataGrid.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "Datum.h"
#include "ReferenceVector.h"

namespace cse {

/**
 * @brief Index a row of the DataGrid
 * @param row_index_ Index of the row
 * @return Indexed vector row from DataGrid
 */
DataGrid::row_t &DataGrid::GetRow(const std::size_t row_index_) {
  assert(row_index_ < grid_.size());

  if (grid_.empty() || row_index_ >= grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  return grid_.at(row_index_);
}

/**
 * @brief Retrieves a row from the DataGrid in a read-only manner.
 *
 * Allows access to a specific row without modifying the DataGrid.
 *
 * @param row_index_ Index of the row to retrieve.
 * @return A const reference to the indexed vector row from the DataGrid.
 */
const DataGrid::row_t &DataGrid::GetRow(const std::size_t row_index_) const {
  assert(row_index_ < grid_.size());

  if (grid_.empty() || row_index_ >= grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  return grid_.at(row_index_);
}

/**
 * @brief Get Datum value from DataGrid
 * @param row_index_ Index of the row
 * @param column_index_ Index of the column
 * @return The Datum at the given index
 */
Datum &DataGrid::GetValue(const std::size_t row_index_,
                          const std::size_t column_index_) {
  assert(!grid_.empty());
  assert(row_index_ < grid_.size());
  assert(column_index_ < grid_[0].size());

  if (grid_.empty() || row_index_ >= grid_.size() ||
      column_index_ >= grid_[0].size()) {
    throw std::out_of_range("Index out of bounds");
  }

  return grid_.at(row_index_).at(column_index_);
}

/**
 * @brief Get const Datum value from DataGrid (used for stream output printing)
 * @param row_index_ Index of the row
 * @param column_index_ Index of the column
 * @return The Datum at the given index
 */
const Datum &DataGrid::GetValue(const std::size_t row_index_,
                                const std::size_t column_index_) const {
  assert(!grid_.empty());
  assert(row_index_ < grid_.size());
  assert(column_index_ < grid_[0].size());

  if (grid_.empty() || row_index_ >= grid_.size() ||
      column_index_ >= grid_[0].size()) {
    throw std::out_of_range("Index out of bounds");
  }

  return grid_.at(row_index_).at(column_index_);
}

/**
 * @brief Returns the shape of the DataGrid
 * @return tuple of (# of rows, # of columns)
 */
std::tuple<std::size_t, std::size_t> DataGrid::Shape() const {
  size_t rows = grid_.size();
  size_t columns = rows == 0 ? 0 : grid_[0].size();
  return std::make_tuple(rows, columns);
}

/**
 * @brief Index a column from DataGrid
 * @param column_index_ Column index to retrieve
 * @return Indexed vector column from DataGrid
 */
cse::ReferenceVector<Datum>
DataGrid::GetColumn(const std::size_t column_index_) {
  assert(!grid_.empty());
  assert(column_index_ < grid_[0].size());

  if (grid_.empty() || column_index_ >= grid_.front().size()) {
    throw std::out_of_range("Column index out of bounds");
  }

  ReferenceVector<Datum> column;

  for (auto &row : grid_) {
    assert(column_index_ < row.size());
    column.PushBack(row.at(column_index_));
  }
  return column;
}

/**
 * @brief Const version of index a column from DataGrid
 * @param column_index_ Column index to retrieve
 * @return Indexed vector column from DataGrid
 */
cse::ReferenceVector<const Datum>
DataGrid::GetColumn(const std::size_t column_index_) const {
  assert(!grid_.empty());
  assert(column_index_ < grid_[0].size());

  if (grid_.empty() || column_index_ >= grid_.front().size()) {
    throw std::out_of_range("Column index out of bounds");
  }

  ReferenceVector<const Datum> column;

  for (auto &row : grid_) {
    assert(column_index_ < row.size());
    column.PushBack(row.at(column_index_));
  }
  return column;
}

/**
 * @brief Inserts a row at given index
 * @param row_index_ Index at which to insert the row
 * @param default_value_ of row values
 */
void DataGrid::InsertDefaultRow(std::size_t row_index_, double default_value_) {
  // If row_index_ is not given, update row_index_ to insert to the end of grid
  if (grid_.empty() || row_index_ == kNoIndex) {
    row_index_ = grid_.size();
  }

  assert(row_index_ <= grid_.size());
  if (row_index_ > grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  const std::size_t num_columns = std::get<1>(Shape());

  // Source ChatGPT
  // Inserts row at row_index_
  // row_index_ must be cast for proper iterator addition
  grid_.insert(grid_.begin() + static_cast<std::ptrdiff_t>(row_index_),
               std::vector<Datum>(num_columns, Datum(default_value_)));
}

/**
 * @brief Inserts a row at given index
 * @param row_index_ Index at which to insert the row
 * @param default_value_ of row
 */
void DataGrid::InsertDefaultRow(std::size_t row_index_,
                                std::string default_value_) {
  // If row_index_ is not given, update row_index_ to insert to the end of grid
  if (grid_.empty() || row_index_ == kNoIndex) {
    row_index_ = grid_.size();
  }

  assert(row_index_ <= grid_.size());
  if (row_index_ > grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  const std::size_t num_columns = std::get<1>(Shape());

  // Source ChatGPT
  // Inserts row at row_index_
  // row_index_ must be cast for proper iterator addition
  grid_.insert(
      grid_.begin() + static_cast<std::ptrdiff_t>(row_index_),
      std::vector<Datum>(num_columns, Datum(std::move(default_value_))));
}

/**
 * @brief Inserts a column at given index
 * @param column_index_ Index at which to insert the column
 * @param default_value_ of column
 */
void DataGrid::InsertDefaultColumn(std::size_t column_index_,
                                   double default_value_) {
  // If grid is empty, add the column, with each value being a row
  if (grid_.empty()) {
    grid_.emplace_back(1, Datum(default_value_));
    return;
  }

  // If column_index_ is max, update it to insert at the end
  if (column_index_ == kNoIndex) {
    column_index_ = grid_[0].size();
  }

  assert(column_index_ <= grid_[0].size());
  if (column_index_ > grid_.front().size()) {
    throw std::out_of_range("Column index out of bounds");
  }

  // Insert the new column at the specified index in each row
  // column_index_ must be cast for proper iterator addition
  for (auto &row : grid_) {
    row.insert(row.begin() + static_cast<std::ptrdiff_t>(column_index_),
               Datum(default_value_));
  }
}

/**
 * @brief Inserts a column at given index
 * @param column_index_ Index at which to insert the column
 * @param default_value_ of column
 */
void DataGrid::InsertDefaultColumn(std::size_t column_index_,
                                   const std::string &default_value_) {
  // If grid is empty, add the column, with each value being a row
  if (grid_.empty()) {
    grid_.emplace_back(1, Datum(default_value_));
    return;
  }

  // If column_index_ is max, update it to insert at the end
  if (column_index_ == kNoIndex) {
    column_index_ = grid_[0].size();
  }

  assert(column_index_ <= grid_[0].size());
  if (column_index_ > grid_.front().size()) {
    throw std::out_of_range("Column index out of bounds");
  }

  // Insert the new column at the specified index in each row
  // column_index_ must be cast for proper iterator addition
  for (auto &row : grid_) {
    row.insert(row.begin() + static_cast<std::ptrdiff_t>(column_index_),
               Datum(default_value_));
  }
}

/**
 * Insert a pre-made row into the DataGrid
 * @param row_
 * @param row_index_
 */
void DataGrid::InsertRow(const std::vector<Datum>& row_, std::size_t row_index_) {
  if (grid_.empty() || row_index_ == kNoIndex) {
    row_index_ = grid_.size();
  }

  assert(row_index_ <= grid_.size());
  // column_index_ must be cast for proper iterator addition
  grid_.insert(grid_.begin() + static_cast<std::ptrdiff_t>(row_index_), row_);
}

/**
 * Insert a pre-made column into the DataGrid
 * @param column_
 * @param column_index_
 */
void DataGrid::InsertColumn(const std::vector<Datum>& column_, std::size_t column_index_) {
  // If grid is empty, create rows from column_
  if (grid_.empty()) {
    for (const auto& value : column_) {
      grid_.emplace_back(1, value);
    }
    return;
  }

  // Ensure column size matches the number of rows
  if (column_.size() != grid_.size()) {
    throw std::invalid_argument("Column size does not match number of rows");
  }

  // If column_index_ is max, append to the end
  if (column_index_ == kNoIndex) {
    column_index_ = grid_[0].size();
  }

  assert(column_index_ <= grid_[0].size());
  if (column_index_ > grid_.front().size()) {
    throw std::out_of_range("Column index out of bounds");
  }

  // Insert values from column_ into each row at the specified index
  // column_index_ must be cast for proper iterator addition
  for (std::size_t i = 0; i < grid_.size(); ++i) {
    grid_[i].insert(grid_[i].begin() + static_cast<std::ptrdiff_t>(column_index_), column_[i]);
  }
}

/**
 * @brief Deletes a row at given index
 * @param row_index_ Index of the row to delete
 */
void DataGrid::DeleteRow(const std::size_t row_index_) {
  assert(!grid_.empty());
  assert(row_index_ < grid_.size());

  if (row_index_ >= grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  // row_index_ must be cast for proper iterator addition
  grid_.erase(grid_.begin() + static_cast<std::ptrdiff_t>(row_index_));
}

/**
 * @brief Deletes a column at given index
 * @param column_index_ Index of the column to delete
 */
void DataGrid::DeleteColumn(const std::size_t column_index_) {
  assert(!grid_.empty());
  assert(column_index_ < grid_[0].size());

  if (grid_.empty() || column_index_ >= grid_.front().size()) {
    throw std::out_of_range("Column index out of bounds");
  }

  for (auto &row : grid_) {
    // column_index_ must be cast for proper iterator addition
    row.erase(row.begin() + static_cast<std::ptrdiff_t>(column_index_));
  }
}

/**
 * @brief Resizes datagrid to any rectangular size with default_value_
 * @param num_rows_
 * @param num_columns_
 * @param default_value_
 */
void DataGrid::Resize(const std::size_t num_rows_,
                      const std::size_t num_columns_,
                      const double default_value_) {
  if (grid_.size() < num_rows_) {
    grid_.resize(num_rows_,
                 std::vector<Datum>(num_columns_, Datum(default_value_)));
  } else if (grid_.size() > num_rows_) {
    grid_.resize(num_rows_);
  }

  for (auto &row : grid_) {
    if (row.size() < num_columns_) {
      row.insert(row.end(), num_columns_ - row.size(), Datum(default_value_));
    } else if (row.size() > num_columns_) {
      row.resize(num_columns_);
    }
  }
}

/**
 * @brief Resizes datagrid to any rectangular size with default_value_
 * @param num_rows_
 * @param num_columns_
 * @param default_value_
 */
void DataGrid::Resize(const std::size_t num_rows_,
                      const std::size_t num_columns_,
                      const std::string &default_value_) {
  if (grid_.size() < num_rows_) {
    grid_.resize(num_rows_,
                 std::vector<Datum>(num_columns_, Datum(default_value_)));
  } else if (grid_.size() > num_rows_) {
    grid_.resize(num_rows_);
  }

  for (auto &row : grid_) {
    if (row.size() < num_columns_) {
      row.insert(row.end(), num_columns_ - row.size(), Datum(default_value_));
    } else if (row.size() > num_columns_) {
      row.resize(num_columns_);
    }
  }
}

/**
 * @brief Outputs DataGrid values to output stream for clear visualization
 * @param os_
 * @return Modified output stream
 */
std::ostream &DataGrid::Print(std::ostream &os_) const {
  auto [rows, columns] = Shape();
  os_ << "Grid Shape: " << rows << " x " << columns << "\n";

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < columns; ++j) {
      if (auto value = GetValue(i, j); value.IsString()) {
        os_ << value.GetString() << " ";
      } else if (value.IsDouble()) {
        os_ << value.GetDouble() << " ";
      }
    }
    os_ << "\n";
  }
  return os_;
}

/**
 * @brief Access a datum in the datagrid
 * @param row_index_
 * @param column_index_
 * @return Reference to the Datum at the given index
 */
Datum &DataGrid::At(std::size_t row_index_, std::size_t column_index_) {
  assert(row_index_ < grid_.size());
  assert(column_index_ < grid_[0].size());
  return grid_[row_index_][column_index_];
}

/**
 * @brief Access a datum in the datagrid (const version)
 * @param row_index_
 * @param column_index_
 * @return Const reference to the Datum at the given index
 */
const Datum &DataGrid::At(std::size_t row_index_,
                          std::size_t column_index_) const {
  assert(row_index_ < grid_.size());
  assert(column_index_ < grid_[0].size());
  return grid_[row_index_][column_index_];
}

/**
 * @brief Access a row in the datagrid
 * @param row_index_
 * @return Reference to the row at the given index
 */
DataGrid::row_t &DataGrid::At(std::size_t row_index_) {
  assert(row_index_ < grid_.size());

  if (grid_.empty() || row_index_ >= grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  return grid_[row_index_];
}

/**
 * @brief Access a row in the datagrid (const version)
 * @param row_index_
 * @return Const reference to the row at the given index
 */
const DataGrid::row_t &DataGrid::At(std::size_t row_index_) const {
  assert(row_index_ < grid_.size());

  if (grid_.empty() || row_index_ >= grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  return grid_[row_index_];
}

/**
 * Returns a DataGrid of two merged DataGrids by Row or Column
 * @param grid_two
 * @param row_append
 * @return merged DataGrid
 */
DataGrid DataGrid::Merge(const DataGrid &grid_two, bool row_append) const {
  // Return grid one or two if the other is empty
  if (grid_.empty()) return grid_two;
  if (grid_two.grid_.empty()) return *this;

  DataGrid merged = *this; // Copy

  // Adding the rows of grid_two to current grid_
  if (row_append) {
    assert(grid_two.grid_[0].size() == grid_[0].size());
    merged.grid_.insert(merged.grid_.end(), grid_two.grid_.begin(),
                        grid_two.grid_.end());
  }

  // Adding the columns of grid_two to current grid_
  else {
    assert(grid_two.grid_.size() == grid_.size());
    for (size_t i = 0; i < merged.grid_.size(); ++i) {
      merged.grid_[i].insert(merged.grid_[i].end(), grid_two.grid_[i].begin(),
                             grid_two.grid_[i].end());
    }
  }

  return merged;
}

/**
 * Returns a rectangular slice of the existing DataGrid
 * @param start_row_index
 * @param end_row_index
 * @param start_column_index
 * @param end_column_index
 * @return DataGrid
 */
DataGrid DataGrid::Slice(std::size_t start_row_index, std::size_t end_row_index,
                         std::size_t start_column_index,
                         std::size_t end_column_index) {
  assert(start_row_index < grid_.size());
  assert(end_row_index < grid_.size());
  assert(start_column_index < grid_[0].size());
  assert(end_column_index < grid_[0].size());

  std::vector<std::vector<Datum>> sliced_data;

  for (std::size_t i = start_row_index; i <= end_row_index; ++i) {
    // Help from ChatGPT
    std::vector<Datum> row_slice(
    // column indices must be cast for proper iterator addition
        grid_[i].begin() + static_cast<std::ptrdiff_t>(start_column_index),
        grid_[i].begin() + static_cast<std::ptrdiff_t>(end_column_index + 1));
    sliced_data.push_back(row_slice);
  }

  return DataGrid(sliced_data);
}

/**
 * Sort rows by a columns
 * @param column_index
 * @param ascending
 */
void DataGrid::SortColumn(std::size_t column_index, bool ascending) {
  assert(!grid_.empty());
  assert(column_index < grid_[0].size());

  // Created with the help of ChatGPT
  // Sort using a custom lambda function, where doubles > strings
  std::ranges::sort(
      grid_, [column_index, ascending](const std::vector<Datum> &a,
                                       const std::vector<Datum> &b) {
        const Datum &datumA = a[column_index];
        const Datum &datumB = b[column_index];

        // Prioritize doubles over strings
        if (datumA.IsDouble() && datumB.IsString())
          return true;
        if (datumA.IsString() && datumB.IsDouble())
          return false;

        // Compare values accordingly
        if (datumA.IsDouble() && datumB.IsDouble()) {
          return ascending ? datumA.GetDouble() < datumB.GetDouble()
                           : datumA.GetDouble() > datumB.GetDouble();
        }
        if (datumA.IsString() && datumB.IsString()) {
          return ascending ? datumA.GetString() < datumB.GetString()
                           : datumA.GetString() > datumB.GetString();
        }
        return false;
      });
}

/**
 * Sorts the entire grid rows by column, giving priority to left most columns
 * @param ascending
 */
void DataGrid::Sort(bool ascending) {
  assert(!grid_.empty());

  // Created with the help of ChatGPT
  // Sort using a custom lambda function, where doubles > strings
  std::ranges::sort(grid_, [ascending](const std::vector<Datum> &a,
                                       const std::vector<Datum> &b) {
    size_t min_size = std::min(a.size(), b.size());

    for (size_t i = 0; i < min_size; ++i) {
      const Datum &datumA = a[i];
      const Datum &datumB = b[i];

      // Prioritize doubles over strings
      if (datumA.IsDouble() && datumB.IsString())
        return true;
      if (datumA.IsString() && datumB.IsDouble())
        return false;

      // Compare values accordingly
      if (datumA.IsDouble() && datumB.IsDouble()) {
        if (datumA.GetDouble() != datumB.GetDouble()) {
          return ascending ? datumA.GetDouble() < datumB.GetDouble()
                           : datumA.GetDouble() > datumB.GetDouble();
        }
      } else if (datumA.IsString() && datumB.IsString()) {
        if (datumA.GetString() != datumB.GetString()) {
          return ascending ? datumA.GetString() < datumB.GetString()
                           : datumA.GetString() > datumB.GetString();
        }
      }
    }
    return false;
  });
}

/**
 * row indices where value is present
 * @param column_index
 * @param value
 * @return vector of indices
 */
std::vector<size_t> DataGrid::Search(std::size_t column_index,
                                     const Datum &value) {
  std::vector<size_t> result;
  for (size_t i = 0; i < grid_.size(); ++i) {
    if (GetValue(i, column_index) == value)
      result.push_back(i);
  }

  return result;
}

/**
 * Calculates the mean of the column
 * @param column_index The desired column index
 * @return The mean of the desired column
 */
double DataGrid::ColumnMean(std::size_t column_index) const {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  return CalculateMean(GetDoubleValues(GetColumn(column_index)));
}

/**
 * Calculates the median of the column
 * @param column_index The desired column index
 * @return The median of the desired column
 */
double DataGrid::ColumnMedian(std::size_t column_index) const {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  return CalculateMedian(GetDoubleValues(GetColumn(column_index)));
}

/**
 * Calculates the modes of the column
 * @param column_index The desired column index
 * @return A list of modes from the desired column
 */
std::vector<double> DataGrid::ColumnMode(std::size_t column_index) const {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  return CalculateMode(GetDoubleValues(GetColumn(column_index)));
}

/**
 * Calculates the standard deviation of a column
 * @param column_index The desired column index
 * @return The standard deviation of the desired column
 */
double DataGrid::ColumnStandardDeviation(std::size_t column_index) const {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  return CalculateStandardDeviation(GetDoubleValues(GetColumn(column_index)));
}

/**
 * Determines the smallest element of a column
 * @param column_index The desired column index
 * @return The minimum element of the desired column
 */
double DataGrid::ColumnMin(std::size_t column_index) const {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  return CalculateMin(GetDoubleValues(GetColumn(column_index)));
}

/**
 * Determines the largest element of a column
 * @param column_index The desired column index
 * @return The maximum element of the desired column
 */
double DataGrid::ColumnMax(std::size_t column_index) const {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  return CalculateMax(GetDoubleValues(GetColumn(column_index)));
}

/**
 * Calculates the mean, median, mode, standard deviation, min, and max of the data grid.
 * @return The mean, median, mode, standard deviation, min, and max of the data grid
 */
DataGrid::DataGridMathSummary DataGrid::CalculateDataGridMathSummary() const {
  std::vector<double> double_values;
  for (const std::vector<Datum>& row : GetDataGrid()) {
    for (const Datum& value : row) {
      if (value.IsDouble() && !std::isnan(value.GetDouble())) {
        double_values.push_back(value.GetDouble());
      }
    }
  }

  DataGridMathSummary data_grid_math_summary;
  data_grid_math_summary.mean = CalculateMean(double_values);
  data_grid_math_summary.median = CalculateMedian(double_values);
  data_grid_math_summary.mode = CalculateMode(double_values);
  data_grid_math_summary.standardDeviation = CalculateStandardDeviation(double_values);
  data_grid_math_summary.min = CalculateMin(double_values);
  data_grid_math_summary.max = CalculateMax(double_values);
  return data_grid_math_summary;
}

/**
 * Calculates the mean of the vector.
 * @param double_values The given doubles vector.
 * @return The mean of the vector
 */
constexpr double DataGrid::CalculateMean(const std::vector<double>& double_values) {
  if (double_values.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  assert(!double_values.empty());
  return std::accumulate(double_values.begin(), double_values.end(), 0.0) /
      static_cast<int>(double_values.size());
}

/**
 * Calculates the median of the vector.
 * @param double_values The given doubles vector.
 * @return The median of the vector
 */
double DataGrid::CalculateMedian(std::vector<double> double_values) {
  if (double_values.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  std::ranges::sort(double_values);

  int size = static_cast<int>(double_values.size());
  if (size % 2 == 0) {
    return (double_values[size / 2] + double_values[size / 2 - 1]) / 2;
  }
  return double_values[size / 2];
}

// CITE: Took inspiration from
// https://www.geeksforgeeks.org/how-to-find-the-mode-of-all-elements-in-a-list-in-cpp/,
// but modified it so it would return multiple modes.
/**
 * Calculates the mode(s) of the vector.
 * @param double_values The given doubles vector.
 * @return The mode(s) of the vector
 */
std::vector<double> DataGrid::CalculateMode(const std::vector<double>& double_values) {
  if (double_values.empty()) {
    return {};
  }

  std::unordered_map<double, int> frequency_counter;
  for (double values : double_values) {
    frequency_counter[values]++;
  }

  std::vector<double> modes;
  int current_max_frequency = -1;
  for (auto value_frequency : frequency_counter) {
    if (current_max_frequency < value_frequency.second) {
      modes.clear();
      modes.push_back(value_frequency.first);
      current_max_frequency = value_frequency.second;
    } else if (current_max_frequency == value_frequency.second) {
      modes.push_back(value_frequency.first);
    }
  }
  return modes;
}

/**
 * Calculates the standard deviation of the vector.
 * @param double_values The given doubles vector.
 * @return The standard deviation of the vector
 */
constexpr double DataGrid::CalculateStandardDeviation(const std::vector<double>& double_values) {
  if (double_values.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  double current_total = 0;
  const double mean = CalculateMean(double_values);
  for (const double value : double_values) {
    current_total += std::pow(value - mean, 2);
  }

  assert(!double_values.empty());
  return sqrt(current_total / static_cast<int>(double_values.size()));
}

/**
 * Calculates the min value of the vector.
 * @param double_values The given doubles vector.
 * @return The min value of the vector
 */
constexpr double DataGrid::CalculateMin(const std::vector<double>& double_values) {
  if (double_values.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  // Used https://en.cppreference.com/w/cpp/algorithm/min_element for min
  // element
  return *std::ranges::min_element(double_values);
}

/**
 * Calculates the max value of the vector.
 * @param double_values The given doubles vector.
 * @return The max value of the vector
 */
constexpr double DataGrid::CalculateMax(const std::vector<double>& double_values) {
  if (double_values.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  // Used https://en.cppreference.com/w/cpp/algorithm/max_element for max
  // element
  return *std::ranges::max_element(double_values);
}

/**
 * Helper function that removes any strings from a column for the math
 * functions.
 * @param reference_vector The column
 * @return A vector that contains only double data
 */
std::vector<double>
DataGrid::GetDoubleValues(const ReferenceVector<const Datum> &reference_vector) {
  std::vector<double> double_values;
  for (const auto &value : reference_vector) {
    if (value.IsDouble() && !std::isnan(value.GetDouble())) {
      assert(!std::isnan(value.IsDouble()));
      double_values.push_back(value.GetDouble());
    }
  }
  return double_values;
}

/**
 * Returns Datums than are less than a given value for the specified column.
 * @param column_index The index for the column.
 * @param value The value to compare with.
 * @return Datums than are less than a given value for the specified column.
 */
cse::ReferenceVector<Datum> DataGrid::ColumnLessThan(std::size_t column_index,
                                                     const Datum &value) {
  return DetermineColumnComparisons(column_index, value, operations::LESS_THAN);
}

/**
 * Returns Datums than are less than or equal to a given value for the specified column.
 * @param column_index The index for the column.
 * @param value The value to compare with.
 * @return Datums than are less than or equal to a given value for the specified column.
 */
cse::ReferenceVector<Datum>
DataGrid::ColumnLessThanOrEqual(std::size_t column_index, const Datum &value) {
  return DetermineColumnComparisons(column_index, value,
                                    operations::LESS_THAN_OR_EQUAL);
}

/**
 * Returns Datums than are greater than a given value for the specified column.
 * @param column_index The index for the column.
 * @param value The value to compare with.
 * @return Datums than are greater than a given value for the specified column.
 */
cse::ReferenceVector<Datum> DataGrid::ColumnGreaterThan(size_t column_index,
                                                        const Datum &value) {
  return DetermineColumnComparisons(column_index, value,
                                    operations::GREATER_THAN);
}

/**
 * Returns Datums than are greater than or equal to a given value for the specified column.
 * @param column_index The index for the column.
 * @param value The value to compare with.
 * @return Datums than are greater than or equal to a given value for the specified column.
 */
cse::ReferenceVector<Datum>
DataGrid::ColumnGreaterThanOrEqual(size_t column_index, const Datum &value) {
  return DetermineColumnComparisons(column_index, value,
                                    operations::GREATER_THAN_OR_EQUAL);
}

/**
 * Returns Datums than are equal to a given value for the specified column.
 * @param column_index The index for the column.
 * @param value The value to compare with.
 * @return Datums than are equal to a given value for the specified column.
 */
cse::ReferenceVector<Datum> DataGrid::ColumnEqual(size_t column_index,
                                                  const Datum &value) {
  return DetermineColumnComparisons(column_index, value, operations::EQUAL);
}

/**
 * Returns Datums than are not equal to a given value for the specified column.
 * @param column_index The index for the column.
 * @param value The value to compare with.
 * @return Datums than are not equal to a value for the specified column.
 */
cse::ReferenceVector<Datum> DataGrid::ColumnNotEqual(size_t column_index,
                                                     const Datum &value) {
  return DetermineColumnComparisons(column_index, value, operations::NOT_EQUAL);
}

/**
 * Helper function that determines what Datum type to compare (double or string) and what comparison to do.
 * @param column_index The index for the column.
 * @param value  The value to compare with.
 * @param operation The comparison operation
 * @return A ReferenceVector of Datums based on the comparison for the specified column.
 */
cse::ReferenceVector<Datum>
DataGrid::DetermineColumnComparisons(std::size_t column_index,
                                     const Datum &value, operations operation) {

  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  ReferenceVector<Datum> values;
  ReferenceVector<Datum> column = GetColumn(column_index);

  // CITE: Used chatgpt and claude to help write this code which removed a lot of duplication.
  // Mostly received help for the lamda.
  auto compare_and_store = [&values, operation](const auto &current_value,
                                                const auto &compare_value,
                                                Datum &datum) {
    switch (operation) {
      case operations::LESS_THAN:
        if (current_value < compare_value)
          values.PushBack(datum);
        break;
      case operations::LESS_THAN_OR_EQUAL:
        if (current_value <= compare_value)
          values.PushBack(datum);
        break;
      case operations::GREATER_THAN:
        if (current_value > compare_value)
          values.PushBack(datum);
        break;
      case operations::GREATER_THAN_OR_EQUAL:
        if (current_value >= compare_value)
          values.PushBack(datum);
        break;
      case operations::EQUAL:
        if (current_value == compare_value)
          values.PushBack(datum);
        break;
      case operations::NOT_EQUAL:
        if (current_value != compare_value)
          values.PushBack(datum);
        break;
      default:
        throw std::invalid_argument("Invalid comparison argument");
    }
  };

  for (Datum &datum : column) {
    if (datum.IsDouble() && value.IsDouble()) {
      compare_and_store(datum.GetDouble(), value.GetDouble(), datum);
    }
    if (datum.IsString() && value.IsString()) {
      compare_and_store(datum.GetString(), value.GetString(), datum);
    }
  }
  return values;
}
} // namespace cse
