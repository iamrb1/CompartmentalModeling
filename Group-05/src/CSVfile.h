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
  
   static DataGrid LoadCsv(const std::string& file_name, char delimiter = ',');
   static bool ExportCsv(const std::string& file_name, const DataGrid& grid, char delimiter = ',');
 };
 
 }  // namespace cse
