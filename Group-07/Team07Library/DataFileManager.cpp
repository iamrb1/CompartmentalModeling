#include "DataFileManager.h"
#include <fstream> // https://stackoverflow.com/questions/13035674/how-to-read-a-file-line-by-line-or-a-whole-text-file-at-once

// Load in path to file as a string
DataFileManager::DataFileManager(const std::string& path) : filePath(path) {}

void DataFileManager::update() {
}

void DataFileManager::addFunction() {
}

void DataFileManager::openFile() {
	std::ifstream file(filePath); // Read file
	if (!file.is_open()){ // Error check
		throw std::runtime_error("Filepath doesn't exist");
	}
}

void DataFileManager::closeFile() {
}

void DataFileManager::clearFunctions() {
}

void DataFileManager::listFunctions() {
}
