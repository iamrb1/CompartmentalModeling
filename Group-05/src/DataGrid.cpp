/**
 * @file DataGrid.cpp
 * @author Shahaab Ali
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
DataGrid::row_t &DataGrid::getRow(const std::size_t row_index_) {
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
const DataGrid::row_t &DataGrid::getRow(const std::size_t row_index_) const {
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
Datum &DataGrid::getValue(const std::size_t row_index_,
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
const Datum &DataGrid::getValue(const std::size_t row_index_,
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
std::tuple<std::size_t, std::size_t> DataGrid::shape() const {
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
DataGrid::getColumn(const std::size_t column_index_) {
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
 * @brief Inserts a row at given index
 * @param row_index_ Index at which to insert the row
 * @param default_value_ of row values
 */
void DataGrid::insertDefaultRow(std::size_t row_index_, double default_value_) {
  assert(row_index_ <= grid_.size());

  if (row_index_ > grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  // If row_index_ is not given, update row_index_ to insert to the end of grid
  if (row_index_ == std::numeric_limits<std::size_t>::max()) {
    row_index_ = grid_.size();
  }

  const std::size_t num_columns = std::get<1>(shape());

  // Source ChatGPT
  // Inserts row at row_index_
  grid_.insert(grid_.begin() + static_cast<std::ptrdiff_t>(row_index_),
               std::vector<Datum>(num_columns, Datum(default_value_)));
}

/**
 * @brief Inserts a row at given index
 * @param row_index_ Index at which to insert the row
 * @param default_value_ of row
 */
void DataGrid::insertDefaultRow(std::size_t row_index_,
                                std::string default_value_) {
  assert(row_index_ <= grid_.size());

  if (row_index_ > grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  // If row_index_ is not given, update row_index_ to insert to the end of grid
  if (row_index_ == std::numeric_limits<std::size_t>::max()) {
    row_index_ = grid_.size();
  }

  const std::size_t num_columns = std::get<1>(shape());

  // Source ChatGPT
  // Inserts row at row_index_
  grid_.insert(
      grid_.begin() + static_cast<std::ptrdiff_t>(row_index_),
      std::vector<Datum>(num_columns, Datum(std::move(default_value_))));
}

/**
 * @brief Inserts a column at given index
 * @param column_index_ Index at which to insert the column
 * @param default_value_ of column
 */
void DataGrid::insertDefaultColumn(std::size_t column_index_,
                                   double default_value_) {
  assert(!grid_.empty());
  assert(column_index_ <= grid_[0].size());

  if (grid_.empty() || column_index_ > grid_.front().size()) {
    throw std::out_of_range("Column index out of bounds");
  }

  // If column_index_ is not given, update column_index_ to insert to the end of
  // grid
  if (column_index_ == std::numeric_limits<std::size_t>::max()) {
    column_index_ = grid_[0].size();
  }

  // Append values to each row
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
void DataGrid::insertDefaultColumn(std::size_t column_index_,
                                   const std::string &default_value_) {
  assert(!grid_.empty());
  assert(column_index_ <= grid_[0].size());

  if (grid_.empty() || column_index_ > grid_.front().size()) {
    throw std::out_of_range("Column index out of bounds");
  }

  // If column_index_ is not given, update column_index_ to insert to the end of
  // grid
  if (column_index_ == std::numeric_limits<std::size_t>::max()) {
    column_index_ = grid_[0].size();
  }

  // Append values to each row
  for (auto &row : grid_) {
    row.insert(row.begin() + static_cast<std::ptrdiff_t>(column_index_),
               Datum(default_value_));
  }
}

/**
 * @brief Deletes a row at given index
 * @param row_index_ Index of the row to delete
 */
void DataGrid::deleteRow(const std::size_t row_index_) {
  assert(!grid_.empty());
  assert(row_index_ < grid_.size());

  if (row_index_ >= grid_.size()) {
    throw std::out_of_range("Row index out of bounds");
  }

  grid_.erase(grid_.begin() + static_cast<std::ptrdiff_t>(row_index_));
}

/**
 * @brief Deletes a column at given index
 * @param column_index_ Index of the column to delete
 */
void DataGrid::deleteColumn(const std::size_t column_index_) {
  assert(!grid_.empty());
  assert(column_index_ < grid_[0].size());

  if (grid_.empty() || column_index_ >= grid_.front().size()) {
    throw std::out_of_range("Column index out of bounds");
  }

  for (auto &row : grid_) {
    row.erase(row.begin() + static_cast<std::ptrdiff_t>(column_index_));
  }
}

/**
 * @brief Resizes datagrid to any rectangular size with default_value_
 * @param num_rows_
 * @param num_columns_
 * @param default_value_
 */
void DataGrid::resize(const std::size_t num_rows_,
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
void DataGrid::resize(const std::size_t num_rows_,
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
std::ostream &DataGrid::print(std::ostream &os_) const {
  auto [rows, columns] = shape();
  os_ << "Grid Shape: " << rows << " x " << columns << "\n";

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < columns; ++j) {
      if (auto value = getValue(i, j); value.IsString()) {
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
Datum &DataGrid::at(std::size_t row_index_, std::size_t column_index_) {
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
const Datum &DataGrid::at(std::size_t row_index_,
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
DataGrid::row_t &DataGrid::at(std::size_t row_index_) {
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
const DataGrid::row_t &DataGrid::at(std::size_t row_index_) const {
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
DataGrid DataGrid::merge(const DataGrid &grid_two, bool row_append) const {
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
DataGrid DataGrid::slice(std::size_t start_row_index, std::size_t end_row_index,
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
void DataGrid::sortColumn(std::size_t column_index, bool ascending) {
  assert(!grid_.empty());
  assert(column_index < grid_[0].size());

  // Created with the help of ChatGPT
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

        return false; // Shouldn't reach here
      });
}

/**
 * Sorts the entire grid rows by column, giving priority to left most columns
 * @param ascending
 */
void DataGrid::sort(bool ascending) {
  assert(!grid_.empty());

  // Created with the help of ChatGPT
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

    // Preserve order if all elements are equal
    return false;
  });
}

/**
 * row indices where value is present
 * @param column_index
 * @param value
 * @return vector of indices
 */
std::vector<size_t> DataGrid::search(std::size_t column_index,
                                     const Datum &value) {
  std::vector<size_t> result;
  for (size_t i = 0; i < grid_.size(); ++i) {
    if (getValue(i, column_index) == value)
      result.push_back(i);
  }

  return result;
}

/**
 * Calculates the mean of the column
 * @param column_index The desired column index
 * @return The mean of the desired column
 */
double DataGrid::columnMean(std::size_t column_index) {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  std::vector<double> double_values = getDoubleValues(getColumn(column_index));
  if (double_values.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  return std::accumulate(double_values.begin(), double_values.end(), 0.0) /
         static_cast<int>(double_values.size());
}

/**
 * Calculates the median of the column
 * @param column_index The desired column index
 * @return The median of the desired column
 */
double DataGrid::columnMedian(std::size_t column_index) {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  std::vector<double> double_values = getDoubleValues(getColumn(column_index));
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
 * Calculates the modes of the column
 * @param column_index The desired column index
 * @return A list of modes from the desired column
 */
std::vector<double> DataGrid::columnMode(std::size_t column_index) {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  std::vector<double> double_values = getDoubleValues(getColumn(column_index));
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
 * Calculates the standard deviation of a column
 * @param column_index The desired column index
 * @return The standard deviation of the desired column
 */
double DataGrid::columnStandardDeviation(std::size_t column_index) {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  std::vector<double> double_values = getDoubleValues(getColumn(column_index));
  if (double_values.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  double current_total = 0;
  double mean = columnMean(column_index);
  for (double value : double_values) {
    current_total += pow(value - mean, 2);
  }
  return sqrt(current_total / static_cast<int>(double_values.size()));
}

/**
 * Determines the smallest element of a column
 * @param column_index The desired column index
 * @return The minimum element of the desired column
 */
double DataGrid::columnMin(std::size_t column_index) {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  std::vector<double> double_values = getDoubleValues(getColumn(column_index));
  if (double_values.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  // Used https://en.cppreference.com/w/cpp/algorithm/min_element for min
  // element
  return *std::ranges::min_element(double_values);
}

/**
 * Determines the largest element of a column
 * @param column_index The desired column index
 * @return The maximum element of the desired column
 */
double DataGrid::columnMax(std::size_t column_index) {
  if (grid_.empty() || column_index >= grid_[0].size()) {
    throw std::out_of_range("Column index out of range.");
  }

  std::vector<double> double_values = getDoubleValues(getColumn(column_index));
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
DataGrid::getDoubleValues(const ReferenceVector<Datum> &reference_vector) {
  std::vector<double> double_values;
  for (const auto &value : reference_vector) {
    if (value.IsDouble()) {
      double_values.push_back(value.GetDouble());
    }
  }
  return double_values;
}

// CITE: Used chatgpt and claude to help write this code which removed a lot of
// duplication. Although there is still some with the switch statements, it's
// much nicer now. WIP
cse::ReferenceVector<Datum>
DataGrid::determineColumnComparisons(std::size_t column_index,
                                     const Datum &value, operations operation) {
  ReferenceVector<Datum> values;
  ReferenceVector<Datum> column = getColumn(column_index);

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
      break;
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

cse::ReferenceVector<Datum> DataGrid::columnLessThan(std::size_t column_index,
                                                     const Datum &value) {
  return determineColumnComparisons(column_index, value, operations::LESS_THAN);
}

cse::ReferenceVector<Datum>
DataGrid::columnLessThanOrEqual(std::size_t column_index, const Datum &value) {
  return determineColumnComparisons(column_index, value,
                                    operations::LESS_THAN_OR_EQUAL);
}

cse::ReferenceVector<Datum> DataGrid::columnGreaterThan(size_t column_index,
                                                        const Datum &value) {
  return determineColumnComparisons(column_index, value,
                                    operations::GREATER_THAN);
}

cse::ReferenceVector<Datum>
DataGrid::columnGreaterThanOrEqual(size_t column_index, const Datum &value) {
  return determineColumnComparisons(column_index, value,
                                    operations::GREATER_THAN_OR_EQUAL);
}

cse::ReferenceVector<Datum> DataGrid::columnEqual(size_t column_index,
                                                  const Datum &value) {
  return determineColumnComparisons(column_index, value, operations::EQUAL);
}

cse::ReferenceVector<Datum> DataGrid::columnNotEqual(size_t column_index,
                                                     const Datum &value) {
  return determineColumnComparisons(column_index, value, operations::NOT_EQUAL);
}

} // namespace cse
