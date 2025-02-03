// OutputLog.hpp
// Author: Bao Tran
// Description: A logging system with multiple verbosity levels
// Created for the Euler Group Project (CSE498-Spring2025)
// 
// Some parts of this implementation were generated with the assistance of ChatGPT (OpenAI).
// Date of ChatGPT Assistance: 2/3/2025
#ifndef OUTPUT_LOG_HPP
#define OUTPUT_LOG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <mutex>

// Logging levels
enum class LogLevel { SILENT, NORMAL, VERBOSE, DEBUG };

class OutputLog {
private:
    LogLevel level;         // Current logging level
    std::ofstream logFile;  // Log file stream
    std::mutex logMutex;    // Mutex for thread safety

    // Private function to get the current timestamp
    std::string getCurrentTime();

public:
    // Constructor with log level and filename
    OutputLog(LogLevel lvl = LogLevel::NORMAL, const std::string& filename = "log.txt");

    // Destructor to close the log file
    ~OutputLog();

    // Set logging level dynamically
    void setLevel(LogLevel lvl);

    // Log a message with the given log level
    void log(const std::string& message, LogLevel msgLevel);
};

#endif // OUTPUT_LOG_HPP