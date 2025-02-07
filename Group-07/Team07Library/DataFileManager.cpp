#include "DataFileManager.h"
#include <fstream> // https://stackoverflow.com/questions/13035674/how-to-read-a-file-line-by-line-or-a-whole-text-file-at-
#include <iostream>

namespace cse {
    // Load in path to file as a string
    DataFileManager::DataFileManager(const std::string& path) : filePath(path) {}

    // Open CSV file based on location (path variable)
    void DataFileManager::openFile() {
        // Handles CSV Case, https://en.cppreference.com/w/cpp/string/basic_string/find_last_of
        if (filePath.substr(filePath.find_last_of(".") + 1) == "csv") {
        	std::cout << "File has .csv extension." << std::endl;
    	}
        // Handles Error
        else {
        	std::cout << "File does not have valid extension." << std::endl;
    	}
    }

    // Updates the file based on provided functions and builds new rows
    void DataFileManager::update() {
        // Iterate over the unordered_map and call each function
        for (const auto& pair : functionMap) {
            std::cout << "Calling " << pair.first << std::endl;
            pair.second();  // Call the function
        }
    }

    void addFunction(const std::string& name, const std::function<void()>& lambda){
        auto var1 = name;
        auto var2 = lambda;
    }

    void DataFileManager::closeFile() {
        filePath = "";
    }
}
