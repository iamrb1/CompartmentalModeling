/**
 * @file CSVfile.h
 * @author Muhammad Masood
 * @brief Manages CSV file interactions with DataGrid.
 *
 * This class provides static functions to load CSV data into a DataGrid
 * and export a DataGrid to a CSV file. It handles basic parsing,
 * type differentiation, and error handling.
 */

 #pragma once

 #include <string>
 #include "DataGrid.h"
 
 namespace cse {
 
 /**
  * @class CSVFile
  * @brief Handles CSV file operations like loading and exporting.
  *
  * Provides functions to load data from a CSV file into a DataGrid and 
  * export the DataGrid content into a CSV file, with proper handling 
  * of data types and error cases.
  */
 class CSVFile {
  public:
   /**
    * @brief Loads a CSV file into a DataGrid.
    *
    * Reads the CSV file line by line, splits each line using the specified
    * delimiter, and converts tokens into Datum objects.
    *
    * @param file_name The name of the CSV file to load.
    * @param delimiter The delimiter used in the CSV file (default is comma).
    * @return A DataGrid populated with the CSV data.
    * @throws std::runtime_error If the file cannot be opened.
    */
   static DataGrid LoadCsv(const std::string& file_name, char delimiter = ',');
 
   /**
    * @brief Exports a DataGrid to a CSV file.
    *
    * Iterates over the DataGrid, sanitizes tokens, and writes the data to a CSV file.
    *
    * @param file_name The name of the CSV file to write.
    * @param grid The DataGrid to export.
    * @param delimiter The delimiter used in the CSV file (default is comma).
    * @return True if the export was successful.
    * @throws std::runtime_error If the file cannot be written.
    */
   static bool ExportCsv(const std::string& file_name, const DataGrid& grid, char delimiter = ',');
 };
 
 }  // namespace cse
 