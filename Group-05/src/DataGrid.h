/**
 * @file DataGrid.h
 * @author Shahaab Ali
 *
 * @brief A 2D data table providing a simple and efficient tabular interface.
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <limits>
#include <vector>

#include "Datum.h"
#include "ReferenceVector.h"

namespace cse {

/**
 * @class DataGrid
 * @brief A 2D data table providing a simple and efficient tabular interface.
 */
class DataGrid {
private:
  using row_t = std::vector<Datum>; // row_t is an alias for a vector of Datums

  /// A 2D vector holding datums
  std::vector<row_t> grid_;

  /// Operations for column comparison
  enum class operations {
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    EQUAL,
    NOT_EQUAL
  };

  [[nodiscard]] static std::vector<double>
  getDoubleValues(const ReferenceVector<const Datum> &reference_vector);

  [[nodiscard]] ReferenceVector<Datum> determineColumnComparisons(
                                                    size_t column_index,
                                                    const Datum &value,
                                                    operations operation);

  [[nodiscard]] static double calculateMean(std::vector<double> double_values);
  [[nodiscard]] static double calculateMedian(std::vector<double> double_values);
  [[nodiscard]] static std::vector<double> calculateMode(const std::vector<double>& double_values);
  [[nodiscard]] static double calculateStandardDeviation(const std::vector<double>& double_values);
  [[nodiscard]] static double calculateMin(std::vector<double> double_values);
  [[nodiscard]] static double calculateMax(std::vector<double> double_values);

public:
  /// Struct for the DataGrid mathematical summary
  struct DataGridMathSummary {
    double mean;
    double median;
    std::vector<double> mode;
    double standardDeviation;
    double min;
    double max;
  };

  /**
   * Constructor using 2D Vector of Datums
   * @param data
   */
  explicit DataGrid(const std::vector<std::vector<Datum>> &data) {
    assert(data.empty() || !data[0].empty());
    assert(isRectangle(data));

    grid_ = data;
  }

  /**
   * Checks if DataGrid is a rectangle
   * @param data
   * @return bool
   */
  [[nodiscard]] static bool
  isRectangle(const std::vector<std::vector<Datum>> &data) {
    if (data.empty())
      return true;

    // Used ChatGPT to replace my range based loop with range method recommended
    // by clang
    return data.empty() ||
           std::ranges::all_of(
               data, [row_one_length = data[0].size()](const auto &row) {
                 return row.size() == row_one_length;
               });
  }

  /**
   * @brief Create rectangular datagrid of desired size with default value
   * (double)
   * @param num_rows_
   * @param num_columns_
   * @param default_value_
   */
  explicit DataGrid(const std::size_t num_rows_ = 0,
                    const std::size_t num_columns_ = 0,
                    const double default_value_ = 0) {
    resize(num_rows_, num_columns_, default_value_);
  }

  /**
   * @brief Create rectangular datagrid of desired size with default value
   * (string)
   * @param num_rows_
   * @param num_columns_
   * @param default_value_
   */
  DataGrid(const std::size_t num_rows_, const std::size_t num_columns_,
           const std::string &default_value_) {
    resize(num_rows_, num_columns_, default_value_);
  }

  /**
   * @brief Default Copy Constructor
   */
  DataGrid(const DataGrid &) = default;

  /**
   * @brief Default Move Constructor
   */
  DataGrid(DataGrid &&) = default;

  /**
   * @brief Default Copy Assignment Operator
   */
  DataGrid &operator=(const DataGrid &) = default;

  /**
   * @brief Default Move Assignment Operator
   */
  DataGrid &operator=(DataGrid &&) = default;

  /**
   * @brief Default Destructor
   */
  ~DataGrid() = default;

  /**
   * @brief Delete all data in datagrid
   */
  void clear() { grid_.clear(); }

  /**
   * @brief Index a row of the DataGrid
   * @param row_index_ Index of the row
   * @return Indexed vector row from DataGrid
   */
  [[nodiscard]] row_t &operator[](const std::size_t row_index_) {
    assert(row_index_ < grid_.size());
    return at(row_index_);
  }

