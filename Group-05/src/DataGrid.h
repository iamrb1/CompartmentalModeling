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
  std::vector<row_t> grid_;

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
   * @param row_index_ Index of the row
   * @return Indexed vector row from DataGrid
   */
  row_t& operator[](const std::size_t row_index_) {
    return grid_.at(row_index_);
  }

  /**
   * @brief Index a row of the DataGrid
   * @param row_index_ Index of the row
   * @return Indexed vector row from DataGrid
   */
  row_t& getRow(const std::size_t row_index_) {
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
  const row_t& getRow(const std::size_t row_index_) const {
    return grid_.at(row_index_);
  }

  /**
   * @brief Get Datum value from DataGrid
   * @param row_index_ Index of the row
   * @param column_index_ Index of the column
   * @return The Datum at the given index
   */
  Datum& getValue(const std::size_t row_index_, const std::size_t column_index_) {
    return grid_.at(row_index_).at(column_index_);
  }

  /**
   * @brief Get const Datum value from DataGrid (used for stream output printing)
   * @param row_index_ Index of the row
   * @param column_index_ Index of the column
   * @return The Datum at the given index
   */
  const Datum& getValue(std::size_t row_index_, std::size_t column_index_) const {
    return grid_.at(row_index_).at(column_index_);
  }

  std::tuple<const std::size_t, const std::size_t> shape() const;
  std::vector<Datum> getColumn(std::size_t column_index_) const;
  void insertRow(std::size_t row_index_);
  void insertColumn(std::size_t column_index_);
  void deleteRow(std::size_t row_index_);
  void deleteColumn(std::size_t column_index_);
  std::ostream& print(std::ostream& os_) const;
};

/**
 * @brief Overloaded stream insertion operator for DataGrid
 * @param os Output stream
 * @param grid DataGrid to be printed
 * @return Output stream with DataGrid content
 */
inline std::ostream& operator<<(std::ostream& os, const DataGrid& grid) {
  return grid_.print(os);
}

}  // namespace cse
