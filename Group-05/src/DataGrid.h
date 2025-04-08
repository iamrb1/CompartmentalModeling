/**
 * @file DataGrid.h
 * @author Shahaab Ali and Max Krawec
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

static constexpr std::size_t kNoIndex = std::numeric_limits<std::size_t>::max();

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
  GetDoubleValues(const ReferenceVector<const Datum> &reference_vector);

  [[nodiscard]] ReferenceVector<Datum> DetermineColumnComparisons(
      std::size_t column_index,
      const Datum &value,
      operations operation);

  [[nodiscard]] static constexpr double CalculateMean(const std::vector<double>& double_values);
  [[nodiscard]] static double CalculateMedian(std::vector<double> double_values);
  [[nodiscard]] static std::vector<double> CalculateMode(const std::vector<double>& double_values);
  [[nodiscard]] static constexpr double CalculateStandardDeviation(const std::vector<double>& double_values);
  [[nodiscard]] static constexpr double CalculateMin(const std::vector<double>& double_values);
  [[nodiscard]] static constexpr double CalculateMax(const std::vector<double>& double_values);

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
    assert(IsRectangle(data));

    grid_ = data;
  }

  /**
   * Checks if DataGrid is a rectangle
   * @param data
   * @return bool
   */
  [[nodiscard]] static bool
  IsRectangle(const std::vector<std::vector<Datum>> &data) {
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
                    const double default_value_ = 0) : grid_(num_rows_, std::vector<Datum>(num_columns_, Datum(default_value_))) {}

  /**
   * @brief Create rectangular datagrid of desired size with default value
   * (string)
   * @param num_rows_
   * @param num_columns_
   * @param default_value_
   */
  DataGrid(const std::size_t num_rows_, const std::size_t num_columns_,
           const std::string &default_value_) {
    Resize(num_rows_, num_columns_, default_value_);
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
  void Clear() { grid_.clear(); }

  /**
   * @brief Index a row of the DataGrid
   * @param row_index_ Index of the row
   * @return Indexed vector row from DataGrid
   */
  [[nodiscard]] row_t &operator[](const std::size_t row_index_) {
    assert(row_index_ < grid_.size());
    return At(row_index_);
  }

  /**
   * @brief Index a const row of the DataGrid
   * @param row_index_ Index of the row
   * @return Indexed const vector row from DataGrid
   */
  [[nodiscard]] const row_t &operator[](const std::size_t row_index_) const {
    assert(row_index_ < grid_.size());
    return At(row_index_);
  }

  [[nodiscard]] row_t &GetRow(const std::size_t row_index_);
  [[nodiscard]] const row_t &GetRow(const std::size_t row_index_) const;
  [[nodiscard]] ReferenceVector<Datum> GetColumn(const std::size_t column_index_);
  [[nodiscard]] ReferenceVector<const Datum> GetColumn(const std::size_t column_index_) const;
  [[nodiscard]] Datum &GetValue(const std::size_t row_index_,
                                const std::size_t column_index_);
  [[nodiscard]] const Datum &GetValue(const std::size_t row_index_,
                                      const std::size_t column_index_) const;
  [[nodiscard]] std::vector<row_t> GetDataGrid() const { return grid_; };

  [[nodiscard]] std::tuple<std::size_t, std::size_t> Shape() const;

  void InsertDefaultRow(
      std::size_t row_index_ = kNoIndex,
      double default_value_ = 0);
  void InsertDefaultRow(
      std::size_t row_index_ = kNoIndex,
      std::string default_value_ = "");

  void InsertDefaultColumn(
      std::size_t column_index_ = kNoIndex,
      double default_value_ = 0);
  void InsertDefaultColumn(
      std::size_t column_index_ = kNoIndex,
      const std::string &default_value_ = "");

  void InsertRow(
      const std::vector<Datum>& row_,
      std::size_t row_index_ = kNoIndex);
  void InsertColumn(
      const std::vector<Datum>& column_,
      std::size_t column_index_ = kNoIndex);

  void DeleteRow(const std::size_t row_index_);
  void DeleteColumn(const std::size_t column_index_);

  void Resize(const std::size_t num_rows_, const std::size_t num_columns_,
              const double default_value_ = 0);
  void Resize(const std::size_t num_rows_, const std::size_t num_columns_,
              const std::string &default_value_);

  Datum &At(std::size_t row_index_, std::size_t column_index_);
  [[nodiscard]] const Datum &At(std::size_t row_index_,
                                std::size_t column_index_) const;
  row_t &At(std::size_t row_index_);
  [[nodiscard]] const row_t &At(std::size_t row_index_) const;

  [[nodiscard]] DataGrid Merge(const DataGrid &grid_two, bool row_append) const;
  DataGrid Slice(std::size_t start_row_index, std::size_t end_row_index,
                 std::size_t start_column_index, std::size_t end_column_index);
  void SortColumn(std::size_t column_index, bool ascending = true);
  void Sort(bool ascending = true);
  std::vector<std::size_t> Search(std::size_t column_index, const Datum &value);

  [[nodiscard]] double ColumnMean(std::size_t column_index) const;
  [[nodiscard]] double ColumnMedian(std::size_t column_index) const;
  [[nodiscard]] std::vector<double> ColumnMode(std::size_t column_index) const;
  [[nodiscard]] double ColumnStandardDeviation(std::size_t column_index) const;
  [[nodiscard]] double ColumnMin(std::size_t column_index) const;
  [[nodiscard]] double ColumnMax(std::size_t column_index) const;
  [[nodiscard]] DataGridMathSummary CalculateDataGridMathSummary() const;

  [[nodiscard]] ReferenceVector<Datum> ColumnLessThan(
      std::size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> ColumnLessThanOrEqual(
      std::size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> ColumnGreaterThan(
      size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> ColumnGreaterThanOrEqual(
      size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> ColumnNotEqual(
      size_t column_index,
      const Datum &value);
  [[nodiscard]] ReferenceVector<Datum> ColumnEqual(
      size_t column_index,
      const Datum &value);

  std::ostream &Print(std::ostream &os_) const;
};

/**
 * @brief Overloaded stream insertion operator for DataGrid
 * @param os Output stream
 * @param grid DataGrid to be printed
 * @return Output stream with DataGrid content
 */
inline std::ostream &operator<<(std::ostream &os, const DataGrid &grid) {
  return grid.Print(os);
}

} // namespace cse
