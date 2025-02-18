#include "DataFileManager.h"
#include <fstream> // https://stackoverflow.com/questions/13035674/how-to-read-a-file-line-by-line-or-a-whole-text-file-at-include <fstream>
#include <iostream>

namespace cse {

/**
 * Opens a CSV file and reads its content into fileData.
 * @param path The file path of the CSV to open.
 */
void DataFileManager::openFile(const std::string& path) {
    // Check if a file is already open
    if (!fileLocation.empty()) {
        std::cerr << "Please close the current file before attempting to open a new one." << std::endl;
        return;
    }

    // Handles CSV Case, https://en.cppreference.com/w/cpp/string/basic_string/find_last_of
    if (path.substr(path.find_last_of(".") + 1) != "csv") {
        // https://en.cppreference.com/w/cpp/io/cerr
        std::cerr << "File extension is not valid." << std::endl;
        return;
    }

    // Check if the file can be opened
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return;
    }

    std::vector<std::vector<std::string>> data;
    std::string line, cell;

    // Read the file line by line
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream lineStream(line);

        // Read each comma separated cell
        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    // Record the file location and read in data
    setFile(path);
    setData(data);
}

/**
 * Updates the CSV based on added functions.
 */
void DataFileManager::update() {
    if (fileLocation.empty()) {
        std::cerr << "No file is currently open." << std::endl;
        return;
    }

    if (fileData.empty()) {
        // Add headers only if the fileData is empty
        std::vector<std::string> headers;
        for (const auto& pair : functionMap) {
            headers.push_back(pair.first);
        }
        fileData.push_back(headers);
    }

    std::vector<std::string> newRow;
    for (const auto& pair : functionMap) {
        newRow.push_back(std::to_string(pair.second()));
    }
    fileData.push_back(newRow);
    updateMade = true;

    // Saves the current state of fileData to the file
    std::ofstream file(fileLocation);
    if (file.is_open()) {
        for (const auto& row : fileData) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) {
                    file << ",";
                }
            }
            file << "\n";
        }
        file.close();
    }
    updateMade = false;
}

/**
 * Closes the file and writes any updates made to the data.
 */
void DataFileManager::closeFile() {
    // No need to write if no updates have been made
    if (!updateMade) {
        return;
    }

    std::ofstream file(fileLocation, std::ios::trunc); // Open file in truncate mode to overwrite
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    // Write header if functionMap is not empty
    if (!fileData.empty()) {
        for (const auto& pair : functionMap) {
            file << pair.first << ",";
        }
        file.seekp(-1, std::ios_base::end); // Remove the last comma
        file << "\n";
    }

    // Write each row of data to the file
    for (const auto& row : fileData) {
        for (size_t j = 0; j < row.size(); ++j) {
            file << row[j];
            if (j < row.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    // Clear the file data after writing
    fileData.clear();
}

}
