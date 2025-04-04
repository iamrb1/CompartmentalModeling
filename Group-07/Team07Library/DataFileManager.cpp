#include "DataFileManager.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::function;
using std::getline;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

namespace cse {

/**
 * Writes data rows to the file.
 */
void DataFileManager::writeRowsToCSV(ofstream &file, const vector<vector<string>> &data) {
  for (const auto &row : data) {
    for (size_t i = 0; i < row.size(); ++i) {
      file << row[i];
      if (i < row.size() - 1) {
        file << ",";
      }
    }
    file << "\n";
  }
}

/**
 * Opens a CSV file and reads its content into csvData.
 * @param path The file path of the CSV to open.
 */
void DataFileManager::openCSV(const string &path) {
  // Check if a file is already open
  if (!fileLocation.empty()) {
    cerr << "Please close the current file before attempting to open a new one." << endl;
    return;
  }

  // Handles CSV Case,
  // https://en.cppreference.com/w/cpp/string/basic_string/find_last_of
  if (path.substr(path.find_last_of(".") + 1) != "csv") {
    // https://en.cppreference.com/w/cpp/io/cerr
    cerr << "File extension is not valid." << endl;
    return;
  }

  // Check if the file can be opened
  ifstream file(path);
  if (!file.is_open()) {
    cerr << "Error opening file: " << path << endl;
    return;
  }

  vector<vector<string>> data;
  string line, cell;

  // Read the file line by line
  while (getline(file, line)) {
    vector<string> row;
    stringstream lineStream(line);

    // Read each comma-separated cell
    while (getline(lineStream, cell, ',')) {
      row.push_back(cell);
    }
    data.push_back(row);
  }

  // Record the file location and read in data
  setFile(path);
  setDataCSV(data);
}

/**
 * Updates the CSV based on added functions.
 */
void DataFileManager::updateCSV() {
  if (fileLocation.empty() or fileLocation.substr(fileLocation.find_last_of(".") + 1) != "csv") {
    cerr << "No valid file is currently open." << endl;
    return;
  }

  if (csvData.empty()) {
    // Add headers only if the csvData is empty
    vector<string> headers;
    for (const auto &pair : functionMap) {
      headers.push_back(pair.first);
    }
    csvData.push_back(headers);
  }

  vector<string> newRow;
  for (const auto &pair : functionMap) {
    newRow.push_back(std::to_string(pair.second()));
  }
  csvData.push_back(newRow);
  updateMade = true;

  // Saves the current state of csvData to the file
  ofstream file(fileLocation);
  if (file.is_open()) {
    writeRowsToCSV(file, csvData);
    file.close();
  }
  updateMade = false;
}

/**
 * Closes the file and writes any updates made to the data.
 */
void DataFileManager::closeCSV() {
  // No need to write if no updates have been made
  if (!updateMade) {
    return;
  }

  ofstream file(fileLocation, std::ios::trunc); // Open file in truncate mode to overwrite
  if (!file.is_open()) {
    cerr << "Error opening file" << endl;
    return;
  }

  // Write header if functionMap is not empty
  if (!csvData.empty()) {
    for (const auto &pair : functionMap) {
      file << pair.first << ",";
    }
    file.seekp(-1, std::ios_base::end); // Remove the last comma
    file << "\n";
  }

  // Write each row of data to the file
  writeRowsToCSV(file, csvData);

  // Clear the file data after writing
  csvData.clear();
}

/**
 * Opens a JSON file and reads its content into csvData.
 * @param path The file path of the CSV to open.
 */
void DataFileManager::openJSON(const string &path) {
  // Check if a file is already open
  if (!fileLocation.empty()) {
    cerr << "Please close the current file before attempting to open a new one." << endl;
    return;
  }

  // Handles Error Case
  if (path.substr(path.find_last_of(".") + 1) != "json") {
    cerr << "File extension is not valid." << endl;
    return;
  }

  // Check if the file can be opened
  ifstream file(path);
  if (!file.is_open()) {
    cerr << "Error opening file: " << path << endl;
    return;
  }

  try {
    // Create an empty property tree
    boost::property_tree::ptree pt;

    // Read the JSON file into the property tree
    boost::property_tree::read_json(path, pt);

    // Record the file location and read in data
    setFile(path);
    setDataJSON(pt);
  }

  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

}

void DataFileManager::updateJSON() {
  if (fileLocation.empty() or fileLocation.substr(fileLocation.find_last_of(".") + 1) != "json") {
    cerr << "No valid file is currently open." << endl;
    return;
  }

  try {
    // Create a property tree and parse the JSON file
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(fileLocation, pt);

    // Iterate through the functionMap and update JSON data
    for (const auto& entry : functionMap) {
      const std::string& key = entry.first;
      const std::function<int()>& func = entry.second;

      // Traverse the property tree to find and update the corresponding key
      try {
        pt.put(key, func()); // Update the value with the result of the lambda function
      } catch (const boost::property_tree::ptree_bad_path& e) {
        cerr << "Key not found in JSON: " << key << endl;
      }
    }

    // Save the updated JSON back to the file
    boost::property_tree::write_json(getFile(), pt);
    // Update our variable saving the JSON file
    setDataJSON(pt);

    cout << "JSON file successfully updated." << endl;
  }

  catch (const std::exception& e) {
    cerr << "Error updating JSON: " << e.what() << endl;
  }
}

/**
 * Closes the JSON file while writing any updates made to the data.
 */
void DataFileManager::closeJSON() {
  // No need to write if no updates have been made
  if (!updateMade) {
    return;
  }

  // Clear the file data after writing
  boost::property_tree::ptree emptyTree; // Create an empty property tree
  setDataJSON(emptyTree); // Pass the empty tree to setDataJSON
}

}
