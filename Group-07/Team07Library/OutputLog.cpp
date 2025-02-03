// OutputLog.pp
// Author: Bao Tran
// Description: A logging system with multiple verbosity levels
// Created for the Euler Group Project (CSE498-Spring2025)
// 
// Some parts of this implementation were generated with the assistance of ChatGPT (OpenAI).
// Date of ChatGPT Assistance: 2/3/2025
#include "OutputLog.hpp"

// Get current timestamp as a formatted string
std::string OutputLog::getCurrentTime() {
    std::time_t now = std::time(nullptr);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

// Constructor: Open the log file and set the log level
OutputLog::OutputLog(LogLevel lvl, const std::string& filename) : level(lvl), logFile(filename, std::ios::app) {
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file: " << filename << std::endl;
    }
}

// Destructor: Close the log file
OutputLog::~OutputLog() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Set the log level dynamically
void OutputLog::setLevel(LogLevel lvl) {
    level = lvl;
}

// Log a message if the given level is within the current logging threshold
void OutputLog::log(const std::string& message, LogLevel msgLevel) {
    if (msgLevel <= level) {
        std::lock_guard<std::mutex> guard(logMutex); // Ensure thread safety

        std::string timeStr = getCurrentTime();
        std::string levelStr = (msgLevel == LogLevel::NORMAL) ? "[NORMAL]" :
                               (msgLevel == LogLevel::VERBOSE) ? "[VERBOSE]" :
                               (msgLevel == LogLevel::DEBUG) ? "[DEBUG]" : "[SILENT]";
        std::string logMessage = timeStr + " " + levelStr + " " + message;

        // Print to console
        std::cout << logMessage << std::endl;

        // Write to log file
        if (logFile.is_open()) {
            logFile << logMessage << std::endl;
        }
    }
}