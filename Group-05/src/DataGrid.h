/**
 * @file DataGrid.h
 * @author Shahaab Ali
 *
 * @brief A 2D data table providing a simple and efficient tabular interface.
 */

#pragma once

#include <iomanip>
#include <vector>

#include "Datum.h"

namespace cse {

/**
 * @class DataGrid
 * @brief A 2D data table providing a simple and efficient tabular interface.
 */
class DataGrid {
 private:
  using row_t = std::vector<Datum>;  // row_t is an alias for a vector of Datums

  /// A 2D vector holding datums
  std::vector<row_t> mGrid;

 public:
  /**
   * @brief Default Constructor
   */
  DataGrid() = default;

  /**
   * @brief Default Destructor
   */
  ~DataGrid() = default;

  /**
   * @brief Index a row of the DataGrid
   * @param row_index Index of the row
   * @return Indexed vector row from DataGrid
   */
  row_t& operator[](const std::size_t row_index) {
    return mGrid.at(row_index);
  }

  /**
   * @brief Index a row of the DataGrid
   * @param row_index Index of the row
   * @return Indexed vector row from DataGrid
   */
  row_t& GetRow(const std::size_t row_index) {
    return mGrid.at(row_index);
  }

  /**
 * @brief Retrieves a row from the DataGrid in a read-only manner.
 *
 * Allows access to a specific row without modifying the DataGrid.
 * 
 * @param row_index Index of the row to retrieve.
 * @return A const reference to the indexed vector row from the DataGrid.
 */
const row_t& GetRow(const std::size_t row_index) const {
  return mGrid.at(row_index);
}

  /**
   * @brief Get Datum value from DataGrid
   * @param row_index Index of the row
   * @param column_index Index of the column
   * @return The Datum at the given index
   */
  Datum& GetValue(const std::size_t row_index, const std::size_t column_index) {
    return mGrid.at(row_index).at(column_index);
  }

  /**
   * @brief Get const Datum value from DataGrid (used for stream output printing)
   * @param row_index Index of the row
   * @param column_index Index of the column
   * @return The Datum at the given index
   */
  const Datum& GetValue(std::size_t row_index, std::size_t column_index) const {
    return mGrid.at(row_index).at(column_index);
  }

  std::tuple<const std::size_t, const std::size_t> Shape() const;
  std::vector<Datum> GetColumn(std::size_t column_index) const;
  void InsertRow(std::size_t row_index);
  void InsertColumn(std::size_t column_index);
  void DeleteRow(std::size_t row_index);
  void DeleteColumn(std::size_t column_index);

  /**
   * @brief Outputs DataGrid values to output stream for visualization
   * @param os Output stream
   * @return Output stream with formatted DataGrid values
   */
  std::ostream& Print(std::ostream& os) const;
};

/**
 * @brief Overloaded stream insertion operator for DataGrid
 * @param os Output stream
 * @param grid DataGrid to be printed
 * @return Output stream with DataGrid content
 */
inline std::ostream& operator<<(std::ostream& os, const DataGrid& grid) {
  return grid.Print(os);
}

}  // namespace cse
