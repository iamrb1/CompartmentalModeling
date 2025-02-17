#ifndef DATAFILEMANAGER_H
#define DATAFILEMANAGER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace cse {
    // Class to manage data file operations, particularly CSV files
    class DataFileManager {
    private:
        // https://stackoverflow.com/questions/55520876/creating-an-unordered-map-of-stdfunctions-with-any-arguments
        std::unordered_map<std::string, std::function<int()>> functionMap;  // Stores functions for data generation
        std::string fileLocation;  // Path to the CSV file
        std::vector<std::vector<std::string>> fileData;  // Data read from the CSV file
        bool updateMade;  // Flag indicating if an update has been made to the data

    public:
        // Default constructor
        DataFileManager() = default;

        // Parameterized constructor initializes file location and opens the file
        DataFileManager(const std::string& path) : fileLocation(path), updateMade(false) {
            openFile(path);
        }

        // Destructor
        ~DataFileManager() = default;

        // Returns the file location
        const std::string& getFile() const {
            return fileLocation;
        }

        // Set the file location
        void setFile(const std::string& path) {
            fileLocation = path;
        }

        // Check if an update has been made
        bool checkUpdate() const {
            return updateMade;
        }

        // Set the update flag
        void setUpdate(bool status) {
            updateMade = status;
        }

        // Set the data read from our file
        void setData(const std::vector<std::vector<std::string>>& data) {
            fileData = data;
        }

        // Return data read from the current file
        std::vector<std::vector<std::string>> getData() const {
            return fileData;
        }

        // Clears all stored functions
        void clearFunctions() {
            functionMap.clear();
        }

        // Adds a function to the map
        void addFunction(const std::string& name, const std::function<int()>& lambda) {
            functionMap[name] = lambda;
        }

        // Deletes a function from our map
        void deleteFunction(const std::string& name) {
            // https://cplusplus.com/reference/unordered_map/unordered_map/erase/
            auto it = functionMap.find(name);
            if (it != functionMap.end()) {
                functionMap.erase(it);
            }
        }

        // Lists all store functions
        void listFunctions() const {
            // https://stackoverflow.com/questions/22880431/iterate-through-unordered-map-c
            for (const auto& [key, value] : functionMap) {
                std::cout << key << std::endl;
            }
        }

        // Opens a CSV file to read its contents
        void openFile(const std::string& path);

        // Updates our CSV file with new data generated from functions
        void update();

        // Closes the file and writes any updates made to the data
        void closeFile();
    };
}

#endif