  /**
   * @brief Index a const row of the DataGrid
   * @param row_index_ Index of the row
   * @return Indexed const vector row from DataGrid
   */
  [[nodiscard]] const row_t &operator[](const std::size_t row_index_) const {
    assert(row_index_ < grid_.size());
    return at(row_index_);
  }

  [[nodiscard]] row_t &getRow(std::size_t row_index_);
  [[nodiscard]] const row_t &getRow(std::size_t row_index_) const;
  [[nodiscard]] ReferenceVector<Datum> getColumn(std::size_t column_index_);
  [[nodiscard]] ReferenceVector<const Datum> getColumn(size_t column_index_) const;
  [[nodiscard]] Datum &getValue(std::size_t row_index_,
                                std::size_t column_index_);
  [[nodiscard]] const Datum &getValue(std::size_t row_index_,
                                      std::size_t column_index_) const;
  [[nodiscard]] std::vector<row_t> getDataGrid() const { return grid_; };

  [[nodiscard]] std::tuple<std::size_t, std::size_t> shape() const;

  void insertDefaultRow(
      std::size_t row_index_ = std::numeric_limits<std::size_t>::max(),
      double default_value_ = 0);
  void insertDefaultRow(
      std::size_t row_index_ = std::numeric_limits<std::size_t>::max(),
      std::string default_value_ = "");

  void insertDefaultColumn(
      std::size_t column_index_ = std::numeric_limits<std::size_t>::max(),
      double default_value_ = 0);
  void insertDefaultColumn(
      std::size_t column_index_ = std::numeric_limits<std::size_t>::max(),
      const std::string &default_value_ = "");

void insertRow(
  std::vector<Datum> row_,
  std::size_t row_index_ = std::numeric_limits<std::size_t>::max());
void insertColumn(
    const std::vector<Datum>& column_,
    std::size_t column_index_ = std::numeric_limits<std::size_t>::max());

  void deleteRow(std::size_t row_index_);
  void deleteColumn(std::size_t column_index_);

  void resize(std::size_t num_rows_, std::size_t num_columns_,
              double default_value_ = 0);
  void resize(std::size_t num_rows_, std::size_t num_columns_,
              const std::string &default_value_);

  Datum &at(std::size_t row_index_, std::size_t column_index_);
  [[nodiscard]] const Datum &at(std::size_t row_index_,
                                std::size_t column_index_) const;
  row_t &at(std::size_t row_index_);
  [[nodiscard]] const row_t &at(std::size_t row_index_) const;

  [[nodiscard]] DataGrid merge(const DataGrid &grid_two, bool row_append) const;
  DataGrid slice(std::size_t start_row_index, std::size_t end_row_index,
                 std::size_t start_column_index, std::size_t end_column_index);
  void sortColumn(std::size_t column_index, bool ascending = true);
  void sort(bool ascending = true);
  std::vector<std::size_t> search(std::size_t column_index, const Datum &value);

  [[nodiscard]] double columnMean(std::size_t column_index) const;
  [[nodiscard]] double columnMedian(std::size_t column_index) const;
  [[nodiscard]] std::vector<double> columnMode(std::size_t column_index) const;
  [[nodiscard]] double columnStandardDeviation(std::size_t column_index) const;
  [[nodiscard]] double columnMin(std::size_t column_index) const;
  [[nodiscard]] double columnMax(std::size_t column_index) const;
  [[nodiscard]] DataGridMathSummary dataGridMathSummary() const;

  [[nodiscard]] ReferenceVector<Datum> columnLessThan(
      size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> columnLessThanOrEqual(
      size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> columnGreaterThan(
      size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> columnGreaterThanOrEqual(
      size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> columnNotEqual(
      size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> columnEqual(
      size_t column_index,
      const Datum &value);

  std::ostream &print(std::ostream &os_) const;
};

/**
 * @brief Overloaded stream insertion operator for DataGrid
 * @param os Output stream
 * @param grid DataGrid to be printed
 * @return Output stream with DataGrid content
 */
inline std::ostream &operator<<(std::ostream &os, const DataGrid &grid) {
  return grid.print(os);
}

} // namespace cse
